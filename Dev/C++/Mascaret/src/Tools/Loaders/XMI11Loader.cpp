/*
 * XMI11Loader.cpp
 *
 *  Created on: 18 janv. 2011
 *      Author: soler
 */

#include "Tools/Loaders/XMI11Loader.h"
#include "Tools/Loaders/XmlParser.h"

#include "VEHA/Kernel/Model.h"
#include "VEHA/Kernel/Package.h"
#include "VEHA/Kernel/Class.h"
#include "VEHA/Kernel/PrimitiveType.h"
#include "VEHA/Kernel/Operation.h"
#include "VEHA/Kernel/Parameter.h"
#include "VEHA/Kernel/Property.h"

#include "VEHA/Behavior/StateMachine/StateMachine.h"
#include "VEHA/Behavior/StateMachine/State.h"
#include "VEHA/Behavior/StateMachine/PseudoState.h"
#include "VEHA/Behavior/StateMachine/Transition.h"
#include "VEHA/Behavior/StateMachine/Region.h"
#include "VEHA/Behavior/StateMachine/Guard.h"
#include "VEHA/Behavior/StateMachine/SignalEvent.h"
#include "VEHA/Behavior/StateMachine/TimeEvent.h"

#include "VEHA/Behavior/Common/Signal.h"
#include "VEHA/Behavior/Common/OpaqueBehavior.h"
#include "VEHA/Behavior/Common/OclQuery.h"

#include "VEHA/Behavior/Activity/Activity.h"
#include "VEHA/Behavior/Activity/ActivityNode.h"
#include "VEHA/Behavior/Activity/ActionNode.h"
#include "VEHA/Behavior/Activity/ObjectNode.h"
#include "VEHA/Behavior/Activity/ControlNode.h"
#include "VEHA/Behavior/Activity/InitialNode.h"
#include "VEHA/Behavior/Activity/FinalNode.h"
#include "VEHA/Behavior/Activity/FlowFinalNode.h"
#include "VEHA/Behavior/Activity/ForkNode.h"
#include "VEHA/Behavior/Activity/JoinNode.h"
#include "VEHA/Behavior/Activity/DecisionNode.h"
#include "VEHA/Behavior/Activity/MergeNode.h"
#include "VEHA/Behavior/Activity/ChoiceNode.h"
#include "VEHA/Behavior/Activity/ActivityEdge.h"
#include "VEHA/Behavior/Activity/ObjectFlow.h"
#include "VEHA/Behavior/Activity/ControlFlow.h"

#include "VEHA/Behavior/BasicActions/Action.h"

#include "VEHA/Entity/EntityClass.h"

#include "HAVE/VirtualHumanClass.h"
#include "BEHAVE/RoleClass.h"

using namespace VEHA;
using namespace BEHAVE;
using namespace HAVE;

#define XMICONTENT                  "XMI.content"
#define XMIMODELELEMENTNAME         "UML:ModelElement.name"
#define XMIOWNEDELEMENT             "UML:Namespace.ownedElement"
#define XMIID                       "xmi.id"
#define XMIIDREF                    "xmi.idref"
#define XMIVALUE                    "xmi.value"
#define XMIFEATURE                  "UML:Classifier.feature"
#define XMISTRUCTURALTYPE           "UML:StructuralFeature.type"

#define XMIINITIALVALUE             "UML:Attribute.initialValue"
#define XMIATTREXPRESSION           "UML:Expression"
#define XMIEXPRESSIONBODY           "UML:Expression.body"
#define XMIGUARDEXPRESSION          "UML:Guard.expression"
#define XMIBOOLEANEXPRESSION        "UML:BooleanExpression"

#define XMICLASSIFIER               "UML:Classifier"
#define XMIGENERALIZATION           "UML:Generalization"
#define XMIGENERALIZATIONPARENT     "UML:Generalization.parent"
#define XMIGENERALIZATIONCHILD      "UML:Generalization.child"
#define XMIGENERALIZABLEELEMENT     "UML:GeneralizableElement"
#define XMICONNECTION               "UML:Association.connection"
#define XMIEND                      "UML:AssociationEnd"
#define XMIENDPARTICPANT            "UML:AssociationEnd.participant"
#define XMIINSTANCE                 "UML:Instance"
#define XMIINSTANCESLOT             "UML:Instance.slot"
#define XMIINSTANCECLASS            "UML:Instance.classifier"
#define XMIATTRIBUTELINK            "UML:AttributeLink"
#define XMIATTRIBUTELINKVAL         "UML:AttributeLink.value"
#define XMITRANSITIONS              "UML:StateMachine.transitions"
#define XMITRANSITIONSOURCE         "UML:Transition.source"
#define XMITRANSITIONTARGET         "UML:Transition.target"
#define XMITRANSITIONTRIGGER        "UML:Transition.trigger"
#define XMITRANSITIONGUARD          "UML:Transition.guard"
#define XMISTATEVERTEX              "UML:StateVertex"
#define XMISTATEMACHINETOP          "UML:StateMachine.top"
#define XMISTATEMACHINECS           "UML:CompositeState"
#define XMISTATEMACHINECONTEXT      "UML:StateMachine.context"
#define XMISTATESUBVERTEX           "UML:CompositeState.subvertex"
#define XMIMODELELEMENT             "UML:ModelElement"
#define XMIPARTITIONS               "UML:ActivityGraph.partition"
#define XMIPARTITION                "UML:Partition"
#define XMIPARTITIONCONTENT         "UML:Partition.contents"
#define XMISTATEMACHINETOP          "UML:StateMachine.top"
#define XMISUBACTIVITYSTATE         "UML:SubactivityState"
#define XMICOMPOSSTATEVERTEX        "UML:CompositeState.subvertex"
#define XMIPSEUDOSTATE              "UML:Pseudostate"
#define XMIFINALSTATE               "UML:FinalState"
#define XMIACTIONSTATE              "UML:ActionState"
#define XMIOBJECTFLOWSTATE          "UML:ObjectFlowState"
#define XMIOBJECTFLOWSTATETYPE      "UML:ObjectFlowState.type"
#define XMIDOACTIVITY               "UML:State.doActivity"
#define XMIENTRYACTIVITY            "UML:State.entry"
#define XMIEXITACTIVITY             "UML:State.exit"
#define XMICALLACTION               "UML:CallAction"
#define XMICALLACTIONOP             "UML:CallAction.operation"
#define XMISENDACTI0N               "UML:SendAction"
#define XMISENDACTI0NSIG            "UML:SendAction.signal"
#define XMIELTTAGGEDVALUE           "UML:ModelElement.taggedValue"
#define XMITAGGEDVALUE              "UML:TaggedValue"
#define XMITAGGEDVALUETYPE          "UML:TaggedValue.type"
#define XMITAGGEDVALUEDEF           "UML:TagDefinition"
#define XMITAGGEDVALUETTYPE         "UML:TagDefinition.tagType"
#define XMITAGGEDVALUEVALUE         "UML:TaggedValue.dataValue"
#define XMISIGNALEVENTSIG           "UML:SignalEvent.signal"
#define XMITIMEEVENTWHEN            "UML:TimeEvent.when"
#define XMICHANGEEVENTEXP           "UML:ChangeEvent.changeExpression"
#define XMICALLEVENTOPER            "UML:CallEvent.operation"
#define XMITIMEEXPRESSION           "UML:TimeExpression"

#define XMIMODEL                    "UML:Model"
#define XMIPACKAGE                  "UML:Package"
#define XMIDATATYPE                 "UML:DataType"
#define XMICLASS                    "UML:Class"
#define XMIATTRIBUTE                "UML:Attribute"
#define XMIOPERATION                "UML:Operation"
#define XMIASSOCIATION              "UML:Association"
#define XMIOBJECT                   "UML:Object"
#define XMISTATEMACHINE             "UML:StateMachine"
#define XMISIMPLESTATE              "UML:SimpleState"
#define XMITRANSITION               "UML:Transition"
#define XMIACTIVITYGRAPH            "UML:ActivityGraph"
#define XMISIGNAL                   "UML:Signal"
#define XMIEVENT                    "UML:Event"
#define XMISIGNALEVENT              "UML:SignalEvent"
#define XMITIMEEVENT                "UML:TimeEvent"
#define XMICHANGEEVENT              "UML:ChangeEvent"
#define XMICALLEVENT                "UML:CallEvent"
#define XMIGUARD                    "UML:Guard"

#define XMIATTMULTIPLICITY          "UML:StructuralFeature.multiplicity"
#define XMIENDMULTIPLICITY          "UML:AssociationEnd.multiplicity"
#define XMIENDNAVIGABLE             "UML:AssociationEnd.isNavigable"
#define XMIENDAGGREGATIONTYPE       "UML:AssociationEnd.aggregation"
#define XMIMULTIPLICITY             "UML:Multiplicity"
#define XMIMULTIPLICITY_RANGE       "UML:Multiplicity.range"
#define XMIMULTIPLICITYRANGE        "UML:MultiplicityRange"
#define XMIMULTIPLICITYLOWER        "UML:MultiplicityRange.lower"
#define XMIMULTIPLICITYUPPER        "UML:MultiplicityRange.upper"

#define XMIOPERATIONPARAMETER       "UML:BehavioralFeature.parameter"
#define XMIPARAMETER                "UML:Parameter"
#define XMIPARAMETERKIND            "UML:Parameter.kind"
#define XMIPARAMETERTYPE            "UML:Parameter.type"

#define XMIMODELELEMENTVISIBILITY   "UML:ModelElement.visibility"
#define XMIMODELELEMENTSTEREOTYPE   "UML:ModelElement.stereotype"
#define XMISTEREOTYPE               "UML:Stereotype"

#define XMIMODELELTCLIENTDEPENDENCY "UML:ModelElement.clientDependency"
#define XMIDEPENDENCY               "UML:Dependency"
#define XMIDEPENDENCYCLIENT         "UML:Dependency.client"
#define XMIDEPENDENCYSUPPLIER       "UML:Dependency.supplier"

#define XMIEXTENSIONS               "XMI.extensions"
#define XMIEXTENSION                "XMI.extension"
#define XMIREFERENCE                "XMI.reference"

#define XMIMODELELEMENTCOMMENT      "UML:ModelElement.comment"
#define XMICOMMENT                  "UML:Comment"


XMI11Loader::XMI11Loader()
{
	namespaces["UML"]="//org.omg/UML/1.4";
}
XMI11Loader::~XMI11Loader()
{

}

shared_ptr<Model> XMI11Loader::getModel(shared_ptr<XmlNode> node)
{
	shared_ptr<Model> model(new Model(""));
	shared_ptr<XmlNode> xmlRoot=node->selectSingleNode(string(XMICONTENT)+"/"+XMIMODEL,namespaces);
	//shared_ptr<XmlNode> xmlContent = xmlRoot->getFirstChild();
	vector<shared_ptr<XmlNode> > dataTypes=xmlRoot->selectNodes(string(XMIOWNEDELEMENT)+"/"+XMIDATATYPE,namespaces);
	for(size_t i=0;i<dataTypes.size();i++)
	{
		shared_ptr<PrimitiveType> type=getDatatype(dataTypes[i]);
		model->addBasicType(type);
	}
	vector<shared_ptr<XmlNode> > stereoTypes=xmlRoot->selectNodes(string(XMIOWNEDELEMENT)+"/"+XMISTEREOTYPE,namespaces);
	for(size_t i=0;i<stereoTypes.size();i++)
	{
		addStereoType(stereoTypes[i]);
	}
	model->setPackage(getPackage(xmlRoot,model));
	linkParametersType();
	linkGeneralizations();
	shared_ptr<Class>  classe(new EntityClass("undef"));
	model->getPackage()->addClasses(classe);
	model->registerAllClasses();
	return model;
}
void XMI11Loader::addStereoType(shared_ptr<XmlNode> node)
{
	_stereotypes[getElementId(node)] = getElementName(node);
}
shared_ptr<Package> XMI11Loader::getPackage(shared_ptr<XmlNode> node, shared_ptr<Model> model)
{
	shared_ptr<Package> package(new Package(getElementName(node)));
	package->setSummary(getElementSummary(node));
	package->setDescription(getElementDescription(node));
	_packages[getElementId(node)] = package;
	string stereotypeRef = getElementStereotypeRef(node);
	string stereotype = "Package";
	if (stereotypeRef != "")
		stereotype =  _stereotypes[stereotypeRef];
	if (stereotype == "Team")
	{
		//_addTeamsFromPackage(xmiPackage);
	}

	vector<shared_ptr<XmlNode> > signalNodes=node->selectNodes(string(XMIOWNEDELEMENT)+"/"+XMISIGNAL,namespaces);
	for(size_t i=0;i<signalNodes.size();i++)
	{
		shared_ptr<Signal> signal( new Signal(getElementName(signalNodes[i])) );
		_signals[getElementId(signalNodes[i])] = signal;
		//_parseSignal(signal,xmiSignal);
		package->addSignals(signal);
	}
	vector<shared_ptr<XmlNode> > classNodes=node->selectNodes(string(XMIOWNEDELEMENT)+"/"+XMICLASS,namespaces);
	for(size_t i=0;i<classNodes.size();i++)
	{
		shared_ptr<Class> classe=getClass(classNodes[i]);
		package->addClasses(classe);
	}
	vector<shared_ptr<XmlNode> > packageNodes=node->selectNodes(string(XMIOWNEDELEMENT)+"/"+XMIPACKAGE,namespaces);
	for(size_t i=0;i<packageNodes.size();i++)
	{
		shared_ptr<Package> childPackage=getPackage(packageNodes[i],model);
		package->addPackage(childPackage);
	}
	return package;
}
shared_ptr<PrimitiveType> XMI11Loader::getDatatype(shared_ptr<XmlNode> node)
{
	shared_ptr<PrimitiveType> type(new PrimitiveType(getElementName(node)));
	//On recupere ceux qui existe dj
	//type=model->getBasicType(getElementId(node));
	_basicTypes[getElementId(node)] = type;
	_classifiers[getElementId(node)] = type;
	return type;
}
shared_ptr<Class> XMI11Loader::getClass(shared_ptr<XmlNode> node)
{
	shared_ptr<Class> classe;
	string stereotypeRef = getElementStereotypeRef(node);
	string stereotype = "Class";
	if (stereotypeRef != "")
		stereotype =  _stereotypes[stereotypeRef];

	if (stereotype == "Entity")
	{
		classe=shared_ptr<Class>( new EntityClass(getElementName(node)) );
	}
	else if (stereotype == "VirtualHuman")
	{
		classe=shared_ptr<Class>( new VirtualHumanClass(getElementName(node)) );
	}
	else if (stereotype == "RoleClass")
	{
		classe=shared_ptr<Class>( new RoleClass(getElementName(node)) );
		//_roleClasses[xmiClasse->getId()] = shared_dynamic_cast<RoleClass>(classe2);
	}
	else
	{
		classe=shared_ptr<Class>( new Class(getElementName(node)) );
	}
	string id=getElementId(node);
	_classes[id] = classe;
	_classifiers[id] = classe;
	// TODO: A faire partout
	classe->setId(id);
	vector<shared_ptr<XmlNode> > attNodes=node->selectNodes(string(XMIFEATURE)+"/"+XMIATTRIBUTE,namespaces);
	for(size_t i=0;i<attNodes.size();i++)
	{
		shared_ptr<Property> prop=getAttribute(attNodes[i],classe);
		classe->addAttribute(prop);
	}
	vector<shared_ptr<XmlNode> > opNodes=node->selectNodes(string(XMIFEATURE)+"/"+XMIOPERATION,namespaces);
	for(size_t i=0;i<opNodes.size();i++)
	{
		shared_ptr<Operation> op=getOperation(opNodes[i],classe);
		classe->addOperation(op);
	}
	vector<shared_ptr<XmlNode> > generalizationNodes=node->selectNodes(string(XMIOWNEDELEMENT)+"/"+XMIGENERALIZATION+"/"+XMIGENERALIZATIONPARENT,namespaces);
	for(size_t i=0;i<generalizationNodes.size();i++)
	{
		addGeneralization(generalizationNodes[i],classe);
	}
	vector<shared_ptr<XmlNode> > associationNodes=node->selectNodes(string(XMIOWNEDELEMENT)+"/"+XMIASSOCIATION,namespaces);
	for(size_t i=0;i<associationNodes.size();i++)
	{
		shared_ptr<Property> prop=getAssociation(associationNodes[i],classe);
		classe->addAttribute(prop);
	}

	return classe;
}
void XMI11Loader::addGeneralization(shared_ptr<XmlNode> node, shared_ptr<Class> c)
{
	shared_ptr<XmlNode> element=node->selectSingleNode(XMIGENERALIZABLEELEMENT,namespaces);
	if(element)
	{
		pair<string,string> g;
		g.first = element->getProperty(XMIIDREF);
		g.second = c->getId();
		_generalisations.push_back(g);
	}
}
shared_ptr<Operation> XMI11Loader::getOperation(shared_ptr<XmlNode> node,shared_ptr<Class> c)
{
	OpaqueBehavior* b;
	string opName=getElementName(node);
	/* sauvegarde du contenu C++ si prsent */
	string data;
	//default C++
	string opClassName = c->getName() + "_" + opName;
	//getElementTaggedValue("C++",data);
	if (!data.empty())
	{
		if (data.find("{PYTHON}")==0)
		{
			data=data.substr(strlen("{PYTHON}"));
#ifndef NOPYTHON
			cerr<<"Create Python Opaque behavior"<<endl;
			cerr<<":::: "<<data<<"::::"<<endl;
			b=new OpaquePythonBehavior(opClassName);
			b->setBody(data);
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
			b=new OclQuery(opClassName);
			b->setBody(data);

		}
		else
		{
			_opaqueBehaviorsCode.push_back(make_pair(opClassName,data));
			b=new OpaqueBehavior(opName);
			b->setBody(opName);
		}
	}
	else
	{
		b=new OpaqueBehavior(opName);
		b->setBody(getElementName(node));
	}
	shared_ptr<Operation> op(new Operation(opName,c,shared_ptr<OpaqueBehavior>(b)));
	vector<shared_ptr<XmlNode> > paramNodes=node->selectNodes(string(XMIOPERATIONPARAMETER)+"/"+XMIPARAMETER,namespaces);
	_idOperations[getElementId(node)]=op;
	for(size_t i=0;i<paramNodes.size();i++)
	{
		shared_ptr<Parameter> p=getParameter(paramNodes[i]);
		op->addParameter(p);
	}
	return op;
}
shared_ptr<Property> XMI11Loader::getAttribute(shared_ptr<XmlNode> node,shared_ptr<Class> c)
{
	string attributeName=getElementName(node);
	string typeRef=node->selectSingleNode(string(XMISTRUCTURALTYPE)+"/"+XMICLASSIFIER,namespaces)->getProperty(XMIIDREF);
	shared_ptr<PrimitiveType> typeAttribute = _basicTypes[typeRef];
	if (!typeAttribute)
	{
		cerr << "Erreur type : " << typeRef << " pour " << 	attributeName << endl;
		//TODO: pb, que fait on ?
		return shared_ptr<Property>();
	}
	string defaultValue;
	shared_ptr<ValueSpecification> value;
	bool derived=false;
	if (attributeName[0]=='/')
	{
		derived=true;
		attributeName=attributeName.substr(1);
	}
	shared_ptr<XmlNode> defaultValueNode = node->selectSingleNode(string(XMIINITIALVALUE)+"/"+XMIATTREXPRESSION+"/"+XMIEXPRESSIONBODY,namespaces);
	if(defaultValueNode)
	{
		defaultValue=defaultValueNode->getInnerText();
		if(!defaultValue.empty())
		{
			if (!derived)
			{
				value=typeAttribute->createValueFromString(defaultValue);
			}
			else
			{
				value=shared_ptr<ValueSpecification>(new Expression(defaultValue,typeAttribute));
			}
		}
	}
	shared_ptr<Property> prop(new Property(attributeName,
			c.get(), // class container
			typeAttribute, // attribute type
			shared_ptr<Property>(), // opposite
			value)); // default value
	return prop;
}
shared_ptr<Property> XMI11Loader::getAssociation(shared_ptr<XmlNode> node,shared_ptr<Class> c)
{
	vector<shared_ptr<XmlNode> > ends=node->selectNodes(string(XMICONNECTION)+"/"+XMIEND,namespaces);
	if(ends.size() != 2)
	{
		cerr << "Problem: expected 2 association end in the association "<<getElementName(node)<<". got "<<ends.size()<<endl;
		// TODO: que faire ??
		return shared_ptr<Property>();
	}
	shared_ptr<XmlNode> participantNode=ends[0]->selectSingleNode(string(XMIENDPARTICPANT)+"/"+XMICLASSIFIER,namespaces);
	if(!participantNode)
	{
		cerr << "Problem: association end " << getElementName(ends[0]) << " of class " << c->getName() << " doesn't have a participant node" << endl;
		// TODO: que faire ??
		return shared_ptr<Property>();
	}
	string participantRef1=participantNode->getProperty(XMIIDREF);
	participantNode=ends[1]->selectSingleNode(string(XMIENDPARTICPANT)+"/"+XMICLASSIFIER,namespaces);
	if(!participantNode)
	{
		cerr << "Problem: association end " << getElementName(ends[1]) << " of class " << c->getName() << " doesn't have a participant node" << endl;
		// TODO: que faire ??
		return shared_ptr<Property>();
	}
	string participantRef2=participantNode->getProperty(XMIIDREF);
	shared_ptr<Class> cl1;
	shared_ptr<Class> cl2;
	map<string, shared_ptr<Class> >::iterator itC;
	itC = _classes.find(participantRef1);
	if (itC != _classes.end())
	{
		cl1 = itC->second;
	}
	else
	{
		cerr << participantRef1 <<" ref not found to instanciate the association " << getElementName(ends[0]) << " of class " << c->getName() << endl;
		// TODO: que faire ??
		return shared_ptr<Property>();
	}
	itC = _classes.find(participantRef2);
	if (itC != _classes.end())
	{
		cl2 = itC->second;
	}
	else
	{
		cerr << participantRef2 <<" ref not found to instanciate the association " << getElementName(ends[1]) << " of class " << c->getName() << endl;
		// TODO: que faire ??
		return shared_ptr<Property>();
	}
	bool navigable1=false;
	bool navigable2=false;
	shared_ptr<XmlNode> navNode=ends[0]->selectSingleNode(XMIENDNAVIGABLE);
	if(navNode)
		navNode->getPropertyBoolean(XMIVALUE,navigable1);
	navNode=ends[1]->selectSingleNode(XMIENDNAVIGABLE);
	if(navNode)
		navNode->getPropertyBoolean(XMIVALUE,navigable2);
	shared_ptr<Property> property1,property2;

	if (navigable1)
	{
		property1=shared_ptr<Property> ( new Property(
				getElementName(ends[0]),cl2.get(),cl1));
		//property1->setDescription(l1->getDescription());
		//property1->setSummary(l1->getSummary());
		//property1->setTags(l1->getTags());

		string multStr;
		shared_ptr<XmlNode> multNode=ends[0]->selectSingleNode(string(XMIENDMULTIPLICITY)+"/"+XMIMULTIPLICITY+"/"+XMIMULTIPLICITY_RANGE+"/"+XMIMULTIPLICITYRANGE+"/"+XMIMULTIPLICITYUPPER,namespaces);
		if(multNode)
		{
			//upper
			multStr= multNode->getChildByName("text")->getString();
			property1->setMaximumNumber(boost::lexical_cast<int>(multStr));
		}
		multNode=ends[0]->selectSingleNode(string(XMIENDMULTIPLICITY)+"/"+XMIMULTIPLICITY+"/"+XMIMULTIPLICITY_RANGE+"/"+XMIMULTIPLICITYRANGE+"/"+XMIMULTIPLICITYLOWER,namespaces);
		if(multNode)
		{
			//upper
			multStr= multNode->getChildByName("text")->getString();
			property1->setMinimumNumber(boost::lexical_cast<int>(multStr));
		}

		cl2->addAttribute(property1);
	}
	if (navigable2)
	{
		property2=shared_ptr<Property> ( new Property(
				getElementName(ends[1]),cl1.get(),cl2));
		//property2->setDescription(l2->getDescription());
		//property2->setSummary(l2->getSummary());
		//property2->setTags(l2->getTags());
		property2->setOpposite(property1);
		string multStr;
		shared_ptr<XmlNode> multNode=ends[1]->selectSingleNode(string(XMIENDMULTIPLICITY)+"/"+XMIMULTIPLICITY+"/"+XMIMULTIPLICITY_RANGE+"/"+XMIMULTIPLICITYRANGE+"/"+XMIMULTIPLICITYUPPER,namespaces);
		if(multNode)
		{
			//upper
			multStr= multNode->getChildByName("text")->getString();
			property2->setMaximumNumber(boost::lexical_cast<int>(multStr));
		}
		multNode=ends[1]->selectSingleNode(string(XMIENDMULTIPLICITY)+"/"+XMIMULTIPLICITY+"/"+XMIMULTIPLICITY_RANGE+"/"+XMIMULTIPLICITYRANGE+"/"+XMIMULTIPLICITYLOWER,namespaces);
		if(multNode)
		{
			//upper
			multStr= multNode->getChildByName("text")->getString();
			property2->setMinimumNumber(boost::lexical_cast<int>(multStr));
		}

		cl1->addAttribute(property2);
	}
	if (property1)
		property1->setOpposite(property2);

	return property1;
}
shared_ptr<Parameter> XMI11Loader::getParameter(shared_ptr<XmlNode> node)
{
	string kind=node->selectSingleNode(XMIPARAMETERKIND,namespaces)->getProperty(XMIVALUE);
	shared_ptr<Parameter> param(new Parameter(getElementName(node),kind));
	_parameters[getElementId(node)] = param;
	//_operationType[getElementId(node)] = operation;
	string typeref=node->selectSingleNode(string(XMIPARAMETERTYPE)+"/"+XMICLASSIFIER,namespaces)->getProperty(XMIIDREF);
	_paramToType[getElementId(node)] = typeref;
	return param;
}

// Behavior
// State Machines
shared_ptr<StateMachine> XMI11Loader::getStateMachine(shared_ptr<XmlNode> node)
{
	shared_ptr<StateMachine> sm(new StateMachine(getElementName(node)));
	return sm;
}
shared_ptr<State> XMI11Loader::getState(shared_ptr<XmlNode> node)
{
	shared_ptr<State> state(new State(getElementName(node)));
	return state;
}
shared_ptr<PseudoState> XMI11Loader::getPseudoState(shared_ptr<XmlNode> node)
{
	shared_ptr<PseudoState> state(new PseudoState(getElementName(node)));
	return state;
}
shared_ptr<Transition> XMI11Loader::getTransition(shared_ptr<XmlNode> node)
{
	shared_ptr<Transition> tr(new Transition());
	return tr;
}
shared_ptr<Region> XMI11Loader::getRegion(shared_ptr<XmlNode> node)
{
	shared_ptr<Region> region(new Region());
	return region;
}
shared_ptr<Guard> XMI11Loader::getGuard(shared_ptr<XmlNode> node)
{
	shared_ptr<Guard> guard(new Guard());
	return guard;
}
shared_ptr<SignalEvent> XMI11Loader::getSignalEvent(shared_ptr<XmlNode> node)
{
	shared_ptr<SignalEvent> signalEvent(new SignalEvent(getElementName(node)));
	return signalEvent;
}
shared_ptr<TimeEvent> XMI11Loader::getTimeEvent(shared_ptr<XmlNode> node)
{
	shared_ptr<TimeEvent> timeEvent(new TimeEvent(getElementName(node)));
	return timeEvent;
}

// Activities
shared_ptr<Activity> XMI11Loader::getActivity(shared_ptr<XmlNode> node)
{
	shared_ptr<Activity> act(new Activity(getElementName(node)));
	return act;
}
shared_ptr<ActionNode> XMI11Loader::getActionNode(shared_ptr<XmlNode> node)
{
	shared_ptr<ActionNode> act(new ActionNode(getElementName(node),""));
	return act;
}
shared_ptr<ControlNode> XMI11Loader::getControlNode(shared_ptr<XmlNode> node)
{
	shared_ptr<ControlNode> controlNode(new ControlNode(getElementName(node),""));
	return controlNode;
}
shared_ptr<ActivityEdge> XMI11Loader::getActivityEdge(shared_ptr<XmlNode> node)
{
	shared_ptr<ActivityEdge> activityEdge(new ActivityEdge(getElementName(node)));
	return activityEdge;
}
shared_ptr<ObjectNode> XMI11Loader::getObjectNode(shared_ptr<XmlNode> node)
{
	shared_ptr<ObjectNode> objectNode(new ObjectNode(getElementName(node)));
	return objectNode;
}
shared_ptr<Action> XMI11Loader::getAction(shared_ptr<XmlNode> node)
{
	//shared_ptr<Action> action(new Action());
	//return action;
	return shared_ptr<Action>();
}

string XMI11Loader::getElementName(shared_ptr<XmlNode> node)
{
	string name="";
	shared_ptr<XmlNode> nameNode=node->selectSingleNode(XMIMODELELEMENTNAME,namespaces);
	if (nameNode->getChildByName("text"))
		name=nameNode->getChildByName("text")->getString();
	return name;
}

string XMI11Loader::getElementId(shared_ptr<XmlNode> node)
{
	string id;
	node->getPropertyString(XMIID, id);
	return id;
}


string XMI11Loader::getElementSummary(shared_ptr<XmlNode> node)
{
	string summary = "";
	shared_ptr<XmlNode> summaryNode=node->selectSingleNode(
			string(XMIELTTAGGEDVALUE)+"/"+XMITAGGEDVALUE+
			"["+XMITAGGEDVALUETYPE+"/"+XMITAGGEDVALUEDEF+"/"+XMITAGGEDVALUETTYPE+"/text() = 'summary']/"+XMITAGGEDVALUEVALUE,namespaces);
	if(summaryNode)
	{
		summary=summaryNode->getChildByName("text")->getString();
	}
	return summary;

}


string XMI11Loader::getElementDescription(shared_ptr<XmlNode> node)
{
	string description = "";
	shared_ptr<XmlNode> descNode=node->selectSingleNode(
			string(XMIELTTAGGEDVALUE)+"/"+XMITAGGEDVALUE+
			"["+XMITAGGEDVALUETYPE+"/"+XMITAGGEDVALUEDEF+"/"+XMITAGGEDVALUETTYPE+"/text() = 'documentation']/"+XMITAGGEDVALUEVALUE,namespaces);
	if(descNode)
	{
		description=descNode->getChildByName("text")->getString();
	}
	return description;

}
/*map<string,string> XMI11Loader::getTags(void)
{
    map<string,string> tags;
    shared_ptr<XmlNode> children = _root->getChild (XMIELTTAGGEDVALUE);
    if (children)
    {
        children = children->getFirstChild();
        while ( children )
        {
            if (children->getName() == XMITAGGEDVALUE)
            {
                shared_ptr<XmlNode> t2 = children;
                if (t2->getChild(XMITAGGEDVALUETYPE))
                {
                    shared_ptr<XmlNode> t3 = t2->getChild(XMITAGGEDVALUETYPE);
                    if (t3->getChild(XMITAGGEDVALUEDEF))
                    {
                        string tagType = t3->getChild(XMITAGGEDVALUEDEF)->getChild(XMITAGGEDVALUETTYPE)->getChild("text")->getString();
                        if (tagType != "documentation" && tagType != "summary")
                        {
                        	tags[tagType]=t2->getChild(XMITAGGEDVALUEVALUE)->getChild("text")->getString();
                        }
                    }
                }
            }
            children = children->getNext();
        }
    }
    return tags;

}

string XMI11Loader::getElementVisibility(void)
{
    string visibility;
    node->getChild(XMIMODELELEMENTVISIBILITY)->getPropertyString(XMIVALUE,visibility);
    return visibility;

}*/


string XMI11Loader::getElementStereotypeRef(shared_ptr<XmlNode> node)
{
	string s;

	shared_ptr<XmlNode> xmlNode = node->getChildByName(XMIMODELELEMENTSTEREOTYPE);
	if(xmlNode != NULL)
	{
		shared_ptr<XmlNode> xmlNode2 = xmlNode->getChildByName(XMISTEREOTYPE);
		if (xmlNode2 != NULL)
		{
			xmlNode2->getPropertyString(XMIIDREF,s);
		}
	}
	return s;
}



/*bool XMI11Loader::getTaggedValue(const string type, string & data)
{
    if (_root)
    {
        int occurence=1;
        shared_ptr<XmlNode> xmlNode;
        while( (xmlNode = _root->getChild(XMIELTTAGGEDVALUE, occurence++)))
        {
            if (xmlNode)
            {
                int occurence2=1;
                shared_ptr<XmlNode> xmlNode2;
                while( (xmlNode2=xmlNode->getChild(XMITAGGEDVALUE,occurence2++)))
                {
                    if(type==xmlNode2->getChild(XMITAGGEDVALUETYPE)->getChild(XMITAGGEDVALUEDEF)
                        ->getChild(XMITAGGEDVALUETTYPE)->getChild("text")->getString())
                    {
                        shared_ptr<XmlNode> textChild;
                        if ((textChild=xmlNode2->getChild(XMITAGGEDVALUEVALUE)->getChild("text")))
                            data= textChild->getString();
                        return true;
                    }

                }
            }
        }
    }
    return false;
}*/


// Second Pass
void XMI11Loader::linkParametersType()
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

void XMI11Loader::linkGeneralizations()
{
	for (size_t i = 0; i < _generalisations.size(); i++)
	{

		shared_ptr<Class> general = _classes[_generalisations[i].first];
		shared_ptr<Class> specific = _classes[_generalisations[i].second];
		general->addChild(specific);
		//Generalization * gen = new Generalization();
		//gen->setGeneral(general);
		//gen->setSpecific(specific);
		//general->addGeneralization(gen);
		//specific->addGeneralization(gen);
	}
}
