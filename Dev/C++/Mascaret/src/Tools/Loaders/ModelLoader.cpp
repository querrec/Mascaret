#include "Tools/filesystem.h"
#include "Tools/Loaders/ModelLoader.h"
#include "MascaretApplication.h"

#include "VEHA/Kernel/Property.h"
#include "VEHA/Kernel/Operation.h"
#include "VEHA/Kernel/LiteralSpecification.h"
#include "VEHA/Kernel/LiteralReal.h"
#include "VEHA/Kernel/LiteralInteger.h"
#include "VEHA/Kernel/LiteralBoolean.h"
#include "VEHA/Kernel/LiteralString.h"
#include "VEHA/Kernel/Expression.h"
#include "VEHA/Kernel/Constraint.h"
#include "VEHA/Kernel/OpaqueExpression.h"
#include "VEHA/Entity/EntityClass.h"
#include "VEHA/Behavior/Common/OpaqueBehavior.h"
#include "VEHA/Behavior/Common/OclQuery.h"
#ifndef NOPYTHON
#include "VEHA/Behavior/Common/OpaquePythonBehavior.h"
#endif

#include "VEHA/Behavior/StateMachine/CallEvent.h"
#include "VEHA/Behavior/StateMachine/SignalEvent.h"
#include "VEHA/Behavior/StateMachine/ChangeEvent.h"
#include "VEHA/Behavior/StateMachine/TimeEvent.h"
#include "VEHA/Behavior/StateMachine/StateMachine.h"
#include "VEHA/Behavior/StateMachine/Region.h"
#include "VEHA/Behavior/StateMachine/State.h"
#include "VEHA/Behavior/StateMachine/Transition.h"
#include "VEHA/Behavior/StateMachine/PseudoState.h"
#include "VEHA/Behavior/StateMachine/FinalState.h"

#include "VEHA/Behavior/BasicActions/CallOperationAction.h"
#include "VEHA/Behavior/BasicActions/SendSignalAction.h"
#include "VEHA/Behavior/BasicActions/PlayAnimationAction.h"
#include "VEHA/Behavior/BasicActions/SetInstanceInRedAction.h"
#include "VEHA/Behavior/BasicActions/SetInstanceTransparencyAction.h"
#include "VEHA/Behavior/BasicActions/GrowUpInstanceAction.h"
#include "VEHA/Behavior/BasicActions/RestoreEnvironmentAction.h"

#include "VEHA/Behavior/Activity/Activity.h"
#include "VEHA/Behavior/Activity/ActionNode.h"
#include "VEHA/Behavior/Activity/ObjectNode.h"
#include "VEHA/Behavior/Activity/ActivityEdge.h"
#include "VEHA/Behavior/Activity/InitialNode.h"
#include "VEHA/Behavior/Activity/JoinNode.h"
#include "VEHA/Behavior/Activity/ForkNode.h"
#include "VEHA/Behavior/Activity/ChoiceNode.h"
#include "VEHA/Behavior/Activity/FinalNode.h"
#include "VEHA/Behavior/Activity/ActivityPartition.h"

#include "HAVE/VirtualHumanClass.h"

#include <stdexcept>
#include <iostream>
#include "VEHA/Behavior/Common/pluginManager.h"
#include <boost/lexical_cast.hpp>
#include "Tools/utils.h"

DECLARE_NAMESPACE_FILESYSTEM;

using namespace VEHA;
using namespace HAVE;

ModelLoader::ModelLoader(shared_ptr<Model> model, string str, bool strIsFileName)
{
	if (str !="")
	{
		_xmiParser =shared_ptr<XMIParser>( new XMIParser());
		if (strIsFileName) model->setUrl(str);

		if ( (strIsFileName && _xmiParser->parseFile(str))
				|| (!strIsFileName && _xmiParser->parseString(str)))
		{
			_model = model;

			shared_ptr<XMIPackage> package = _getInitialPackage();

			string filename=(strIsFileName?str:package->getName());
			_addDataTypes(package);
			_addStereotypes(package);

			//      _model->setName(package->getName());
			//      _model->setSummary(package->getSummary());
			//      _model->setDescription(package->getDescription());
			_model->setPackage(_addClassesFromPackage(package));
			_setParametersType();
			_addGeneralisations();
			_addAssociations(package);
			_addStateMachines(package);
			_addActivities(package);

                        #if BOOST_FILESYSTEM_VERSION == 2
			string basename=fs::path(filename).leaf();
                        #else
			string basename=fs::path(filename).leaf().string();
                        #endif
			/*PluginManager::getInstance()->updatePlugin(basename+"_GeneratedOpaqueBehaviors",_opaqueBehaviorsCode);

			if (!PluginManager::getInstance()->openPlugin(basename+"_GeneratedOpaqueBehaviors.so"))
				cerr<<"can't open plugin associate to "<<filename<<" model ("<<basename<<"_GeneratedOpaqueBehaviors.so)"<<endl;*/

			shared_ptr<Class>  classe(new EntityClass("undef"));

			_model->getPackage()->addClasses(classe);
			_model->registerAllClasses();
		}
		else
		{
			cerr << "No model found" << endl;
		}
	}
}

ModelLoader::ModelLoader(shared_ptr<Model> model, shared_ptr<XmlNode> xmiRootNode)
{
	_xmiParser =shared_ptr<XMIParser>( new XMIParser());
	if ( _xmiParser->parseNode(xmiRootNode))
	{
		_model = model;

		shared_ptr<XMIPackage> package = _getInitialPackage();

		string filename=(package->getName());
		_addDataTypes(package);
		_addStereotypes(package);

		//      _model->setName(package->getName());
		//      _model->setSummary(package->getSummary());
		//      _model->setDescription(package->getDescription());
		_model->setPackage(_addClassesFromPackage(package));
		_setParametersType();
		_addGeneralisations();
		_addAssociations(package);
		_addStateMachines(package);
		_addActivities(package);

		#if BOOST_FILESYSTEM_VERSION == 2
		string basename=fs::path(filename).leaf();
                #else
		string basename=fs::path(filename).leaf().string();
		#endif
		PluginManager::getInstance()->updatePlugin(basename+"_GeneratedOpaqueBehaviors",_opaqueBehaviorsCode);

		if (!PluginManager::getInstance()->openPlugin(basename+"_GeneratedOpaqueBehaviors.so"))
			cerr<<"can't open plugin associate to "<<filename<<" model ("<<basename<<"_GeneratedOpaqueBehaviors.so)"<<endl;

		shared_ptr<Class>  classe(new EntityClass("undef"));

		_model->getPackage()->addClasses(classe);
		_model->registerAllClasses();
	}
	else
	{
		cerr << "No model found" << endl;
	}
}

void ModelLoader::reload(shared_ptr<Class> classe)
{
	cerr << " Reloading classe " << classe->getName() << endl;
	cerr << _model->getUrl() << endl;
	cerr << _xmiParser << endl;
	_xmiParser->parseFile(_model->getUrl());
	shared_ptr<XMIPackage> package = _getInitialPackage();
	shared_ptr<XMIClasse> xmiClasse = package->getClasse(classe->getName(),true);

	if (xmiClasse)
	{
		cerr << "XMI : " << xmiClasse->getName() << endl;
		_reloadClasse(classe, xmiClasse);
	}

}

shared_ptr<XMIPackage> ModelLoader::_getInitialPackage(void)
{
	if (_xmiParser)
		return _xmiParser->getPackage();
	else
		return shared_ptr<XMIPackage>();
}


void ModelLoader::_addStereotypes(shared_ptr<XMIPackage> package)
{
	unsigned int nType = 1;
	shared_ptr<XMIStereotype> xmiStereotype;
	bool found = true;
	while (found)
	{
		xmiStereotype = package->getStereotype(nType++);
		if (xmiStereotype != NULL)
			_stereotypes[xmiStereotype->getId()] = xmiStereotype->getName();
		else
			found = false;
	}

	unsigned int nPackage = 1;
	shared_ptr<XMIPackage>xmiPackage;
	bool found2 = true;
	while (found2)
	{
		xmiPackage = package->getPackage(nPackage++);
		if (xmiPackage != NULL)
			_addStereotypes(xmiPackage);
		else
			found2 = false;
	}
}
void ModelLoader::_addDataTypes(shared_ptr<XMIPackage> package)
{
	unsigned int nType = 1;
	shared_ptr<XMIDataType> xmiType;
	bool found = true;
	while (found)
	{
		xmiType = package->getDataType(nType++);
		if (xmiType != NULL)
		{
			shared_ptr<PrimitiveType> type(new PrimitiveType(xmiType->getName() ));
			_model->addBasicType(type);
			//On recupere ceux qui existe dj
			type=_model->getBasicType(xmiType->getName());
			_basicTypes[xmiType->getId()] = type;
			_classifiers[xmiType->getId()] = type;
		}
		else
			found = false;
	}
	unsigned int nPackage = 1;
	shared_ptr<XMIPackage>xmiPackage;
	bool found2 = true;
	while (found2)
	{
		xmiPackage = package->getPackage(nPackage++);
		if (xmiPackage != NULL)
			_addDataTypes(xmiPackage);
		else
			found2 = false;
	}
}


shared_ptr<PrimitiveType> ModelLoader::getDataType(string dataTypeRef)
{
	map<string, shared_ptr<PrimitiveType> >::iterator i = _basicTypes.find(dataTypeRef);
	if (i != _basicTypes.end()) return i->second;
	else return shared_ptr<PrimitiveType>();
}


void ModelLoader::_addGeneralisations(void)
{
	for (size_t i = 0; i < _generalisations.size(); i++)
	{
		//      Generalization * gen = new Generalization();
		shared_ptr<Class> general = _classes[_generalisations[i].first];
		shared_ptr<Class> specific = _classes[_generalisations[i].second];
		general->addChild(specific);
		//      gen->setGeneral(general);
		//      gen->setSpecific(specific);
		//      general->addGeneralization(gen);
		//      specific->addGeneralization(gen);
	}
}


shared_ptr<Package> ModelLoader::_addClassesFromPackage(shared_ptr<XMIPackage> package)
{
	shared_ptr<Package> mPackage( new Package(package->getName()) );
	mPackage->setDescription(package->getDescription());
	mPackage->setSummary(package->getSummary());
	mPackage->setTags(package->getTags());
	_packages[package->getId()] = mPackage;

	unsigned int nSignal = 1;
	shared_ptr<XMISignal> xmiSignal;
	bool foundS = true;
	while (foundS)
	{
		xmiSignal = package->getSignal(nSignal++);
		if (xmiSignal != NULL)
		{
			shared_ptr<Signal> signal( new Signal(xmiSignal->getName()) );
			_signals[xmiSignal->getId()] = signal;
			//         _parseSignal(signal,xmiSignal);
			mPackage->addSignals(signal);
		}
		else
			foundS = false;
	}

	unsigned int nClasses = 1;
	shared_ptr<XMIClasse> xmiClasse;
	bool found = true;
	while (found)
	{
		xmiClasse = package->getClasse(nClasses++);
		if (xmiClasse != NULL)
		{
			_xmiClasses.push_back(xmiClasse);

			string stereotypeRef = xmiClasse->getStereotypeRef();
			string stereotype = "Class";
			if (stereotypeRef != "") stereotype =  _stereotypes[stereotypeRef];

			shared_ptr<Class> classe;

			if (stereotype == "Entity")
			{
				shared_ptr<Class> classe2( new EntityClass(xmiClasse->getName()) );
				classe = classe2;
			}
			else if (stereotype == "VirtualHuman")
			{
				shared_ptr<Class> classe2( new VirtualHumanClass(xmiClasse->getName()) );
				classe = classe2;
			}
			else if (stereotype == "RoleClass")
			{
				shared_ptr<Class> classe2( new RoleClass(xmiClasse->getName()) );
				_parseClasse(classe2,xmiClasse);
				_roleClasses[xmiClasse->getId()] = shared_dynamic_cast<RoleClass>(classe2);
			}
			else
			{
				shared_ptr<Class> classe2( new Class(xmiClasse->getName()) );
				classe = classe2;
			}

			if (stereotype != "RoleClass")
			{
				_classes[xmiClasse->getId()] = classe;
				_classifiers[xmiClasse->getId()] = classe;
				_parseClasse(classe,xmiClasse);
				mPackage->addClasses(classe);
			}
		}
		else
			found = false;
	}

	unsigned int nPackage = 1;
	shared_ptr<XMIPackage>xmiPackage;
	bool found2 = true;
	while (found2)
	{
		xmiPackage = package->getPackage(nPackage++);
		if (xmiPackage != NULL)
		{
			_addTeamsFromPackage(xmiPackage);
			mPackage->addPackage(_addClassesFromPackage(xmiPackage));
/*
			string stereotypeRef = xmiPackage->getStereotypeRef();
			string stereotype = "Package";
			if (stereotypeRef != "") stereotype =  _stereotypes[stereotypeRef];

			if (stereotype == "Team" )
			{
				mPackage->addPackage(_addClassesFromPackage(xmiPackage));
				_addTeamsFromPackage(xmiPackage);
			}
			else
				mPackage->addPackage(_addClassesFromPackage(xmiPackage));
*/
		}
		else
			found2 = false;
	}
	return mPackage;
}

void ModelLoader::_addTeamsFromPackage(shared_ptr<XMIPackage> package)
{

	shared_ptr<OrganisationalStructure> organisation( new OrganisationalStructure(package->getName()) );
	organisation->setDescription(package->getDescription());
	organisation->setSummary(package->getSummary());
	organisation->setTags(package->getTags());
	_organisations[package->getId()] = organisation;


}

void ModelLoader::_parseSignal(shared_ptr<Signal> signal, shared_ptr<XMISignal> xmiSignal)
{
	//   signal->setName(xmiSignal->getName());
	//   signal->setId(xmiSignal->getId());
	signal->setSummary(xmiSignal->getSummary());
	signal->setDescription(xmiSignal->getDescription());
	signal->setTags(xmiSignal->getTags());

	_addAttributes(signal, xmiSignal);

}


void ModelLoader::_reloadClasse(shared_ptr<Class> classe, shared_ptr<XMIClasse> xmiClasse)
{
	//   classe->setName(xmiClasse->getName());
	//   classe->setId(xmiClasse->getId());
	classe->setSummary(xmiClasse->getSummary());
	classe->setDescription(xmiClasse->getDescription());
	//classe->setTags(xmiClasse->getTags());

	_reloadAttributes(classe, xmiClasse);
	_reloadOperations(classe, xmiClasse);
}

void ModelLoader::_parseClasse(shared_ptr<Class> classe, shared_ptr<XMIClasse> xmiClasse)
{
	//   classe->setName(xmiClasse->getName());
	//   classe->setId(xmiClasse->getId());
	classe->setSummary(xmiClasse->getSummary());
	classe->setDescription(xmiClasse->getDescription());
	classe->setTags(xmiClasse->getTags());

	_addAttributes(classe, xmiClasse);
	_addOperations(classe, xmiClasse);

	_addEvents(classe, xmiClasse);

	vector<string> parents = xmiClasse->getParentsID();
	for (size_t i = 0; i < parents.size(); i++)
	{
		pair<string,string> g;
		g.first = parents[i];
		g.second = xmiClasse->getId();
		_generalisations.push_back(g);
	}
}


void ModelLoader::_addEvents(shared_ptr<Class> classe, shared_ptr<XMIClasse> xmiClasse)
{
	unsigned int nCall = 1;
	shared_ptr<XMICallEvent> xmiCall;
	bool foundCall = true;
	while (foundCall)
	{
		xmiCall = xmiClasse->getCallEvent(nCall++);
		if (xmiCall )
		{
			shared_ptr<Trigger> trigger( new Trigger(xmiCall->getName()  ));
			classe->addTriggers(trigger);
			shared_ptr<CallEvent> callevent(new CallEvent(xmiCall->getName()));
			trigger->setEvent(callevent);
			//      callevent->setName(xmiCall->getName());
			//      callevent->setId(xmiCall->getId());
			//      trigger->setId(xmiCall->getId());
			callevent->setSummary(xmiCall->getSummary());
			callevent->setDescription(xmiCall->getDescription());
			callevent->setTags(xmiCall->getTags());
			string opRef = xmiCall->getOperationID();

#if GCC_VERSION > 40100
			//>= gcc 4.1
			callevent->setOperation(_idOperations.at(opRef));
#else
			callevent->setOperation(MAP_AT(_idOperations,opRef));
#endif

			_idEvents[xmiCall->getId()] = trigger;
		}
		else
			foundCall = false;
	}

	unsigned int nSignal = 1;
	shared_ptr<XMISignalEvent> xmiSignal;
	bool foundSignal = true;
	while (foundSignal)
	{
		xmiSignal = xmiClasse->getSignalEvent(nSignal++);
		if (xmiSignal )
		{
			shared_ptr<Trigger> trigger (new Trigger(xmiSignal->getName()));
			classe->addTriggers(trigger);
			shared_ptr<SignalEvent> signalevent(new SignalEvent(xmiSignal->getName()));
			trigger->setEvent(signalevent);
			//      signalevent->setName(xmiSignal->getName());
			//      signalevent->setId(xmiSignal->getId());
			//      trigger->setId(xmiSignal->getId());
			signalevent->setSummary(xmiSignal->getSummary());
			signalevent->setDescription(xmiSignal->getDescription());
			signalevent->setTags(xmiSignal->getTags());
			string signalID = xmiSignal->getSignalID();
			map < string, shared_ptr<Signal> >::iterator iS = _signals.find(signalID);
			if (iS != _signals.end())
				signalevent->setSignalClass(iS->second);
			else cerr << "Signal not found ...(" << signalID << ")" << endl;
			_idEvents[xmiSignal->getId()] = trigger;
		}
		else
			foundSignal = false;
	}

	unsigned int nChange = 1;
	shared_ptr<XMIChangeEvent> xmiChange;
	bool foundChange = true;
	while (foundChange)
	{
		xmiChange = xmiClasse->getChangeEvent(nChange++);
		if (xmiChange)
		{
			shared_ptr<Trigger> trigger(new Trigger(xmiChange->getName()));
			classe->addTriggers(trigger);
			shared_ptr<ChangeEvent> changeevent (new ChangeEvent(xmiChange->getName()));
			trigger->setEvent(changeevent);
			//      changeevent->setName(xmiChange->getName());
			//      changeevent->setId(xmiChange->getId());
			//      trigger->setId(xmiChange->getId());
			changeevent->setSummary(xmiChange->getSummary());
			changeevent->setDescription(xmiChange->getDescription());
			changeevent->setTags(xmiChange->getTags());
			_idEvents[xmiChange->getId()] = trigger;
			string body = xmiChange->getBody();
			shared_ptr<Expression> ex(new Expression(body,MascaretApplication::getInstance()->getModel()->getBasicType("boolean") ));
			//ex->setLanguage("OCL"); ex->setBody(body);
			changeevent->setChangeExpression(ex);

		}
		else
			foundChange = false;
	}

	unsigned int nTime = 1;
	shared_ptr<XMITimeEvent> xmiTime;
	bool foundTime = true;
	while (foundTime)
	{
		xmiTime = xmiClasse->getTimeEvent(nTime++);
		if (xmiTime )
		{
			shared_ptr<Trigger> trigger(new Trigger(xmiTime->getName()));
			classe->addTriggers(trigger);
			shared_ptr<TimeEvent> timeevent( new TimeEvent(xmiTime->getName()));
			trigger->setEvent(timeevent);
			//      timeevent->setName(xmiTime->getName());
			//      timeevent->setId(xmiTime->getId());
			//      trigger->setId(xmiTime->getId());
			timeevent->setSummary(xmiTime->getSummary());
			timeevent->setDescription(xmiTime->getDescription());
			timeevent->setTags(xmiTime->getTags());
			_idEvents[xmiTime->getId()] = trigger;
			string body = xmiTime->getBody();
			shared_ptr<LiteralReal> value (new LiteralReal(xmiTime->getBody()));
			//shared_ptr<OpaqueExpression> ex(new OpaqueExpression());
			//ex->setLanguage("TIME"); ex->setBody(body);
			timeevent->setWhen(value);
		}
		else
			foundTime = false;
	}

}


void ModelLoader::_addAttributes(shared_ptr<Class> classe, shared_ptr<XMIClasse> xmiClasse)
{
	unsigned int nAttribute = 1;
	shared_ptr<XMIAttribute> xmiAttribute;
	bool foundAttribute = true;
	while (foundAttribute)
	{
		xmiAttribute = xmiClasse->getAttribute(nAttribute++);
		if (xmiAttribute != NULL)
		{
			//Property * attr = new Property();
			// Recherche du type
			string typeXMI = xmiAttribute->getTypeRef();
			shared_ptr<PrimitiveType> typeAttribute = getDataType(typeXMI);
			if (!typeAttribute)  // != (PrimitiveType*)NULL)
				//            attr->setType(typeAttribute);
				//         else
				cerr << "Erreur type : " << typeXMI << " pour " << xmiAttribute->getName() << endl;

			//         attr->setName(xmiAttribute->getName());
			//         attr->setId(xmiAttribute->getId());
			//         attr->setSummary(xmiAttribute->getSummary());
			//         attr->setDescription(xmiAttribute->getDescription());
			//         attr->setClassifier(classe);

			// Default value
			string defaultValue;
			shared_ptr<ValueSpecification> value;
			bool derived=false;
			string attributeName=xmiAttribute->getName();
			if (attributeName[0]=='/') { derived=true; attributeName=attributeName.substr(1);}
			if (xmiAttribute->hasInitialValue() )
			{
				defaultValue = xmiAttribute->getInitialValue();
				if (!derived)
				{
					value=typeAttribute->createValueFromString(defaultValue);
				}
				else
				{
					value=shared_ptr<ValueSpecification>(new Expression(defaultValue,typeAttribute));
				}
				//            if (typeAttribute->getName() == "real")
				//               value = shared_ptr<ValueSpecification> (new LiteralReal(defaultValue));
				//            else if (typeAttribute->getName() == "integer")
				//               value = shared_ptr<ValueSpecification> (new LiteralInteger(defaultValue));
				//            else if (typeAttribute->getName() == "string")
				//               value = shared_ptr<ValueSpecification> (new LiteralString(defaultValue));
				//	    else if (typeAttribute->getName() =="boolean")
				//               value = shared_ptr<ValueSpecification> (new LiteralBoolean(defaultValue));
			}
			shared_ptr<Property> attr(new Property(
					attributeName,
					classe.get(),
					typeAttribute,
					//opposite
					shared_ptr<Property>(),
					value            //default value
			));
			string multStr;
			//upper
			multStr= xmiAttribute->getMultiplicity();
			attr->setMaximumNumber(boost::lexical_cast<int>(multStr));
			//upper
			multStr= xmiAttribute->getMultiplicityLower();
			attr->setMinimumNumber(boost::lexical_cast<int>(multStr));
			//derived
			attr->setIsDerived(derived);
			attr->setSummary(xmiAttribute->getSummary());
			attr->setDescription(xmiAttribute->getDescription());
			attr->setTags(xmiAttribute->getTags());
			classe->addAttribute(attr);
		}
		else
			foundAttribute = false;
	}
}

void ModelLoader::_reloadAttributes(shared_ptr<Class> classe, shared_ptr<XMIClasse> xmiClasse)
{
	unsigned int nAttribute = 1;
	shared_ptr<XMIAttribute> xmiAttribute;
	bool foundAttribute = true;

	map<string, shared_ptr<Property> >  attributes = classe->getAttributes();

	while (foundAttribute)
	{
		xmiAttribute = xmiClasse->getAttribute(nAttribute++);
		if (xmiAttribute != NULL)
		{
			map<string, shared_ptr<Property> >::iterator itA = attributes.find(xmiAttribute->getName());
			if (itA == attributes.end())
			{
				// Recherche du type
				string typeXMI = xmiAttribute->getTypeRef();
				shared_ptr<PrimitiveType> typeAttribute = getDataType(typeXMI);
				if (!typeAttribute)
					cerr << "Erreur type : " << typeXMI << " pour " << xmiAttribute->getName() << endl;

				// Default value
				string defaultValue;
				shared_ptr<ValueSpecification> value;
				bool derived=false;
				string attributeName=xmiAttribute->getName();
				if (attributeName[0]=='/') { derived=true; attributeName=attributeName.substr(1);}
				if (xmiAttribute->hasInitialValue() )
				{
					defaultValue = xmiAttribute->getInitialValue();
					if (!derived)
					{
						value=typeAttribute->createValueFromString(defaultValue);
					}
					else
					{
						value=shared_ptr<ValueSpecification>(new Expression(defaultValue,typeAttribute));
					}
				}
				shared_ptr<Property> attr(new Property(
						attributeName,
						classe.get(),
						typeAttribute,
						shared_ptr<Property>(),
						value            //default value
				));
				string multStr;
				//upper
				multStr= xmiAttribute->getMultiplicity();
				attr->setMaximumNumber(boost::lexical_cast<int>(multStr));
				//upper
				multStr= xmiAttribute->getMultiplicityLower();
				attr->setMinimumNumber(boost::lexical_cast<int>(multStr));
				//derived
				attr->setIsDerived(derived);
				attr->setSummary(xmiAttribute->getSummary());
				attr->setDescription(xmiAttribute->getDescription());
				attr->setTags(xmiAttribute->getTags());
				classe->addAttribute(attr);
			}
		}
		else
			foundAttribute = false;
	}
}


void ModelLoader::_addOperations(shared_ptr<Class> classe, shared_ptr<XMIClasse> xmiClasse)
{
	unsigned int nOperation = 1;
	shared_ptr<XMIOperation> xmiOperation;
	xmiOperation = xmiClasse->getOperation(nOperation);
	while (xmiOperation != NULL)
	{

		// Pour l'instant que Opaque Behavior (C++ / Python)
		OpaqueBehavior* b;
		/* sauvegarde du contenu C++ si prsent */
		string data;
		//default C++
		string opClassName = classe->getName() + "_" + xmiOperation->getName();
		xmiOperation->getTaggedValue("C++",data);
		if (!data.empty())
		{
			if (data.find("{PYTHON}")==0)
			{
				data=data.substr(strlen("{PYTHON}"));
#ifndef NOPYTHON
				cerr<<"Create Python Opaque behavior"<<endl;
				cerr<<":::: "<<data<<"::::"<<endl;
				b=new OpaquePythonBehavior(opClassName); b->setBody(data);
#else
				cerr<<"Can't create Python Opaque behavior"<<endl;
				cerr<<"Python module not installed"<<endl;
				b=NULL;
#endif
			}
			else if (data.find("{OCL}")==0)
			{
				data=data.substr(strlen("{OCL}"));
				cerr<<"Create Ocl Query behavior"<<endl;
				cerr<<":::: "<<data<<"::::"<<endl;
				b=new OclQuery(opClassName); b->setBody(data);

			}
			else
			{
				_opaqueBehaviorsCode.push_back(make_pair(opClassName,data));
				b=new OpaqueBehavior(xmiOperation->getName()); b->setBody(xmiOperation->getName());
			}
		}
		else
		{
			b=new OpaqueBehavior(xmiOperation->getName());
			b->setBody(xmiOperation->getName());
		}
		shared_ptr<Operation> operation(new Operation(
				xmiOperation->getName(),
				classe,
				shared_ptr<OpaqueBehavior>(b)
		));
		//      operation->setName(xmiOperation->getName());
		//      operation->setId(xmiOperation->getId());
		operation->setSummary(xmiOperation->getSummary());
		operation->setDescription(xmiOperation->getDescription());
		operation->setTags(xmiOperation->getTags());
		_addParameters(operation, xmiOperation);
		_idOperations[xmiOperation->getId()]=operation;
		//TODO : normalement a devrait etre fait par le new Operation
		classe->addOperation(operation);

		nOperation++;
		xmiOperation = xmiClasse->getOperation(nOperation);
	}
}

void ModelLoader::_reloadOperations(shared_ptr<Class> classe, shared_ptr<XMIClasse> xmiClasse)
{
	unsigned int nOperation = 1;
	shared_ptr<XMIOperation> xmiOperation;
	xmiOperation = xmiClasse->getOperation(nOperation);

	map< string,shared_ptr<Operation> > operations = classe->getOperations();

	while (xmiOperation != NULL)
	{
		map<string, shared_ptr<Operation> >::iterator itO = operations.find(xmiOperation->getName());
		if (itO == operations.end())
		{
			cerr << "Cette operation n'existe pas : " << xmiOperation->getName() << endl;
			string opClassName = classe->getName() + "_" + xmiOperation->getName();
			OpaqueBehavior* b;
			b=new OpaqueBehavior(xmiOperation->getName());
			b->setBody(xmiOperation->getName());

			shared_ptr<Operation> operation(new Operation( xmiOperation->getName(), classe, shared_ptr<OpaqueBehavior>(b)));
			operation->setSummary(xmiOperation->getSummary());
			operation->setDescription(xmiOperation->getDescription());
			operation->setTags(xmiOperation->getTags());
			_reloadParameters(operation, xmiOperation);
			_idOperations[xmiOperation->getId()]=operation;
			classe->addOperation(operation);
			_reloadActivities(_getInitialPackage(), operation, xmiOperation->getId());
		}
		nOperation++;
		xmiOperation = xmiClasse->getOperation(nOperation);
	}
}


void ModelLoader::_addParameters(shared_ptr<Operation> operation, shared_ptr<XMIOperation> xmiOperation)
{
	unsigned int nParameter = 1;
	//   bool hasReturn = false;
	shared_ptr<XMIParameter> xmiParameter;
	xmiParameter = xmiOperation->getParameter(nParameter);
	while (xmiParameter != NULL)
	{
		shared_ptr<Parameter> parameter( new Parameter( xmiParameter->getName(), xmiParameter->getKind()  ));
		parameter->setDescription(xmiParameter->getDescription());
		parameter->setSummary(xmiParameter->getSummary());
		parameter->setTags(xmiParameter->getTags());
		_parameters[xmiParameter->getId()] = parameter;
		_operationType[xmiParameter->getId()] = operation;
		_paramToType[xmiParameter->getId()] = xmiParameter->getTypeRef();
		operation->addParameter(parameter);
		nParameter++;
		xmiParameter = xmiOperation->getParameter(nParameter);
	}
}

void ModelLoader::_reloadParameters(shared_ptr<Operation> operation, shared_ptr<XMIOperation> xmiOperation)
{
	unsigned int nParameter = 1;
	//   bool hasReturn = false;
	shared_ptr<XMIParameter> xmiParameter;
	xmiParameter = xmiOperation->getParameter(nParameter);
	while (xmiParameter != NULL)
	{
		shared_ptr<Parameter> parameter( new Parameter( xmiParameter->getName(), xmiParameter->getKind()  ));
		parameter->setDescription(xmiParameter->getDescription());
		parameter->setSummary(xmiParameter->getSummary());
		parameter->setTags(xmiParameter->getTags());
		_parameters[xmiParameter->getId()] = parameter;
		_operationType[xmiParameter->getId()] = operation;
		_paramToType[xmiParameter->getId()] = xmiParameter->getTypeRef();
		shared_ptr<Classifier> type=_classifiers[xmiParameter->getTypeRef()];
		parameter->setType(type);
		operation->addParameter(parameter);
		nParameter++;
		xmiParameter = xmiOperation->getParameter(nParameter);
	}
}


void ModelLoader::_setParametersType(void)
{
	map <string, string>::iterator it;
	for (it = _paramToType.begin(); it != _paramToType.end(); it++)
	{
#if GCC_VERSION > 40100
		shared_ptr<Parameter> parameter= _parameters.at(it->first);
		shared_ptr<Classifier> type=_classifiers.at(it->second);
#else
		shared_ptr<Parameter> parameter= MAP_AT(_parameters,it->first);
		shared_ptr<Classifier> type=MAP_AT(_classifiers,it->second);
#endif

		parameter->setType(type);
	}
}


void ModelLoader::_addActivities(shared_ptr<XMIPackage> package)
{
cerr << "ADD ACTIVITIES //////// " << endl;
	unsigned int nActivity = 1;
	shared_ptr<XMIActivityGraph> xmiActivity;
	bool found = true;
	while (found)
	{
		xmiActivity = package->getActivityGraph(nActivity++);
		if (xmiActivity != NULL)
		{
			string stereotypeRef = xmiActivity->getStereotypeRef();
			string stereotype = "Activity";
			if (stereotypeRef != "") stereotype =  _stereotypes[stereotypeRef];

			if (stereotype != "Procedure")
			{
				map < string, shared_ptr<Operation> >::iterator it = _idOperations.find(xmiActivity->getContextId());
				if (it != _idOperations.end())
				{
					shared_ptr<Activity> activity (new Activity(xmiActivity->getName()));

cerr << " PARSING ACTIVITY NAME : " << activity->getName() << endl;

					map<string, shared_ptr<ActivityNode> > activityNodes;
					map<string, shared_ptr<ActivityEdge> > activityEdges;

					/* Action Nodes */
					unsigned int nActionState = 1;
					shared_ptr<XMIActionState> xmiActionState;
					bool foundAS = true;
					while (foundAS)
					{
						xmiActionState = xmiActivity->getActionState(nActionState++);
						if (xmiActionState != NULL)
						{
							shared_ptr<ActionNode> actionNode (new ActionNode(xmiActionState->getName()));
							activityNodes[xmiActionState->getId()] = actionNode;

							if (xmiActionState->getCallOperationRef() !="")
							{
								map < string, shared_ptr<Operation> >::iterator it =  _idOperations.find(xmiActionState->getCallOperationRef());
								if (it != _idOperations.end() )
								{
									shared_ptr<CallOperationAction> action (new CallOperationAction());
									action->setOperation(_idOperations[xmiActionState->getCallOperationRef()]);
									action->setArguments(xmiActionState->getArguments());
									actionNode->setAction(action);
								}
								else cerr << "Erreur : Operation non trouvee ..." << endl;
							}
							if (xmiActionState->getSendSignalRef() !="")
							{
								map < string, shared_ptr<Signal> >::iterator it =  _signals.find(xmiActionState->getSendSignalRef());
								if (it != _signals.end() )
								{
									shared_ptr<SendSignalAction> action (new SendSignalAction());
									action->setSignalClass(_signals[xmiActionState->getSendSignalRef()]);
									actionNode->setAction(action);
								}
								else cerr << "Erreur Signal non trouve ..." << endl;
							}
							string toDo;
							if (xmiActionState->getTaggedValue("To Do (unused)",toDo))
							{
								string command = _parseToDo(toDo);
								string commandName=_parseCommandeName(command);
								if (commandName == "playAnimation")
								{
									string animName;
									string ressourceName;
									double speed;
									int sens;
									_parsePlayAnimationCommand (command, animName,ressourceName, speed, sens);
									shared_ptr<PlayAnimationAction> action (new PlayAnimationAction());
									action->setAnimationName(animName);
									actionNode->setAction(action);
								}
								else if (commandName == "setRed")
								{
									shared_ptr<SetInstanceInRedAction> action (new SetInstanceInRedAction());
									actionNode->setAction(action);
								}
								else if (commandName == "setTransparency")
								{
									shared_ptr<SetInstanceTransparencyAction> action (new SetInstanceTransparencyAction());
									actionNode->setAction(action);
								}
								else if (commandName == "growUp")
								{
									shared_ptr<GrowUpInstanceAction> action (new GrowUpInstanceAction());
									actionNode->setAction(action);
								}
								else if (commandName == "restoreAll")
								{
									shared_ptr<RestoreEnvironmentAction> action (new RestoreEnvironmentAction());
									actionNode->setAction(action);
								}
								else
								{
									cerr << "Pas d'action trouvé pour le noeud "<< actionNode->getName()<<endl;
								}
							}
							//activity->addNode(actionNode); //On le fait plus tard sur tous les noeuds
						}
						else foundAS = false;
					}

					/* PseudoState == ControlNode */
					unsigned int nPseudoState = 1;
					shared_ptr<XMIPseudoState> xmiPseudoState;
					bool foundPS = true;
					while (foundPS)
					{
						xmiPseudoState = xmiActivity->getPseudoState(nPseudoState++);
						if (xmiPseudoState != NULL)
						{
							string kind = xmiPseudoState->getKind(_xmiParser->getParser());
							if (kind == "initial")
							{
								shared_ptr<InitialNode> controlNode (new InitialNode());
								activityNodes[xmiPseudoState->getId()] = controlNode;
								activity->setInitialNode(controlNode);
							}
							else if (kind == "fork")
							{
								shared_ptr<ForkNode> controlNode (new ForkNode());
								activityNodes[xmiPseudoState->getId()] = controlNode;
							}
							else if (kind == "join")
							{
								shared_ptr<JoinNode> controlNode (new JoinNode());
								activityNodes[xmiPseudoState->getId()] = controlNode;
							}

						}
						else foundPS = false;
					}

					/* FinalState == ControlNode */
					unsigned int nFinalState = 1;
					shared_ptr<XMIPseudoState> xmiFinalState;
					bool foundFS = true;
					while (foundFS)
					{
						xmiFinalState = xmiActivity->getFinalState(nFinalState++);
						if (xmiFinalState != NULL)
						{
							shared_ptr<FinalNode> controlNode (new FinalNode());
							activityNodes[xmiFinalState->getId()] = controlNode;
						}
						else foundFS = false;
					}

					/* ObjectFlowState == ObjectNode */
					unsigned int nObject = 1;
					shared_ptr<XMIObjectFlowState> xmiObjectFlow;
					bool foundOF = true;
					while (foundOF)
					{
						xmiObjectFlow = xmiActivity->getObjectFlowState(nObject++);
						if (xmiObjectFlow != NULL)
						{
							shared_ptr<ObjectNode> objectNode (new ObjectNode(xmiObjectFlow->getName()));
							activityNodes[xmiObjectFlow->getId()] = objectNode;
						}
						else foundOF = false;
					}

					/* Transition == Edge */
					unsigned int nTransition = 1;
					shared_ptr<XMITransition> xmiTransition;
					bool foundT = true;
					while (foundT)
					{
						xmiTransition = xmiActivity->getTransition(nTransition++);
						if (xmiTransition != NULL)
						{
							string sourceid = xmiTransition->getSourceID();
							string targetid = xmiTransition->getTargetID();

							map <string, shared_ptr<ActivityNode> >::iterator itS = activityNodes.find(sourceid);
							map <string, shared_ptr<ActivityNode> >::iterator itT = activityNodes.find(targetid);

							if ( itS != activityNodes.end() && itT != activityNodes.end())
							{
								shared_ptr<ActivityNode> source = activityNodes[sourceid];
								shared_ptr<ActivityNode> target = activityNodes[targetid];
								shared_ptr<ActivityEdge> edge (new ActivityEdge(xmiTransition->getName()));
								source->addOutgoing(edge);
								target->addIncoming(edge);
								edge->setSource(source);
								edge->setTarget(target);
								activityEdges[xmiTransition->getId()] = edge;
							}
							else cerr << "Erreur Activite Node non trouve ..." << endl;
						}
						else foundT = false;
					}

					/* Partition */
					unsigned int nPartition = 1;
					shared_ptr<XMIPartition> xmiPartition;
					bool foundP = true;
					while (foundP)
					{
						xmiPartition = xmiActivity->getPartition(nPartition++);
						if (xmiPartition != NULL)
						{
							shared_ptr<ActivityPartition> partition (new ActivityPartition(xmiPartition->getName()));
							activity->addPartition(partition);

							unsigned int nContent = 1;
							bool foundC = true;
							while (foundC)
							{
								string contentRef = xmiPartition->getContent(nContent++);
								if (contentRef != "")
								{
									/* Recherche activtyNode */
									map < string , shared_ptr<ActivityNode> >::iterator itAN =activityNodes.find(contentRef);
									if (itAN != activityNodes.end())
									{
										shared_ptr<ActivityNode> node = activityNodes[contentRef];
										partition->addNode(node);
										node->addPartition(partition);
									}
									else
									{
										/* Recherche activtyEdge */
										map < string , shared_ptr<ActivityEdge> >::iterator itAE =activityEdges.find(contentRef);
										if (itAE != activityEdges.end())
										{
											shared_ptr<ActivityEdge> node = activityEdges[contentRef];
											partition->addEdge(node);
											node->addPartition(partition);
										}
									}
								}
								else foundC = false;
							}
						}
						else foundP = false;
					}
					for(map<string,shared_ptr<ActivityNode> >::iterator i=activityNodes.begin();i!=activityNodes.end();i++)
						activity->addNode(i->second);

					it->second->setMethod(activity);
				}
				else
					cerr << "Erreur : Operation non trouvee .... " << endl;
			}
			else // Procedures
			{
				map < string, shared_ptr<OrganisationalStructure> >::iterator it = _organisations.find(xmiActivity->getContextId());
				if (it != _organisations.end())
				{
					//cerr << "Trouve equipe : " << it->second->getName() << " pour la procedure " << xmiActivity->getName() << endl;
					shared_ptr<OrganisationalStructure> organisation = it->second;
					shared_ptr<Procedure> procedure = shared_ptr<Procedure>(new Procedure(xmiActivity->getName()));
					shared_ptr<Activity> activity (new Activity(xmiActivity->getName()));
					procedure->setActivity(activity);

					organisation->addProcedure(procedure);

					unsigned int nPartition = 1;
					shared_ptr<XMIPartition> xmiPartition;
					bool foundP = true;
					while (foundP)
					{
						xmiPartition = xmiActivity->getPartition(nPartition++);
						if (xmiPartition != NULL)
						{
							//cerr << "Role : " << xmiPartition->getName() << endl;

							unsigned int iContent = 1;
							bool foundC = true;
							string contentID;
							while (foundC)
							{
								contentID = xmiPartition->getContent(iContent++);
								if (contentID != "")
								{
									if (_roleClasses[contentID])
									{
										//               if (_roles[xmiPartition->getName()] == Role::nullRef())
										{
											shared_ptr<Role> role = shared_ptr<Role>(new Role(xmiPartition->getName()));
											role->setRoleClass(_roleClasses[contentID]);
											organisation->addRole(role);
											//cerr << organisation->getName() << " : " << role->getName() << " / " << role->getRoleClass()->getName() << endl;
										}
									}
								}
								else
									foundC = false;
							}
						}
						else foundP = false;
					}
					// Creation de l'activity pour la proc....
					map<string, shared_ptr<ActivityNode> > activityNodes;
					map<string, shared_ptr<ActivityEdge> > activityEdges;

					/* Action Nodes */
					unsigned int nActionState = 1;
					shared_ptr<XMIActionState> xmiActionState;
					bool foundAS = true;
					while (foundAS)
					{
						xmiActionState = xmiActivity->getActionState(nActionState++);
						if (xmiActionState != NULL)
						{
							shared_ptr<ActionNode> actionNode (new ActionNode(xmiActionState->getName()));
							activityNodes[xmiActionState->getId()] = actionNode;
							if (xmiActionState->getCallOperationRef() !="")
							{
								map < string, shared_ptr<Operation> >::iterator it =  _idOperations.find(xmiActionState->getCallOperationRef());
								if (it != _idOperations.end() )
								{
									shared_ptr<Operation> op=_idOperations[xmiActionState->getCallOperationRef()];
									string classifierName=op->getClassifier()->getName();
									map < string, shared_ptr<Class> >::iterator classesIt=_classes.begin();
									for(;classesIt != _classes.end();classesIt++)
									{
										if(classesIt->second->getName() == classifierName && classesIt->second != op->getClassifier())
										{
											map<string, shared_ptr<Operation> > operations=classesIt->second->getOperations();
											map<string, shared_ptr<Operation> >::const_iterator opIt=operations.begin();
											while(opIt != operations.end())
											{
												if(opIt->second->getName() == op->getName())
												{
													op=opIt->second;
													break;
												}
												opIt++;
											}
											break;
										}
									}
									shared_ptr<CallOperationAction> action (new CallOperationAction());
									action->setOperation(op);
									actionNode->setAction(action);
								}
								else cerr << "Erreur : Operation non trouvee ..." << endl;
							}
							if (xmiActionState->getSendSignalRef() !="")
							{
								map < string, shared_ptr<Signal> >::iterator it =  _signals.find(xmiActionState->getSendSignalRef());
								if (it != _signals.end() )
								{
									shared_ptr<SendSignalAction> action (new SendSignalAction());
									action->setSignalClass(_signals[xmiActionState->getSendSignalRef()]);
									actionNode->setAction(action);
								}
								else cerr << "Erreur Signal non trouve ..." << endl;
							}
							string toDo;
							if (xmiActionState->getTaggedValue("To Do (unused)",toDo))
							{
								string command = _parseToDo(toDo);
								string commandName=_parseCommandeName(command);
								if (commandName == "playAnimation")
								{
									string animName;
									string ressourceName;
									double speed;
									int sens;
									_parsePlayAnimationCommand (command, animName,ressourceName, speed, sens);
									shared_ptr<PlayAnimationAction> action (new PlayAnimationAction());
									action->setAnimationName(animName);
									actionNode->setAction(action);
								}
								else if (commandName == "setRed")
								{
									shared_ptr<SetInstanceInRedAction> action (new SetInstanceInRedAction());
									actionNode->setAction(action);
								}
								else if (commandName == "setTransparency")
								{
									shared_ptr<SetInstanceTransparencyAction> action (new SetInstanceTransparencyAction());
									actionNode->setAction(action);
								}
								else if (commandName == "growUp")
								{
									shared_ptr<GrowUpInstanceAction> action (new GrowUpInstanceAction());
									actionNode->setAction(action);
								}
								else if (command == "restoreAll")
								{
									shared_ptr<RestoreEnvironmentAction> action (new RestoreEnvironmentAction());
									actionNode->setAction(action);
								}
								else
								{
									cerr << "Pas d'action trouvé pour le noeud "<< actionNode->getName()<<endl;
								}
							}
							//activity->addNode(actionNode); // Je le fais plus tard sur tous les neouds
						}
						else foundAS = false;
					}

					/* PseudoState == ControlNode */
					unsigned int nPseudoState = 1;
					shared_ptr<XMIPseudoState> xmiPseudoState;
					bool foundPS = true;
					while (foundPS)
					{
						xmiPseudoState = xmiActivity->getPseudoState(nPseudoState++);
						if (xmiPseudoState != NULL)
						{
							string kind = xmiPseudoState->getKind(_xmiParser->getParser());
							if (kind == "initial")
							{
								shared_ptr<InitialNode> controlNode (new InitialNode());
								activityNodes[xmiPseudoState->getId()] = controlNode;
								activity->setInitialNode(controlNode);
								cerr << " Activity : " << activity->getName() << " addControl Node" << endl;
							}
							else if (kind == "fork")
							{
								shared_ptr<ForkNode> controlNode (new ForkNode());
								activityNodes[xmiPseudoState->getId()] = controlNode;
							}
							else if (kind == "join")
							{
								shared_ptr<JoinNode> controlNode (new JoinNode());
								activityNodes[xmiPseudoState->getId()] = controlNode;
							}
							else if (kind == "choice")
							{
								shared_ptr<ChoiceNode> controlNode (new ChoiceNode());
								activityNodes[xmiPseudoState->getId()] = controlNode;
							}
							else if (xmiPseudoState->getName() == "Final")
							{
								cerr << " Final ...... " << endl;
								shared_ptr<FinalNode> controlNode (new FinalNode());
								activityNodes[xmiPseudoState->getId()] = controlNode;
							}

						}
						else foundPS = false;
					}

					/* FinalState == ControlNode */
					unsigned int nFinalState = 1;
					shared_ptr<XMIPseudoState> xmiFinalState;
					bool foundFS = true;
					while (foundFS)
					{
						xmiFinalState = xmiActivity->getFinalState(nFinalState++);
						if (xmiFinalState != NULL)
						{
							shared_ptr<FinalNode> controlNode (new FinalNode());
							activityNodes[xmiFinalState->getId()] = controlNode;
						}
						else foundFS = false;
					}

					/* ObjectFlowState == ObjectNode */
					unsigned int nObject = 1;
					shared_ptr<XMIObjectFlowState> xmiObjectFlow;
					bool foundOF = true;
					while (foundOF)
					{
						xmiObjectFlow = xmiActivity->getObjectFlowState(nObject++);
						if (xmiObjectFlow != NULL)
						{
							shared_ptr<ObjectNode> objectNode (new ObjectNode(xmiObjectFlow->getName()));
							shared_ptr<Ressource> ressource ( new Ressource(objectNode->getName()));
							organisation->addRessource(ressource);
							procedure->addRessource(ressource);
							ressource->setEntityClass(shared_dynamic_cast<EntityClass>(_classes[xmiObjectFlow->getType()]));
							activityNodes[xmiObjectFlow->getId()] = objectNode;
						}
						else foundOF = false;
					}

					/* Transition == Edge */
					unsigned int nTransition = 1;
					shared_ptr<XMITransition> xmiTransition;
					bool foundT = true;
					while (foundT)
					{
						xmiTransition = xmiActivity->getTransition(nTransition++);
						if (xmiTransition != NULL)
						{
							string sourceid = xmiTransition->getSourceID();
							string targetid = xmiTransition->getTargetID();

							map <string, shared_ptr<ActivityNode> >::iterator itS = activityNodes.find(sourceid);
							map <string, shared_ptr<ActivityNode> >::iterator itT = activityNodes.find(targetid);

							if ( itS != activityNodes.end() && itT != activityNodes.end())
							{
								shared_ptr<ActivityNode> source = activityNodes[sourceid];
								shared_ptr<ActivityNode> target = activityNodes[targetid];
								shared_ptr<ActivityEdge> edge (new ActivityEdge(xmiTransition->getName()));
								source->addOutgoing(edge);
								target->addIncoming(edge);
								edge->setSource(source);
								edge->setTarget(target);
								// guard
								string guardEx = xmiTransition->getGuardExpression();
								if (guardEx != "")
								{
									cerr << guardEx << endl;
									shared_ptr<Constraint> guard (new Constraint());
									shared_ptr<Expression> exp (new Expression(guardEx,_model->getBasicType("boolean")));
									guard->setSpecification(exp);
									edge->setGuard(guard);
								}
								activityEdges[xmiTransition->getId()] = edge;
								activity->addEdge(edge);
							}
							else
							{
								/*
                        cerr << "Procedure : Erreur Activite Node non trouve ..." << endl;
                        if (itS == activityNodes.end()) cerr << "Node : " << sourceid << endl;
                        if (itT == activityNodes.end()) cerr << "Node : " << targetid << endl;
								 */
							}
						}
						else foundT = false;
					}

					/* Partition */
					unsigned int naPartition = 1;
					shared_ptr<XMIPartition> xmiaPartition;
					bool foundPa = true;
					while (foundPa)
					{
						xmiaPartition = xmiActivity->getPartition(naPartition++);
						if (xmiaPartition != NULL)
						{
							shared_ptr<ActivityPartition> partition (new ActivityPartition(xmiaPartition->getName()));
							activity->addPartition(partition);
							partition->setRole(organisation->getRole(xmiaPartition->getName()));
							unsigned int nContent = 1;
							bool foundC = true;
							while (foundC)
							{
								string contentRef = xmiaPartition->getContent(nContent++);
								if (contentRef != "")
								{
									/* Recherche activtyNode */
									map < string , shared_ptr<ActivityNode> >::iterator itAN =activityNodes.find(contentRef);
									if (itAN != activityNodes.end())
									{
										shared_ptr<ActivityNode> node = activityNodes[contentRef];
										partition->addNode(node);
										node->addPartition(partition);
									}
									else
									{
										/* Recherche activtyEdge */
										map < string , shared_ptr<ActivityEdge> >::iterator itAE =activityEdges.find(contentRef);
										if (itAE != activityEdges.end())
										{
											shared_ptr<ActivityEdge> node = activityEdges[contentRef];
											partition->addEdge(node);
											node->addPartition(partition);
										}
									}
								}
								else foundC = false;
							}
						}
						else foundPa = false;
					}
					map < string , shared_ptr<ActivityNode> >::iterator it=activityNodes.begin();
					while(it != activityNodes.end())
					{
						activity->addNode(it->second);
						it++;
					}

					// Fin creation de l'activity....
				}


			}
		}
		else found = false;
	}
}

void ModelLoader::_reloadActivities(shared_ptr<XMIPackage> package, shared_ptr<Operation> operation, string id)
{
	unsigned int nActivity = 1;
	shared_ptr<XMIActivityGraph> xmiActivity;
	bool found = true;
	while (found)
	{
		xmiActivity = package->getActivityGraph(nActivity++);
		if (xmiActivity != NULL)
		{
			string stereotypeRef = xmiActivity->getStereotypeRef();
			string stereotype = "Activity";
			if (stereotypeRef != "") stereotype =  _stereotypes[stereotypeRef];

			// map < string, shared_ptr<Operation> >::iterator it = _idOperations.find(xmiActivity->getContextId());
			if (id == xmiActivity->getContextId())
			{

				cerr << " Trouve Activity Graph..." << endl;
				shared_ptr<Activity> activity (new Activity(xmiActivity->getName()));

				map<string, shared_ptr<ActivityNode> > activityNodes;
				map<string, shared_ptr<ActivityEdge> > activityEdges;

				/* Action Nodes */
				unsigned int nActionState = 1;
				shared_ptr<XMIActionState> xmiActionState;
				bool foundAS = true;
				while (foundAS)
				{
					xmiActionState = xmiActivity->getActionState(nActionState++);
					if (xmiActionState != NULL)
					{
						shared_ptr<ActionNode> actionNode (new ActionNode(xmiActionState->getName()));
						activityNodes[xmiActionState->getId()] = actionNode;

						if (xmiActionState->getCallOperationRef() !="")
						{
							map < string, shared_ptr<Operation> >::iterator it =  _idOperations.find(xmiActionState->getCallOperationRef());
							if (it != _idOperations.end() )
							{
								shared_ptr<CallOperationAction> action (new CallOperationAction());
								action->setOperation(_idOperations[xmiActionState->getCallOperationRef()]);
								action->setArguments(xmiActionState->getArguments());
								actionNode->setAction(action);
							}
							else cerr << "Erreur : Operation non trouvee ..." << endl;
						}
						if (xmiActionState->getSendSignalRef() !="")
						{
							map < string, shared_ptr<Signal> >::iterator it =  _signals.find(xmiActionState->getSendSignalRef());
							if (it != _signals.end() )
							{
								shared_ptr<SendSignalAction> action (new SendSignalAction());
								action->setSignalClass(_signals[xmiActionState->getSendSignalRef()]);
								actionNode->setAction(action);
							}
							else cerr << "Erreur Signal non trouve ..." << endl;
						}
						string toDo;
						if (xmiActionState->getTaggedValue("To Do (unused)",toDo))
						{
							string command = _parseToDo(toDo);
							string commandName = _parseCommandeName(toDo);
							if (commandName == "playAnimation")
							{
								string animName;
								string ressourceName;
								double speed;
								int sens;
								_parsePlayAnimationCommand (toDo, animName,ressourceName, speed, sens);
								shared_ptr<PlayAnimationAction> action (new PlayAnimationAction());
								action->setAnimationName(animName);
								actionNode->setAction(action);
							}
						}
						activity->addNode(actionNode);
					}
					else foundAS = false;
				}

				/* PseudoState == ControlNode */
				unsigned int nPseudoState = 1;
				shared_ptr<XMIPseudoState> xmiPseudoState;
				bool foundPS = true;
				while (foundPS)
				{
					xmiPseudoState = xmiActivity->getPseudoState(nPseudoState++);
					if (xmiPseudoState != NULL)
					{
						string kind = xmiPseudoState->getKind(_xmiParser->getParser());
						if (kind == "initial")
						{
							shared_ptr<InitialNode> controlNode (new InitialNode());
							activityNodes[xmiPseudoState->getId()] = controlNode;
							activity->setInitialNode(controlNode);
						}
						else if (kind == "fork")
						{
							shared_ptr<ForkNode> controlNode (new ForkNode());
							activityNodes[xmiPseudoState->getId()] = controlNode;
						}
						else if (kind == "join")
						{
							shared_ptr<JoinNode> controlNode (new JoinNode());
							activityNodes[xmiPseudoState->getId()] = controlNode;
						}

					}
					else foundPS = false;
				}

				/* FinalState == ControlNode */
				unsigned int nFinalState = 1;
				shared_ptr<XMIPseudoState> xmiFinalState;
				bool foundFS = true;
				while (foundFS)
				{
					xmiFinalState = xmiActivity->getFinalState(nFinalState++);
					if (xmiFinalState != NULL)
					{
						shared_ptr<FinalNode> controlNode (new FinalNode());
						activityNodes[xmiFinalState->getId()] = controlNode;
					}
					else foundFS = false;
				}

				/* ObjectFlowState == ObjectNode */
				unsigned int nObject = 1;
				shared_ptr<XMIObjectFlowState> xmiObjectFlow;
				bool foundOF = true;
				while (foundOF)
				{
					xmiObjectFlow = xmiActivity->getObjectFlowState(nObject++);
					if (xmiObjectFlow != NULL)
					{
						shared_ptr<ObjectNode> objectNode (new ObjectNode(xmiObjectFlow->getName()));
						activityNodes[xmiObjectFlow->getId()] = objectNode;
					}
					else foundOF = false;
				}

				/* Transition == Edge */
				unsigned int nTransition = 1;
				shared_ptr<XMITransition> xmiTransition;
				bool foundT = true;
				while (foundT)
				{
					xmiTransition = xmiActivity->getTransition(nTransition++);
					if (xmiTransition != NULL)
					{
						string sourceid = xmiTransition->getSourceID();
						string targetid = xmiTransition->getTargetID();

						map <string, shared_ptr<ActivityNode> >::iterator itS = activityNodes.find(sourceid);
						map <string, shared_ptr<ActivityNode> >::iterator itT = activityNodes.find(targetid);

						if ( itS != activityNodes.end() && itT != activityNodes.end())
						{
							shared_ptr<ActivityNode> source = activityNodes[sourceid];
							shared_ptr<ActivityNode> target = activityNodes[targetid];
							shared_ptr<ActivityEdge> edge (new ActivityEdge(xmiTransition->getName()));
							source->addOutgoing(edge);
							target->addIncoming(edge);
							edge->setSource(source);
							edge->setTarget(target);
							activityEdges[xmiTransition->getId()] = edge;
						}
						else cerr << "Erreur Activite Node non trouve ..." << endl;
					}
					else foundT = false;
				}

				/* Partition */
				unsigned int nPartition = 1;
				shared_ptr<XMIPartition> xmiPartition;
				bool foundP = true;
				while (foundP)
				{
					xmiPartition = xmiActivity->getPartition(nPartition++);
					if (xmiPartition != NULL)
					{
						shared_ptr<ActivityPartition> partition (new ActivityPartition(xmiPartition->getName()));
						activity->addPartition(partition);

						unsigned int nContent = 1;
						bool foundC = true;
						while (foundC)
						{
							string contentRef = xmiPartition->getContent(nContent++);
							if (contentRef != "")
							{
								/* Recherche activtyNode */
								map < string , shared_ptr<ActivityNode> >::iterator itAN =activityNodes.find(contentRef);
								if (itAN != activityNodes.end())
								{
									shared_ptr<ActivityNode> node = activityNodes[contentRef];
									partition->addNode(node);
									node->addPartition(partition);
								}
								else
								{
									/* Recherche activtyEdge */
									map < string , shared_ptr<ActivityEdge> >::iterator itAE =activityEdges.find(contentRef);
									if (itAE != activityEdges.end())
									{
										shared_ptr<ActivityEdge> node = activityEdges[contentRef];
										partition->addEdge(node);
										node->addPartition(partition);
									}
								}
							}
							else foundC = false;
						}
					}
					else foundP = false;
				}

				operation->setMethod(activity);
			}
		}
		else found = false;
	}
}

void ModelLoader::_addStateMachines(shared_ptr<XMIPackage> package)
{
	unsigned int nStateMachine = 1;
	shared_ptr<XMIStateMachine> xmiMachine;
	bool found = true;
	while (found)
	{
		xmiMachine = package->getStateMachine(nStateMachine++);
		if (xmiMachine != NULL)
		{
			shared_ptr<StateMachine> machine (new StateMachine(xmiMachine->getName()));
			//         machine->setName(xmiMachine->getName());
			//         machine->setId(xmiMachine->getId());
			machine->setSummary(xmiMachine->getSummary());
			machine->setDescription(xmiMachine->getDescription());
			machine->setTags(xmiMachine->getTags());

			shared_ptr<Class> classe = _classes[xmiMachine->getContextClassifierID()];
			//TODO aurait du etre fait dans la creation
			classe->addOwnedBehavior(machine);
			machine->setOwner(classe);
			//         map<string,shared_ptr<Trigger> >& triggers = classe->getTriggers();
			//         map<string,shared_ptr<Operation> > operations = classe->getOperations();

			shared_ptr<Region> region(new Region());
			machine->addRegion(region);

			map<string, shared_ptr<Vertex> > vertices;

			// ATTENTION : A faire gestion du finalstate
			bool fPS = true;
			int nPS = 1;
			shared_ptr<XMIPseudoState> xmips;
			while (fPS)
			{
				xmips = xmiMachine->getPseudoState(nPS++);
				if (xmips != NULL)
				{
					shared_ptr<PseudoState> ps(new PseudoState(xmips->getName()));
					//ps->setName(xmips->getName());
					//               ps->setId(xmips->getId());
					ps->setSummary(xmips->getSummary());
					ps->setDescription(xmips->getDescription());
					ps->setTags(xmips->getTags());
					region->addVertices(ps);
					machine->addConnectionPoint(ps);
					vertices[xmips->getId()] = ps;

					if (xmips->getKind(_xmiParser->getParser()) == "initial")
						ps->setKind(INITIAL);
					if (xmips->getKind(_xmiParser->getParser()) == "join")
						ps->setKind(JOIN);
					if (xmips->getKind(_xmiParser->getParser()) == "fork")
						ps->setKind(FORK);
				}
				else fPS = false;
			}

			bool fS = true;
			int nS = 1;
			shared_ptr<XMISimpleState> xmis;
			while (fS)
			{
				xmis = xmiMachine->getSimpleState(nS++);
				if (xmis != NULL)
				{
					shared_ptr<State> s(new State(xmis->getName()));
					//               s->setId(xmis->getId());
					s->setSummary(xmis->getSummary());
					s->setDescription(xmis->getDescription());
					s->setTags(xmis->getTags());
					string stereotypeRef = xmis->getStereotypeRef();
					string stereotype;
					if (stereotypeRef != "") stereotype =  _stereotypes[stereotypeRef];
					if (stereotype == "NonInterupt") s->setNonInterupt(true);

					string doAct = xmis->getDoActivityOperationRef();
					string entryAct = xmis->getEntryActivityOperationRef();
					string exitAct = xmis->getExitActivityOperationRef();

					//               bool foundDo = false;
					//               bool foundEntry = false;
					//               bool foundExit = false;
					if (doAct!="")
					{
						try
						{
#if GCC_VERSION > 40100
							shared_ptr<Operation> op=_idOperations.at(doAct);
#else
							shared_ptr<Operation> op=MAP_AT(_idOperations,doAct);
#endif
							s->setDoBehavior(op);

						}catch (std::out_of_range&)
						{
							cerr << " Error : do behavior not found" << endl;
						}
					}
					if (entryAct!="")
					{
						try
						{
#if GCC_VERSION > 40100
							shared_ptr<Operation> op=_idOperations.at(entryAct);
#else
							shared_ptr<Operation> op=MAP_AT(_idOperations,entryAct);
#endif
							s->setEntryBehavior(op);

						}catch (std::out_of_range&)
						{
							cerr << " Error : entry behavior not found" << endl;
						}
					}
					if (exitAct!="")
					{
						try
						{
#if GCC_VERSION > 40100
							shared_ptr<Operation> op=_idOperations.at(exitAct);
#else
							shared_ptr<Operation> op=MAP_AT(_idOperations,exitAct);
#endif
							s->setExitBehavior(op);

						}catch (std::out_of_range&)
						{
							cerr << " Error : exit behavior not found" << endl;
						}
					}
					/*
                                   for (unsigned int iOper = 0; iOper < operations.size(); iOper++)
                                   {
                                      if (doAct != "" && idOperations.[iOper]->getId() == doAct)
                                      {
                                         s->setDoBehavior(operations[iOper]);
                                         foundDo = true;
                                      }
                                      if (entryAct != "" && operations[iOper]->getId() == entryAct)
                                      {
                                         s->setEntryBehavior(operations[iOper]);
                                         foundEntry = true;
                                      }
                                      if (exitAct != "" && operations[iOper]->getId() == exitAct)
                                      {
                                         s->setExitBehavior(operations[iOper]);
                                         foundExit = true;
                                      }
                                   }
                                   if (doAct!="" && !foundDo) cerr << " Error : do behavior not found" << endl;
                                   if (entryAct!="" && !foundEntry) cerr << " Error : entry behavior not found" << endl;
                                   if (exitAct!="" && !foundExit) cerr << " Error : exit behavior not found" << endl;
					 */
					region->addVertices(s);
					vertices[xmis->getId()] = s;
				}
				else fS = false;
			}

			bool foundT = true;
			int nT = 1;
			shared_ptr<XMITransition> xmitransition;
			while (foundT)
			{
				xmitransition = xmiMachine->getTransition(nT++);
				if (xmitransition != NULL)
				{
					//xmitransition->getName() ));
					shared_ptr<Transition> t (new Transition());
					//               t->setId(xmitransition->getId());
					t->setSummary(xmitransition->getSummary());
					t->setDescription(xmitransition->getDescription());
					t->setTags(xmitransition->getTags());
					region->addTransition(t);

					// Trigger
					string tid = xmitransition->getTriggerID();
					if (tid != "")
					{
						try
						{

#if GCC_VERSION > 40100
							t->addTrigger(_idEvents.at(tid));
#else
							t->addTrigger(MAP_AT(_idEvents,tid));
#endif
						}        //idEvents contient map id -> Trigger
						catch (std::out_of_range&) {cerr << "Erreur : trigger not found" << endl; }
						/*
                                          bool foundTrigger = false;
                                          unsigned int iTrigger = 0;
                                          while (!foundTrigger && iTrigger< triggers.size())
                                          {
                                             if (triggers[iTrigger]->getId() == tid)
                                             {
                                                foundTrigger = true;
                                                t->addTrigger(triggers[iTrigger]);
                                             }
                                             iTrigger ++;
                                          }
                                          if (!foundTrigger) cerr << "Erreur : trigger not found" << endl;*/
					}

					// guard
					string guardEx = xmitransition->getGuardExpression();
					if (guardEx != "")
					{
						shared_ptr<Constraint> guard (new Constraint());
						shared_ptr<OpaqueExpression> exp (new OpaqueExpression());
						guard->setSpecification(exp);
						exp->setBody(guardEx);
						exp->setLanguage("OCL");
						t->setGuard(guard);
					}

					// source
					string sourceID = xmitransition->getSourceID();
					map<string, shared_ptr<Vertex> >::iterator itV = vertices.find(sourceID);
					if (itV == vertices.end())
						cerr << "Vertex not found ...("<<sourceID<<")" << endl;
					else
					{
						shared_ptr<Vertex> vertex = itV->second;
						//TODO en une fois?
						t->setSource(vertex);
						vertex->addOutgoing(t);
					}
					// target
					string targetID = xmitransition->getTargetID();
					itV = vertices.find(targetID);
					if (itV == vertices.end())
						cerr << "Vertex not found ...("<<targetID<<")" << endl;
					else
					{
						shared_ptr<Vertex> vertex = itV->second;
						t->setTarget(vertex);
						vertex->addIncoming(t);
					}
				}
				else foundT = false;
			}

		}
		else found = false;
	}
}

string ModelLoader::_parseToDo(const string& toDo)
{
	size_t posEndCommand = toDo.find(';');
	if(posEndCommand == string::npos)
		return toDo;
	string command(toDo,0,posEndCommand+1);
	//command = strNoLeadingSpace(command);
	return command;

}
string ModelLoader::_parseCommandeName(const string& command)
{
	string commandName = _readValue(command, 0);
	return commandName;
}
void ModelLoader::_parsePlayAnimationCommand (const string& command, string & animName, string &ressourceName, double &speed, int &sens)
{
	string speeds, senss;

	animName = _readValue(command, 1);

	speed = 1;
	speeds = _readValue(command, 2);
	if (speeds != "")
		speed = atof(speeds.c_str());

	sens = 1;
	senss = _readValue(command, 3);
	if (senss != "")
		sens = atoi(senss.c_str());

	ressourceName = _readValue(command, 4);

}


string ModelLoader::_readValue (const string & command, int index)
{
	int nb = 0;
	string toDo = command;
	string res;
	res  = command;
	int posEndValue = 1;

	while ( (posEndValue >=0) && (nb <= index) )
	{
		posEndValue = toDo.find(' ');
		string tvalue(toDo,0,posEndValue);
		res = tvalue;
		toDo = toDo.substr(posEndValue+1,toDo.size()-1);
		nb ++;
	}
	if (index == nb-1) {
		int size = res.length();
		if (res[size-1] == ';')
		{
			res = res.substr(0, size-1);
		}
		return res;
	}
	else return "";
}



void ModelLoader::_addAssociations(shared_ptr<XMIPackage> package)
{
	unsigned int nRelation = 1;
	shared_ptr<XMIRelation> xmiRelation;
	bool found = true;
	while (found)
	{
		xmiRelation = package->getRelation(nRelation++);
		if (xmiRelation != NULL)
		{
			shared_ptr<Package> p;
			map<string, shared_ptr<Package> >::iterator it = _packages.find(package->getId());
			if(it!=_packages.end())
			{
				p = it->second;
				//            Association * association = new Association();
				//            association->setName(xmiRelation->getName());
				//            association->setId(xmiRelation->getId());
				//            association->setDescription(xmiRelation->getDescription());
				//            association->setSummary(xmiRelation->getSummary());
				//            p->addAssociations(association);

				shared_ptr<XMILinkEnd> l1 = xmiRelation->getLinkEnd(1);
				shared_ptr<XMILinkEnd> l2 = xmiRelation->getLinkEnd(2);
				if (l1 != NULL && l2 != NULL)
				{
					shared_ptr<Class> cl1;
					shared_ptr<Class> cl2;
					map<string, shared_ptr<Class> >::iterator itC;
					itC = _classes.find(l1->getParticipantId());
					if (itC != _classes.end()) cl1 = itC->second;
					else cerr << "Type not defined..." << endl;
					itC = _classes.find(l2->getParticipantId());
					if (itC != _classes.end()) cl2 = itC->second;
					else cerr << "Type not defined..." << endl;

					shared_ptr<Property> property1,property2;

					if (l1->isNavigable())
					{
						property1=shared_ptr<Property> ( new Property(
								l1->getName(),cl2.get(),cl1));
						property1->setDescription(l1->getDescription());
						property1->setSummary(l1->getSummary());
						property1->setTags(l1->getTags());

						string multStr;
						//upper
						multStr= l1->getMultiplicity();
						property1->setMaximumNumber(boost::lexical_cast<int>(multStr));
						//upper
						multStr= l1->getMultiplicityLower();
						property1->setMinimumNumber(boost::lexical_cast<int>(multStr));

						cl2->addAttribute(property1);
					}
					if (l2->isNavigable())
					{
						property2=shared_ptr<Property> ( new Property(
								l2->getName(),cl1.get(),cl2));
						property2->setDescription(l2->getDescription());
						property2->setSummary(l2->getSummary());
						property2->setTags(l2->getTags());
						property2->setOpposite(property1);
						string multStr;
						//upper
						multStr= l2->getMultiplicity();
						property2->setMaximumNumber(boost::lexical_cast<int>(multStr));
						//upper
						multStr= l2->getMultiplicityLower();
						property2->setMinimumNumber(boost::lexical_cast<int>(multStr));

						cl1->addAttribute(property2);
					}
					if (property1)
						property1->setOpposite(property2);
					/*               property1->setName(l1->getName());
                                   property1->setId(l1->getId());
                                   property1->setDescription(l1->getDescription());
                                   property1->setSummary(l1->getSummary());
                                   property1->setType(cl1);
                                   if (l1->isNavigable())
                                   {
                                      cl2->addAssociations(property1);
                                      property1->setClassifier(cl2);
                                   }

                                   Property * property2 = new Property();
                                   property2->setName(l2->getName());
                                   property2->setId(l2->getId());
                                   property2->setDescription(l2->getDescription());
                                   property2->setSummary(l2->getSummary());
                                   property2->setType(cl2);
                                   if (l2->isNavigable())
                                   {
                                      cl1->addAssociations(property2);
                                      property2->setClassifier(cl1);
                                   }

                                   property1->setOpposite(property2);
                                   property2->setOpposite(property1);

                                   association->addMemberEnd(property1);
                                   association->addMemberEnd(property2);
					 */
				}
			}
			else cerr << "Erreur interne : package" << endl;
		}
		else found = false;
	}

	unsigned int nPackage = 1;
	shared_ptr<XMIPackage> xmiPackage;
	bool found2 = true;
	while (found2)
	{
		xmiPackage = package->getPackage(nPackage++);
		if (xmiPackage != NULL)
			_addAssociations(xmiPackage);
		else
			found2 = false;
	}

}
