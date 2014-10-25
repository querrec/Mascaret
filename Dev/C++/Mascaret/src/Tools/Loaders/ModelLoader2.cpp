#include "Tools/filesystem.h"
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

//#include "VEHA/Behavior/BasicActions/CallOperationAction.h"
//#include "VEHA/Behavior/BasicActions/CallBehaviorAction.h"
#include "VEHA/Behavior/BasicActions/SendSignalAction.h"
#include "VEHA/Behavior/BasicActions/AddStructuralFeatureValueAction.h"
#include "VEHA/Behavior/BasicActions/AcceptEventAction.h"
#include "VEHA/Behavior/BasicActions/InputPin.h"
#include "VEHA/Behavior/BasicActions/OutputPin.h"
#include "VEHA/Behavior/BasicActions/ValuePin.h"

#include "VEHA/Behavior/Activity/Activity.h"
#include "VEHA/Behavior/Activity/ActionNode.h"
#include "VEHA/Behavior/Activity/ObjectNode.h"
#include "VEHA/Behavior/Activity/ActivityParameterNode.h"
#include "VEHA/Behavior/Activity/ActivityEdge.h"
#include "VEHA/Behavior/Activity/InitialNode.h"
#include "VEHA/Behavior/Activity/FinalNode.h"
#include "VEHA/Behavior/Activity/MergeNode.h"
#include "VEHA/Behavior/Activity/DecisionNode.h"
#include "VEHA/Behavior/Activity/JoinNode.h"
#include "VEHA/Behavior/Activity/ForkNode.h"
#include "VEHA/Behavior/Activity/LoopNode.h"
#include "VEHA/Behavior/Activity/ControlFlow.h"
#include "VEHA/Behavior/Activity/ObjectFlow.h"
#include "VEHA/Behavior/Activity/ActivityPartition.h"

#include "HAVE/VirtualHumanClass.h"
#include "BEHAVE/AgentClass.h"
#include "BEHAVE/RoleClass.h"
#include "BEHAVE/Role.h"
#include "BEHAVE/Ressource.h"
#include "BEHAVE/Procedure.h"
#include "BEHAVE/OrganisationalStructure.h"
#include "BEHAVE/AgentPlateform.h"

#include "SysML/Block.h"

#include <stdexcept>    
#include <iostream>
#include "VEHA/Behavior/Common/pluginManager.h"
#include <boost/lexical_cast.hpp>
#include "Tools/utils.h"
#include "boost/algorithm/string.hpp"
//#include "boost/algorithm/string/case_conv.hpp"

DECLARE_NAMESPACE_FILESYSTEM;

using namespace VEHA;
using namespace HAVE;
using namespace BEHAVE;
using namespace SysML;

#include "Tools/Loaders/ModelLoader2.h"

ModelLoader2::ModelLoader2(string str, bool strIsFileName) {
	// Bouml preserved body begin 0001F4E7
	if(strIsFileName)
		cerr << "[ModelLoader2 Info] #### Start parsing model file: " << str << endl;
	else
		cerr << "[ModelLoader2 Info] #### Start parsing model string: " << str << endl;

	_loader = boost::make_shared<XmlParser>();
	if ((strIsFileName && _loader->parseFile(str)) || (!strIsFileName
			&& _loader->parseString(str))) {
		_xmi = _loader->getRoot();

		shared_ptr<XmlNode> modelNode;
		if (_xmi->getName() == "Model")
			modelNode = _xmi;
		else if (_xmi->getName() == "parser")
			modelNode = _xmi->getFirstChild()->getChildByName("Model");
		else
			modelNode = _xmi->getChildByName("Model");

		string modelName = modelNode->getProperty("name");
		cout << "Model : " << modelName << endl;

		if (MascaretApplication::getInstance()->hasModel(modelName)) {
			_model = MascaretApplication::getInstance()->getModel(modelName);
		} else 
		{
			_model = boost::make_shared<Model>(modelName);
			if (strIsFileName)
				_model->setUrl(str);

			MascaretApplication::getInstance()->addModel(_model);

			addStereotypes();

			addPackage(modelNode, shared_ptr<Package> ());

			_setParametersType();
			addGeneralizations();
			addAssociation();
			addActivitiesAndStateMachines();
			addCallOperations();
			addCallBehaviors();
			addConditions();

			string filename = (strIsFileName ? str : modelNode->getProperty(
					"name"));
#if BOOST_FILESYSTEM_VERSION == 2
			string basename=fs::path(filename).leaf();
#else
			string basename = fs::path(filename).leaf().string();
#endif

			/*PluginManager::getInstance()->updatePlugin(basename+"_GeneratedOpaqueBehaviors",_opaqueBehaviorsCode);
			 if (!PluginManager::getInstance()->openPlugin(basename+"_GeneratedOpaqueBehaviors.so"))
			 cerr<<"can't open plugin associate to "<<filename<<" model ("<<basename<<"_GeneratedOpaqueBehaviors.so)"<<endl;*/

			shared_ptr<Class> classe = boost::make_shared<EntityClass>("undef");
			_model->getPackage()->addClasses(classe);

			_model->registerAllClasses();
		}
	} else {
		cerr << "[ModelLoader2 Error] Errors when openning the file." << endl;
	}

	if(strIsFileName)
		cerr << "[ModelLoader2 Info] #### Finish parsing model file: " << str << endl << endl << endl;
	else
		cerr << "[ModelLoader2 Info] #### Finish parsing model string." << endl << endl;
	// Bouml preserved body end 0001F4E7
}

void ModelLoader2::reload(shared_ptr<Class> classe) {
	/*
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
	 */

}

void ModelLoader2::addPackage(const shared_ptr<XmlNode> & packageNode,
		const shared_ptr<Package> & parent) {
	// Bouml preserved body begin 0001F567
	string type, pkgName;
	packageNode->getPropertyString("type", type);
	if (type == "" && packageNode->getName() == "Model")
		type = "uml:Model";

	packageNode->getPropertyString("name", pkgName);
	if (type != "uml:Model" && type != "uml:Package") {
		cerr << pkgName << " is a " << type
				<< " and must be a Package or a Model" << endl;
		return;
	}

	shared_ptr<Package> pkg;
	if (type == "uml:Package" || type == "uml:Model") {
		cerr << "[ModelLoader2 Info] Parsing Package : " << pkgName << endl;
		pkg = boost::make_shared<Package>(pkgName);

		//_packages.push_front(pkg);
		pkg->setDescription(getComment(packageNode));
		pkg->setSummary(getSummary(packageNode));
		pkg->setTags(getTags(packageNode));

		if (parent)
		{
			cerr << "[ModelLoader2 Info] Package " << pkgName << " got parent" << endl;
			parent->addPackage(pkg);
		}
		else
		{
			cerr << "[ModelLoader2 Info] Package " << pkgName << " is root" << endl;
			_model->setPackage(pkg);
		}
	}
	shared_ptr<XmlNode> child = packageNode->getFirstChild();
	while (child) {
		/* manage package, classes, activities, and stateMachines*/
		if (child->getName() == "packagedElement" || child->getName()
				== "ownedMember") {
			string childType = child->getProperty("type");
			if (childType == "uml:DataType" || childType == "uml:PrimitiveType")
				addDatatype(child);
			else if (childType == "uml:Signal")
				addSignal(child, pkg);
		} else if (child->getName() == "ownedRule") {
			string childType = child->getProperty("type");
			if (childType == "uml:Constraint") {
				ConstraintT constraint;
				constraint.id = child->getProperty("id");
				constraint.constrainedElementID = child->getProperty(
						"constrainedElement");
				shared_ptr<XmlNode> specifNode = child->getChildByName(
						"specification");
				constraint.specification = specifNode->getProperty("value");
				_constraints.push_back(constraint);
			}
		}
		child = child->getNext();
	}

	child = packageNode->getFirstChild();
	while (child) {
		/* manage package, classes, activities, and stateMachines*/
		if (child->getName() == "packagedElement" || child->getName()
				== "ownedMember") {
			string childType = child->getProperty("type");
			if (childType == "uml:Package")
				addPackage(child, pkg);

		}
		child = child->getNext();
	}

	child = packageNode->getFirstChild();
	while (child) {
		/* manage package, classes, activities, and stateMachines*/
		if (child->getName() == "packagedElement" || child->getName()
				== "ownedMember") {
			string childType = child->getProperty("type");
			if (childType == "uml:SignalEvent")
				addSignalEvent(child, pkg);
			if (childType == "uml:CallEvent")
				addCallEvent(child, pkg);
			if (childType == "uml:ChangeEvent")
				addChangeEvent(child, pkg);
			if (childType == "uml:TimeEvent")
				addTimeEvent(child, pkg);
		}

		/* manage signals */
		child = child->getNext();
	}
	child = packageNode->getFirstChild();
	while (child) {
		/* manage package, classes, activities, and stateMachines*/
		if (child->getName() == "packagedElement" || child->getName()
				== "ownedMember") {
			string childType = child->getProperty("type");
			if (childType == "uml:Class" || childType == "uml:Interface")
				addClass(child, pkg);
			else if (childType == "uml:Activity")
				_activities.push_back(make_pair<shared_ptr<XmlNode> ,
						shared_ptr<Package> > (child, pkg));
			//addActivity(child,pkg);
			else if (childType == "uml:StateMachine") //StateMachine
				_stateMachines.push_back(make_pair<shared_ptr<XmlNode> ,
						shared_ptr<Package> > (child, pkg));
			//addStateMachine(child,pkg);
		}

		/* manage signals */
		child = child->getNext();
	}
	child = packageNode->getFirstChild();
	while (child) {
		/* manage package, classes, activities, and stateMachines*/
		if (child->getName() == "packagedElement" || child->getName()
				== "ownedMember") {
			string childType = child->getProperty("type");
			if (childType == "uml:Collaboration")
				addOrganisationalStructure(child);
		}
		/* manage signals */
		child = child->getNext();
	}
	//add signals
	map<string, shared_ptr<Signal> >::iterator itSig;
	for (itSig = _signals.begin(); itSig != _signals.end(); itSig++)
		pkg->addSignals(itSig->second);
	//_signals.clear();


	// Bouml preserved body end 0001F567
}

vector<shared_ptr<OrganisationalStructure> > ModelLoader2::getOrganisationalStructure() {
	MascaretApplication * application = MascaretApplication::getInstance();
	shared_ptr<AgentPlateform> plateform = application->getAgentPlateform();
	return plateform->getOrganisationalStructure();
}

void ModelLoader2::addOrganisationalStructure(
		const shared_ptr<XmlNode> & orgNode) {
	string orgName;
	orgNode->getPropertyString("name", orgName);


	cerr << "[ModelLoader2 Info] Parsing Organisation : " << orgName << endl;

	shared_ptr<OrganisationalStructure> organisation = boost::make_shared<OrganisationalStructure>(orgName);
	organisation->setDescription(getComment(orgNode));
	organisation->setSummary(getSummary(orgNode));
	organisation->setTags(getTags(orgNode));

	MascaretApplication * application = MascaretApplication::getInstance();
	shared_ptr<AgentPlateform> plateform = application->getAgentPlateform();
	plateform->addOrganisationalStructure(organisation);

	shared_ptr<XmlNode> child = orgNode->getFirstChild();
	while (child) {
		if (child->getName() == "ownedAttribute") {
			string childType = child->getProperty2("type");
			string roleName = child->getProperty("name");
			shared_ptr<Classifier> classe = _classifiers[childType];
			
			cerr << ">>> PARSING ROLE NAME: " << roleName << " with TYPE: " << childType << endl;
			
			cerr << "classe: " << classe << endl;
			cerr << "shared_dynamic_cast<RoleClass> (classe): " << shared_dynamic_cast<RoleClass> (classe) << endl;
			cerr << "shared_dynamic_cast<EntityClass> (classe): " << shared_dynamic_cast<EntityClass> (classe) << endl;

			if (classe && shared_dynamic_cast<RoleClass> (classe)) {
				shared_ptr<Role> role = boost::make_shared<Role>(roleName);
				role->setRoleClass(shared_dynamic_cast<RoleClass> (classe));
				organisation->addRole(role);
				cerr << " >> is role" << endl;
			} else if (classe && shared_dynamic_cast<EntityClass> (classe)) {
				shared_ptr<Ressource> ressource = boost::make_shared<Ressource>(roleName);
				ressource->setEntityClass(shared_dynamic_cast<EntityClass> (
						classe));
				organisation->addRessource(ressource);
				cerr << " >> is resource" << endl;
			} else
			{
				cerr << " >> ERROR: class not found" << endl;
			}
		}
		child = child->getNext();
	}

	child = orgNode->getFirstChild();
	while (child) {
		if (child->getName() == "ownedBehavior") {
			string childType = child->getProperty("type");
			string childName = child->getProperty("name");
			string childId = child->getProperty("id");

			if (childType == "uml:Activity") {
				shared_ptr<Activity> activity = addActivity(child);
				shared_ptr<Procedure> procedure = boost::make_shared<Procedure>(childName);
				procedure->setActivity(activity);
				organisation->addProcedure(procedure);
				_idBehaviors[childId] = activity;
			}
		}

		child = child->getNext();
	}

}

void ModelLoader2::addClass(const shared_ptr<XmlNode> & classNode,
		const shared_ptr<Package> & pkg) {
	// Bouml preserved body begin 0001F5E7

	string type, className;

	classNode->getPropertyString("type", type);
	classNode->getPropertyString("name", className);
	if (type != "uml:Class" && type != "uml:Interface") {
		cerr << "[ModelLoader2 Warning] " << className << " is a " << type
				<< " and must be a class" << endl;
		return;
	}
	string id = classNode->getProperty("id");

	bool entity = false;
	bool agent = false;
	bool role = false;
	shared_ptr<Class> cl;

	if (isStereotypedEntity(classNode)) {
		cl = boost::make_shared<EntityClass>(className);
		entity = true;
	} else if (isStereotypedBlock(classNode)) {
		cl = boost::make_shared<Block>(className);
		entity = true;
	} else if (isStereotypedAgent(classNode)) {
		cl = boost::make_shared<AgentClass>(className);
		agent = true;
	} else if (isStereotypedVirtualHuman(classNode)) {
		cl = boost::make_shared<VirtualHumanClass>(className);
	} else if (isStereotypedRole(classNode)) {
		cl = boost::make_shared<RoleClass>(className);
		role = true;
	} else {
		cerr << " NON STEREOTYPED..... : " << className << endl;
		cl = boost::make_shared<EntityClass>(className);
	}

	/* add in package*/
	_idClass[id] = cl;
	_classifiers[id] = cl;
	if (pkg)
		pkg->addClasses(cl);
	cl->setDescription(getComment(classNode));
	cl->setSummary(getSummary(classNode));
	cl->setTags(getTags(classNode));

	shared_ptr<XmlNode> child = classNode->getFirstChild();
	while (child) {
		string childName = child->getName();

		if (childName == "ownedOperation")
			addOperation(child, cl);
		else if (childName == "ownedAttribute")
			addAttribute(child, cl);
		else if (childName == "generalization") {
			_toGeneralize.push_back(
					make_pair(id, child->getProperty("general")));
		}
		child = child->getNext();
	}

	map<shared_ptr<CallEvent> , string>::iterator it;
	for (it = _callEvents.begin(); it != _callEvents.end(); it++) {
		shared_ptr<Operation> oper = _idOperations[it->second];
		if (oper)
			it->first->setOperation(oper);
	}

	child = classNode->getFirstChild();
	while (child) {
		string childName = child->getName();

		if (childName == "ownedBehavior" || childName == "nestedClassifier") {
			if (child->getProperty("type") == "uml:StateMachine")
				addStateMachineToClass(child, cl);
			if (child->getProperty("type") == "uml:Activity")
				addActivityToClass(child, cl);
		}

		child = child->getNext();
	}
	// Bouml preserved body end 0001F5E7
}

void ModelLoader2::addSignal(const shared_ptr<XmlNode> & classNode,
		const shared_ptr<Package> & pkg) {
	// Bouml preserved body begin 0001F5E7

	string type, className;

	classNode->getPropertyString("type", type);
	classNode->getPropertyString("name", className);
	if (type != "uml:Signal") {
		cerr << "[ModelLoader2 Warning] " << className << " is a " << type
				<< " and must be a signal" << endl;
		return;
	}
	string id = classNode->getProperty("id");

	//cerr << " ############## Add Signal : " << className << endl;

	shared_ptr<Signal> signal = boost::make_shared<Signal>(className);

	/* add in package*/
	_signals[id] = signal;

	if (pkg)
		pkg->addSignals(signal);
	signal->setDescription(getComment(classNode));
	signal->setSummary(getSummary(classNode));
	signal->setTags(getTags(classNode));
	shared_ptr<XmlNode> child = classNode->getFirstChild();
	while (child) {
		string childName = child->getName();

		if (childName == "ownedOperation")
			addOperation(child, signal);
		else if (childName == "ownedAttribute") {
			cerr << "AddAttribute SIGNAL" << endl;
			addAttribute(child, signal);
		}

		child = child->getNext();
	}

	child = classNode->getFirstChild();
	while (child) {
		string childName = child->getName();

		if (childName == "ownedBehavior") {
			if (child->getProperty("type") == "uml:StateMachine")
				addStateMachineToClass(child, signal);
		} else if (childName == "nestedClassifier") {
			if (child->getProperty("type") == "uml:StateMachine")
				addStateMachineToClass(child, signal);
		}

		child = child->getNext();
	}
	// Bouml preserved body end 0001F5E7
}
void ModelLoader2::addCallEvent(const shared_ptr<XmlNode> & classNode,
		const shared_ptr<Package> & pkg) {
	// Bouml preserved body begin 0001F5E7

	string type, className;

	classNode->getPropertyString("type", type);
	if (type != "uml:CallEvent") {
		cerr << "[ModelLoader2 Warning] " << className << " is a " << type
				<< " and must be a signal" << endl;
		return;
	}
	string id = classNode->getProperty("id");

	shared_ptr<CallEvent> callEvent = boost::make_shared<CallEvent>(id);
	string operName = classNode->getProperty("operation");

	_callEvents[callEvent] = operName;

	/* add in package*/
	_events[id] = callEvent;

	// Bouml preserved body end 0001F5E7
}
void ModelLoader2::addSignalEvent(const shared_ptr<XmlNode> & classNode,
		const shared_ptr<Package> & pkg) {
	// Bouml preserved body begin 0001F5E7

	string type, className;

	classNode->getPropertyString("type", type);
	classNode->getPropertyString("name", className);
	if (type != "uml:SignalEvent") {
		cerr << "[ModelLoader2 Warning] " << className << " is a " << type
				<< " and must be a signal" << endl;
		return;
	}
	string id = classNode->getProperty("id");

	shared_ptr<SignalEvent> signalEvent = boost::make_shared<SignalEvent>(className);
	string sigName = classNode->getProperty("signal");
	shared_ptr<Signal> sig = _signals[sigName];
	signalEvent->setSignalClass(_signals[sigName]);

	/* add in package*/
	_events[id] = signalEvent;

	// Bouml preserved body end 0001F5E7
}

//TODO: choper contenu de data (tag? description?)

void ModelLoader2::addOperation(const shared_ptr<XmlNode> & opNode,
		const shared_ptr<Class> & cl) {
	// Bouml preserved body begin 0001FAE7
	string opName;
	opNode->getPropertyString("name", opName);

	// Pour l'instant que Opaque Behavior (C++ / Python)
	OpaqueBehavior* b;
	string data;
	//default C++
	string opClassName = cl->getName() + "_" + opName;
	//        xmiOperation->getTaggedValue("C++",data);
	//TODO choper contenu data
	if (!data.empty()) {
		if (data.find("{PYTHON}") == 0) {
			data = data.substr(strlen("{PYTHON}"));
#ifndef NOPYTHON
			cerr << "Create Python Opaque behavior" << endl;
			cerr << ":::: " << data << "::::" << endl;
			b = new OpaquePythonBehavior(opClassName);
			b->setBody(data);
#else
			cerr<<"Can't create Python Opaque behavior"<<endl;
			cerr<<"Python module not installed"<<endl;
			b=NULL;
#endif
		} else if (data.find("{OCL}") == 0) {
			data = data.substr(strlen("{OCL}"));
			cerr << "Create Ocl Query behavior" << endl;
			cerr << ":::: " << data << "::::" << endl;
			b = new OclQuery(opClassName);
			b->setBody(data);

		} else {
			_opaqueBehaviorsCode.push_back(make_pair(opClassName, data));
			//b=new OpaqueBehavior(opClassName); b->setBody(opClassName);
			b = new OpaqueBehavior(opName);
			b->setBody(opName);
		}
	} else {
		/*
		 b=new OpaqueBehavior(opClassName);
		 b->setBody(opClassName);
		 */
		b = new OpaqueBehavior(opName);
		b->setBody(opName);
	}
	shared_ptr<Operation> operation = boost::make_shared<Operation>(opName, cl,
			shared_ptr<OpaqueBehavior>(b));
	_addParameters(operation, opNode);
	_idOperations[opNode->getProperty("id")] = operation;
	//cerr << opNode->getProperty("id") << endl;
	//TODO : normalement a devrait etre fait par le new Operation
	cl->addOperation(operation);
	operation->setDescription(getComment(opNode));
	operation->setSummary(getSummary(opNode));
	operation->setTags(getTags(opNode));
	// Bouml preserved body end 0001FAE7
}

void ModelLoader2::addAttribute(const shared_ptr<XmlNode> & attrNode,
		const shared_ptr<Class> & cl) {
	// Bouml preserved body begin 0001FB67

	shared_ptr<XmlNode> typeNode = attrNode->getChildByName("type");

	if (isStereotypedBuiltIn(attrNode)) {//TODO Pour le moment on ignore les propri�t�s builtIn
		return;
	}
	string assoId;
	if (!attrNode->getPropertyString("association", assoId))
		assoId = attrNode->getProperty("id");
	//add at end of package

	_idAsso[assoId].push_back(pair<shared_ptr<Class> , shared_ptr<XmlNode> > (
			cl, attrNode));
	// Bouml preserved body end 0001FB67
}

shared_ptr<Property> ModelLoader2::_addAttribute(
		const shared_ptr<XmlNode> & attrNode, const shared_ptr<Class> & cl) {
	// Bouml preserved body begin 0001FBE7


	string type, attrName;
	attrNode->getPropertyString("type", type);
	attrNode->getPropertyString("name", attrName);
	bool derived = (attrNode->getProperty("isDerived") == "true");

	shared_ptr<XmlNode> typeNode = attrNode->getChildByName("type");
	shared_ptr<XmlNode> defaultNode = attrNode->getChildByName("defaultValue");
	string strVal = (defaultNode) ? defaultNode->getProperty("value") : "";
	strVal = (defaultNode && strVal == "") ? defaultNode->getProperty("symbol") : "";

	shared_ptr<Classifier> attributeType;
	if (typeNode && typeNode->getProperty("type") == "uml:Class") {
		string typeId = typeNode->getProperty("idref");
		map<string, shared_ptr<Classifier> >::iterator it = _classifiers.find(
				typeId);
		if (it != _classifiers.end())
			attributeType = _classifiers[typeId];
		else {
			boost::to_lower(typeId);
			attributeType = _model->getBasicType(typeId);
		}
	} else if (typeNode && typeNode->getProperty("type") == "uml:PrimitiveType") {
		string href = typeNode->getProperty("href");
		string strType = href.substr(href.rfind('#') + 1);
		boost::to_lower(strType);
		attributeType = _model->getBasicType(strType);
	} else if (!typeNode) {
		typeNode = attrNode;
		string typeId = typeNode->getProperty2("type");
		map<string, shared_ptr<Classifier> >::iterator it = _classifiers.find(
				typeId);
		if (it != _classifiers.end())
			attributeType = _classifiers[typeId];
		else {
			boost::to_lower(typeId);
			attributeType = _model->getBasicType(typeId);
		}
	}

	shared_ptr<ValueSpecification> value;
	if (derived) {
		value = boost::make_shared<Expression>(strVal, attributeType);
	} else if (strVal != "") {
		value = attributeType->createValueFromString(strVal);
	}

	shared_ptr<Property> attr = boost::make_shared<Property>(attrName, cl.get(), attributeType,
	//opposite
			shared_ptr<Property> (), value //default value
			);

	/* Multiplicity */
	string mulStr = "1";
	shared_ptr<XmlNode> mulNode = attrNode->getChildByName("upperValue");
	if (mulNode) {
		mulStr = mulNode->getProperty("value");
		if (mulStr == "*")
			mulStr = "-1";
		if (mulStr == "")
			mulStr = "1";
	}
	attr->setMaximumNumber(boost::lexical_cast<int>(mulStr));

	mulNode = attrNode->getChildByName("lowerValue");
	mulStr = "1";
	if (mulNode) {
		mulStr = mulNode->getProperty("value");
		if (mulStr == "*")
			mulStr = "-1";
		if (mulStr == "")
			mulStr = "0";
	}
	attr->setMinimumNumber(boost::lexical_cast<int>(mulStr));
	/* 	*/
	//                                                //derived
	attr->setIsDerived(derived);

	cl->addAttribute(attr);
	attr->setDescription(getComment(attrNode));
	attr->setSummary(getSummary(attrNode));
	attr->setTags(getTags(attrNode));

	return attr;

	// Bouml preserved body end 0001FBE7
}

void ModelLoader2::addConditions(void) {
	for (unsigned int iCondition = 0; iCondition < _constraints.size(); iCondition++) {
		ConstraintT constraint = _constraints[iCondition];
		std::set<string>::iterator itPre = _stereoPreconditions.find(
				constraint.id);
		if (itPre != _stereoPreconditions.end()) {
			shared_ptr<Operation> operation;
			map<string, shared_ptr<Operation> >::iterator itOper =
					_idOperations.find(constraint.constrainedElementID);
			if (itOper != _idOperations.end()) {
				operation = _idOperations[constraint.constrainedElementID];
				shared_ptr<Expression> expression = boost::make_shared<Expression>(
						constraint.specification, _model->getBasicType(
								"boolean"));
				shared_ptr<Constraint> pre = boost::make_shared<Constraint>();
				pre->setSpecification(expression);
				operation->addPrecondition(pre);
				cerr << " PRECONDITION : " << constraint.specification
						<< " FOR : " << operation->getFullName() << endl;
			}
		}
		std::set<string>::iterator itPost = _stereoPostconditions.find(
				constraint.id);
		if (itPost != _stereoPostconditions.end()) {
			shared_ptr<Operation> operation;
			map<string, shared_ptr<Operation> >::iterator itOper =
					_idOperations.find(constraint.constrainedElementID);
			if (itOper != _idOperations.end()) {
				operation = _idOperations[constraint.constrainedElementID];
				shared_ptr<Expression> expression = boost::make_shared<Expression>(
						constraint.specification, _model->getBasicType(
								"boolean"));
				shared_ptr<Constraint> pre = boost::make_shared<Constraint>();
				pre->setSpecification(expression);
				operation->addPostcondition(pre);
				cerr << " POSTCONDITION : " << constraint.specification
						<< " FOR : " << operation->getFullName() << endl;
			}
		}
	}
}

//TODO: rajouter multiplicit�
void ModelLoader2::addAssociation() {
	// Bouml preserved body begin 0001FC67


	properties_t::iterator it;
	for (it = _idAsso.begin(); it != _idAsso.end(); it++) {
		if (it->second.size() == 1) {
			shared_ptr<Property> p;
			p = _addAttribute(it->second[0].second, it->second[0].first); //xmlNode of attribute , Class
			p->setIsNavigable(false);
		} else {
			shared_ptr<Property> p1, p2;
			p1 = _addAttribute(it->second[0].second, it->second[0].first); //xmlNode of attribute , Class
			p2 = _addAttribute(it->second[1].second, it->second[1].first); //xmlNode of attribute , class
			p1->setIsNavigable(true);
			p2->setIsNavigable(true);
			p1->setOpposite(p2);
			p2->setOpposite(p1);
		}
	}
	// Bouml preserved body end 0001FC67
}

void ModelLoader2::_addParameters(const shared_ptr<Operation> & op,
		const shared_ptr<XmlNode> & opNode) {
	// Bouml preserved body begin 0001FCE7

	shared_ptr<XmlNode> child = opNode->getFirstChild();
	while (child) {
		if (child->getName() == "ownedParameter") {
			string paramName, paramKind;
			paramName = child->getProperty("name");
			paramKind = child->getProperty("direction");
			string id = child->getProperty("id");
			shared_ptr<Parameter> parameter = boost::make_shared<Parameter>(paramName, paramKind);

			shared_ptr<XmlNode> typeNode = child->getChildByName("type");
			if (typeNode && typeNode->getProperty("type") == "uml:Class") {
				_parameters[id] = parameter;
				_paramToType[id] = typeNode->getProperty("idref"); //set type after
			} else if (typeNode && typeNode->getProperty("type")
					== "uml:PrimitiveType") {
				shared_ptr<Classifier> type;
				string href = typeNode->getProperty("href");
				string strType = href.substr(href.rfind('#') + 1);
				boost::to_lower(strType);
				type = _model->getBasicType(strType);
				parameter->setType(type);
			} else if (!typeNode && child->hasProperty("type")) {
				_parameters[id] = parameter;
				_paramToType[id] = child->getProperty("type"); //set type after
			}
			op->addParameter(parameter);

		}

		child = child->getNext();
	}
	// Bouml preserved body end 0001FCE7
}

void ModelLoader2::_setParametersType() {
	// Bouml preserved body begin 0001FD67


	map<string, string>::iterator it;
	for (it = _paramToType.begin(); it != _paramToType.end(); it++) {
#if GCC_VERSION > 40100

		shared_ptr<Parameter> parameter = _parameters.at(it->first);
		shared_ptr<Classifier> type = _classifiers.at(it->second);
#else
		shared_ptr<Parameter> parameter= MAP_AT(_parameters,it->first);
		shared_ptr<Classifier> type=MAP_AT(_classifiers,it->second);
#endif

		parameter->setType(type);

	}
	// Bouml preserved body end 0001FD67
}

void ModelLoader2::addActivity(const shared_ptr<XmlNode> & activityNode,
		const shared_ptr<Package> &) {
	//cerr << " ###### AddActivity 1 " << endl;
	// Bouml preserved body begin 0001FDE7
	string name = activityNode->getProperty("name");
	string idoperation = activityNode->getProperty("specification");
	shared_ptr<Activity> activity = boost::make_shared<Activity>(name);
	shared_ptr<XmlNode> child = activityNode->getFirstChild();
	while (child) {
		if (child->getName() == "node") //actionNode
			addActivityNode(child, activity);
		else if (child->getName() == "ownedParameter")
			addActivityParameter(child, activity);
		else if (child->getName() == "group")
			addActivityGroup(child, activity);

		child = child->getNext();
	}

	child = activityNode->getFirstChild();
	while (child) {
		if (child->getName() == "edge")
			addActivityEdge(child, activity);
		child = child->getNext();
	}

/*
	vector<shared_ptr<ActivityPartition> > parts = activity->getPartition();
	if (parts.size() == 1)
	 {
	 vector< shared_ptr<ActivityNode> > nodes = activity->getNode();
	 for (unsigned int iNode = 0; iNode < nodes.size(); iNode ++)
	 {
	 nodes[iNode]->addPartition(parts[0]);
	 parts[0]->addNode(nodes[iNode]);
	 }
	 }
	 else if (parts.size() >= 1)
	 {
	 vector< shared_ptr<ActivityNode> > nodes = activity->getNode();
	 for (unsigned int iPart = 0;iPart<parts.size();iPart++)
	 {
	 for (unsigned int iNode = 0; iNode < nodes.size(); iNode ++)
	 {
	 nodes[iNode]->addPartition(parts[iPart]);
	 parts[iPart]->addNode(nodes[iNode]);
	 }
	 }
	 }
*/

	map<string, shared_ptr<Operation> >::iterator it;
	if ((it = _idOperations.find(idoperation)) != _idOperations.end()) {
		it->second->setMethod(activity);
	} else
		cerr << "[ModelLoader2 Warning] Operation : " << idoperation
		<< " not found for activity " << name << endl;
	//	pkg->addActivity(activity);
	activity->setDescription(getComment(activityNode));
	activity->setSummary(getSummary(activityNode));
	activity->setTags(getTags(activityNode));

	// Bouml preserved body end 0001FDE7
}

void ModelLoader2::addActivityToClass(const shared_ptr<XmlNode> & activityNode,
		const shared_ptr<Class> & cl) {
	// Bouml preserved body begin 0001FDE7
	string name = activityNode->getProperty("name");

	//cerr << " ###### AddActivity 2 " << name << endl;
	bool hasSpecification = false;
	string idoperation;
	if (activityNode->hasProperty("specification")) {
		idoperation = activityNode->getProperty("specification");
		hasSpecification = true;
	}

	shared_ptr<Activity> activity = boost::make_shared<Activity>(name);
	shared_ptr<XmlNode> child = activityNode->getFirstChild();
	while (child) {
		if (child->getName() == "group")
			addActivityGroup(child, activity);
		child = child->getNext();
	}
	child = activityNode->getFirstChild();
	while (child) {
		if (child->getName() == "node") //actionNode
			addActivityNode(child, activity);
		else if (child->getName() == "ownedParameter")
			addActivityParameter(child, activity);
		child = child->getNext();
	}

	child = activityNode->getFirstChild();
	while (child) {
		if (child->getName() == "edge")
			addActivityEdge(child, activity);
		child = child->getNext();
	}

	if (hasSpecification) {
		map<string, shared_ptr<Operation> >::iterator it;
		if ((it = _idOperations.find(idoperation)) != _idOperations.end()) {
			it->second->setMethod(activity);
		} else
			cerr << "[ModelLoader2 Warning] Operation : " << idoperation
			<< " not found for activity " << name << endl;
	} else {
		if (cl->hasOperation(name)) {
			shared_ptr<Operation> op = MAP_AT(cl->getOperations(), name);
			op->setMethod(activity);
		}
	}

	//	pkg->addActivity(activity);
	activity->setDescription(getComment(activityNode));
	activity->setSummary(getSummary(activityNode));
	activity->setTags(getTags(activityNode));
	// Bouml preserved body end 0001FDE7
}

shared_ptr<Activity> ModelLoader2::addActivity(
		const shared_ptr<XmlNode> & activityNode) {
	// Bouml preserved body begin 0001FDE7
	string name = activityNode->getProperty("name");

	//cerr << " ###### AddActivity 3 " << endl;
	shared_ptr<Activity> activity = boost::make_shared<Activity>(name);

	shared_ptr<XmlNode> child = activityNode->getFirstChild();
	while (child) {
		if (child->getName() == "group")
			addActivityGroup(child, activity);

		child = child->getNext();
	}

	child = activityNode->getFirstChild();
	while (child) {
		if (child->getName() == "node") //actionNode
			addActivityNode(child, activity);
		else if (child->getName() == "ownedParameter")
			addActivityParameter(child, activity);

		child = child->getNext();
	}

	child = activityNode->getFirstChild();
	while (child) {
		if (child->getName() == "edge")
			addActivityEdge(child, activity);
		child = child->getNext();
	}

/*
	vector<shared_ptr<ActivityPartition> > parts = activity->getPartition();
	if (parts.size() == 1)
	 {
	 vector< shared_ptr<ActivityNode> > nodes = activity->getNode();
	 for (unsigned int iNode = 0; iNode < nodes.size(); iNode ++)
	 {
	 nodes[iNode]->addPartition(parts[0]);
	 parts[0]->addNode(nodes[iNode]);
	 }
	 }
	 else if (parts.size() >= 1)
	 {
	 vector< shared_ptr<ActivityNode> > nodes = activity->getNode();
	 for (unsigned int iPart = 0;iPart<parts.size();iPart++)
	 {
	 for (unsigned int iNode = 0; iNode < nodes.size(); iNode ++)
	 {
	 nodes[iNode]->addPartition(parts[iPart]);
	 parts[iPart]->addNode(nodes[iNode]);
	 }
	 }
	 }
*/

	//	pkg->addActivity(activity);
	activity->setDescription(getComment(activityNode));
	activity->setSummary(getSummary(activityNode));
	activity->setTags(getTags(activityNode));
	return activity;

	// Bouml preserved body end 0001FDE7
}

//TODO ajout apr�s le reste
void ModelLoader2::addStateMachine(const shared_ptr<XmlNode> & smNode,
		const shared_ptr<Package> &) {
	// Bouml preserved body begin 0001FE67
	
	cerr << "[ModelLoader2 Info] addStateMachine" << endl;

	string name = smNode->getProperty("name");
	string specif = smNode->getProperty("specification");
	map<string, shared_ptr<Operation> >::iterator it;

	shared_ptr<StateMachine> machine = boost::make_shared<StateMachine>(name);
	machine->setDescription(getComment(smNode));
	machine->setSummary(getSummary(smNode));
	machine->setTags(getTags(smNode));
	if ((it = _idOperations.find(specif)) != _idOperations.end()) {
		it->second->setMethod(machine);
		it->second->getClassifier()->addOwnedBehavior(machine);
		machine->setOwner(it->second->getClassifier());
	}

	map<string, shared_ptr<Vertex> > vertices;
	shared_ptr<XmlNode> child = smNode->getFirstChild();
	while (child) {
		if (child->getName() == "region") {
			shared_ptr<Region> region = boost::make_shared<Region>();
			machine->addRegion(region);
			addMachineRegion(child, machine, region, vertices);
		} else if (child->getName() == "transition")
			addMachineTransition(child, machine, vertices);
		child = child->getNext();
	}

	// Bouml preserved body end 0001FE67
}

void ModelLoader2::addStateMachineToClass(const shared_ptr<XmlNode> & smNode,
		const shared_ptr<Class> & classe) {
	// Bouml preserved body begin 0001FE67
	cerr << "[ModelLoader2 Info] addStateMachineToClass (SM: ";

	string name = smNode->getProperty("name");

	cerr << name << " Class: " << classe->getFullName() << ")" << endl;

	shared_ptr<StateMachine> machine = boost::make_shared<StateMachine>(name);
	machine->setDescription(getComment(smNode));
	machine->setSummary(getSummary(smNode));
	machine->setTags(getTags(smNode));

	// SubMachine ?
	if (smNode->hasProperty("submachineState")) {
		string subMachineRef = smNode->getProperty("id");
		_subStateMachines[subMachineRef] = machine;
		//cerr << "SUB STATE MACHINE" << endl;
	} else {
		classe->addOwnedBehavior(machine);
		machine->setOwner(classe);
	}

	map<string, shared_ptr<Vertex> > vertices;
	shared_ptr<XmlNode> child = smNode->getFirstChild();
	while (child) {
		if (child->getName() == "region") {
			shared_ptr<Region> region = boost::make_shared<Region>();
			machine->addRegion(region);
			addMachineRegion(child, machine, region, vertices);
		} else if (child->getName() == "transition")
			addMachineTransition(child, machine, vertices);
		child = child->getNext();
	}

	// Bouml preserved body end 0001FE67
}

void ModelLoader2::addGeneralizations() {
	// Bouml preserved body begin 0001FEE7
	for (size_t i = _toGeneralize.size(); i--;) // vec( pair (id child, id parent) )
		_idClass[_toGeneralize[i].second]->addChild(
				_idClass[_toGeneralize[i].first]);

	// Bouml preserved body end 0001FEE7
}

void ModelLoader2::addDatatype(const shared_ptr<XmlNode> & dtNode) {
	// Bouml preserved body begin 0001FF67

	string name = dtNode->getProperty("name");

	string id = dtNode->getProperty("id");
	shared_ptr<PrimitiveType> type = boost::make_shared<PrimitiveType>(name);
	_model->addBasicType(type);
	//On recupere ceux qui existe dj
	type = _model->getBasicType(name);
	//_basicTypes[id] = type;
	_classifiers[id] = type;
	type->setDescription(getComment(dtNode));
	type->setSummary(getSummary(dtNode));
	type->setTags(getTags(dtNode));

	// Bouml preserved body end 0001FF67
}

void ModelLoader2::addStereotypes() {
	// Bouml preserved body begin 00020167

	shared_ptr<XmlNode> child = _xmi->getFirstChild();
	while (child) {
		string name = child->getName();
		if (name == "Agent") {
			_stereoAgents.insert(child->getProperty("base_Element"));
		} else if (name == "VirtualHuman") {
			if (child->hasProperty("base_Class"))
				_stereoVirtualHumans.insert(child->getProperty("base_Class"));
			else
				_stereoVirtualHumans.insert(child->getProperty("base_Element"));
		} else if (name == "Entity") {
			if (child->hasProperty("base_Class"))
				_stereoEntities.insert(child->getProperty("base_Class"));
			else
				_stereoEntities.insert(child->getProperty("base_Element"));
		} else if (name == "Role") {
			if (child->hasProperty("base_Interface"))
				_stereoRoles.insert(child->getProperty("base_Interface"));
			else
				_stereoRoles.insert(child->getProperty("base_Element"));
		} else if (name == "BuiltIn")
			_stereoBuiltIn.insert(child->getProperty("base_Element"));
		else if (name == "NonInterupt")
			_stereoNonInterupt.insert(child->getProperty("base_Element"));
		else if (name == "precondition")
			_stereoPreconditions.insert(child->getProperty("base_Constraint"));
		else if (name == "postcondition")
			_stereoPostconditions.insert(child->getProperty("base_Constraint"));
		else if (name == "Block")
		{
			_stereoBlocks.insert(child->getProperty("base_Class"));
		}

		child = child->getNext();
	}
	// Bouml preserved body end 00020167
}

//FinalNode, Merge et Decision devraent �tre des activityNode !!

void ModelLoader2::addActivityNode(const shared_ptr<XmlNode> & node,
		shared_ptr<Activity> & activity) {
	// Bouml preserved body begin 000205E7
	string type = node->getProperty("type");
	string name = node->getProperty("name");
	string id = node->getProperty("id");
	string idPartition = node->getProperty("inPartition");

	shared_ptr<ActivityNode> actNode;
	//control
	if (type == "uml:InitialNode") {
		actNode = boost::make_shared<InitialNode>();
		activity->setInitialNode(actNode);
	} else if (type == "uml:FinalNode" || type == "uml:ActivityFinalNode") {
		actNode = boost::make_shared<FinalNode>();
	} else if (type == "uml:LoopNode") {
		cerr << "|||||2 Check 'inPartition' for loop node: " << idPartition << endl;
		addActivityGroup(node, activity);
	} else if (type == "uml:ForkNode") {
		actNode = boost::make_shared<ForkNode>();
	} else if (type == "uml:JoinNode") {
		actNode = boost::make_shared<JoinNode>();
	} else if (type == "uml:MergeNode") {
		actNode = boost::make_shared<MergeNode>();
	} else if (type == "uml:DecisionNode") {
		actNode = boost::make_shared<DecisionNode>();
	} else if (type == "uml:ObjectNode" || type == "uml:CentralBufferNode") {
		shared_ptr<ObjectNode> objNode = boost::make_shared<ObjectNode>(name);
		shared_ptr<Classifier> ressourceType = _getObjectNodeType(node);
		if(ressourceType)
			objNode->setResourceType(ressourceType);
		actNode = objNode;
                string type2 = node->getProperty2("type");
                map<string, shared_ptr<Class> >::iterator itC = _idClass.find(type2);
                if (itC != _idClass.end()) 
                {       
                        (shared_dynamic_cast<ObjectNode>(actNode))->setResourceType(itC->second);
                } 
	}
	/* TODO
	 else if (type=="uml:OpaqueAction")
	 {
	 }
	 else if (type=="uml:AcceptEventAction")
	 {
	 }
	 else if (type=="uml:TriggerAction")
	 {
	 }
	 else if (type=="uml:ClearVariableAction")
	 {
	 }
	 else if (type=="uml:WriteVariableAction")
	 {
	 }
	 else if (type=="uml:AddVariableValueAction")
	 {
	 }
	 else if (type=="uml:RemoveVariableValueAction")
	 {
	 } */
	else if (type == "uml:ValueSpecificationAction") {
		shared_ptr<XmlNode> valueNode = node->getChildByName("value");
		string expressionValue = valueNode->getProperty("value");
		shared_ptr<Expression> expression = boost::make_shared<Expression>(
				expressionValue, _model->getBasicType("boolean"));
		_activityExpressions[node->getProperty("id")] = expression;
	}
	else if (type == "uml:CallBehaviorAction")
	{
		shared_ptr<ActionNode> an = boost::make_shared<ActionNode>(name);
		shared_ptr<CallBehaviorAction> cb = boost::make_shared<CallBehaviorAction>();
		shared_ptr<XmlNode> beNode = node->getChildByName("behavior");

		string opid = beNode ? beNode->getProperty("idref") : node->getProperty("behavior");

		if (_idBehaviors.find(opid) != _idBehaviors.end())
		{
			cb->setBehavior(_idBehaviors[opid]);
		}
		else
		{
			cerr << "[ModelLoader2 Info] Behavior " << opid
					<<" not yet found for ActionNode " << name << ". Postbone..." << endl;
			_callBehaviors[cb] = opid;
		}

		an->setAction(cb);
		addPins(an,node);
		actNode = an;
	}
	else if (type == "uml:CallOperationAction")
	{
		shared_ptr<ActionNode> an = boost::make_shared<ActionNode>(name);
		shared_ptr<CallOperationAction> act = boost::make_shared<CallOperationAction>();
		shared_ptr<XmlNode> opNode = node->getChildByName("operation");
		if (opNode) {
			string opid;
			opid = opNode->getProperty("idref");
			if (_idOperations.find(opid) != _idOperations.end())
				act->setOperation(_idOperations[opid]);
			else {
				//cerr << "ERREUR PAS OPERATION... " << opid<< endl;
				_callOperations[act] = opid;
			}
		} else {
			string opid;
			opid = node->getProperty("operation");
			if (_idOperations.find(opid) != _idOperations.end())
				act->setOperation(_idOperations[opid]);
			else {
				//cerr << "ERREUR PAS OPERATION... " << opid<< endl;
				_callOperations[act] = opid;
			}
		}
		an->setAction(act);
		addPins(an,node);
		actNode = an;
	} else if (type == "uml:SendSignalAction") {
		shared_ptr<ActionNode> an = boost::make_shared<ActionNode>(name);
		shared_ptr<SendSignalAction> act = boost::make_shared<SendSignalAction>();
		shared_ptr<XmlNode> sigNode = node->getChildByName("signal");
		if (sigNode) {
			string signame = sigNode->getProperty("name");
			if (_signals.find(signame) == _signals.end())
				_signals[signame] = boost::make_shared<Signal>(signame);
			else
				cerr << " Signal : " << signame << " not found" << endl;
			act->setSignalClass(_signals[signame]);
		} else {
			string signame = node->getProperty("signal");
			if (_signals.find(signame) == _signals.end())
				_signals[signame] = boost::make_shared<Signal>(signame);
			else {
				;
			}//cerr << " Signal : " << signame << " found" << endl;
			act->setSignalClass(_signals[signame]);
		}

		shared_ptr<XmlNode> targetNode = node->getChildByName("argument");
		if (targetNode) {
			string targetName = targetNode->getProperty("name");
			shared_ptr<XmlNode> typeNode = targetNode->getChildByName("type");
			string classid = typeNode->getProperty("idref");
			act->setTargetName(targetName);
			act->setTargetClass(_idClass[classid]);
		}
		an->setAction(act);
		addPins(an,node);
		actNode = an;
	} else if (type == "uml:AddStructuralFeatureValueAction") {
		//cerr << " ############# Found a AddStructuralFeatureValueAction : " << endl;

		shared_ptr<ActionNode> an = boost::make_shared<ActionNode>(name);
		shared_ptr<Classifier> type;
		shared_ptr<XmlNode> pinNode = node->getChildByName("object");

		string attrName = pinNode->getProperty("name");

		cerr << " Name == " << attrName << endl;

		shared_ptr<XmlNode> typeNode = pinNode->getChildByName("type");
		if (typeNode && typeNode->getProperty("type") == "uml:PrimitiveType") {
			string href = typeNode->getProperty("href");
			string strType = href.substr(href.rfind('#') + 1);
			boost::to_lower(strType);
			type = _model->getBasicType(strType);
		}

		shared_ptr<XmlNode> valueNode = pinNode->getChildByName("value");
		if (valueNode) {
			string value = valueNode->getProperty("value");
			cerr << "    VALUE : " << value << endl;
			shared_ptr<Expression> expression = boost::make_shared<Expression>(value, type);
			shared_ptr<AddStructuralFeatureValueAction> act =
					boost::make_shared<AddStructuralFeatureValueAction>();
			act->setValue(expression);
			act->setProperty(attrName);
			an->setAction(act);
		}
		actNode = an;

	} else if (type == "uml:AcceptEventAction") {
		shared_ptr<ActionNode> an = boost::make_shared<ActionNode>(name);
		shared_ptr<AcceptEventAction> act= boost::make_shared<AcceptEventAction>();

		shared_ptr<XmlNode> triggerNode = node->getChildByName("trigger");
		if (triggerNode) {
			shared_ptr<Trigger> trigger = boost::make_shared<Trigger>(node->getProperty("name"));
			trigger->setId(triggerNode->getProperty("id"));

			if (triggerNode->hasProperty("event")) {
				string idEvent = triggerNode->getProperty("event");

				map<string, shared_ptr<Event> >::iterator it = _events.find(
						idEvent);
				if (it != _events.end()) {
					trigger->setEvent(_events[idEvent]);
					act->setTrigger(trigger);
				} else
					cerr << "Could not find event : " << idEvent << endl;
			}
		}
		an->setAction(act);
		addPins(an,node);
		actNode = an;
	}
	// ...
	else {
		cerr << "Unknown state: " << type << endl;
	}

	if (actNode) {
		_activityNodes[id] = actNode;
		activity->addNode(actNode);
		//cerr << "searching _partitions for '" << idPartition << "'" << endl;
		map<string, shared_ptr<ActivityPartition> >::iterator it = _partitions.find(idPartition);
		if (it != _partitions.end())
		{
			actNode->addPartition(it->second);
			it->second->addNode(actNode);
			
			//cerr << " >>> >>> adding partition " << it->second << "(id = " << idPartition << ") to node " << actNode << endl;
		}
		else {
			//_waitingNodes[id] = idPartition;
		}
		actNode->setDescription(getComment(node));
		actNode->setSummary(getSummary(node));
		actNode->setTags(getTags(node));
	}

	// Bouml preserved body end 000205E7
}

void ModelLoader2::addActivityNode(const shared_ptr<XmlNode> & node,
		shared_ptr<LoopNode> & loopNode, shared_ptr<Activity> & activity) {
	// Bouml preserved body begin 000205E7
	string type = node->getProperty("type");
	string name = node->getProperty("name");
	string id = node->getProperty("id");
	
	/*
	//recreate partitions
	string loopPartitions = "";
	for (int i=0; i<loopNode->getPartition().size(); ++i)
	{
		if (i>0) loopPartitions += " ";
		loopPartitions += loopNode->getPartition()[i]->getId();
		cerr << "... HACK PARTITION ..." << endl;
	}
	*/
	string idPartition = node->getProperty("inPartition");
	
	cerr << "|| addActivityNode() with type " << type << endl;

	shared_ptr<ActivityNode> actNode;
	//control
	if (type == "uml:InitialNode") { cerr << "uml:InitialNode" << endl;
		actNode = boost::make_shared<InitialNode>();
		loopNode->setInitialNode(actNode);
	} else if (type == "uml:FinalNode" || type == "uml:ActivityFinalNode") { cerr << "uml:FinalNode or uml:ActivityFinalNode" << endl;
		actNode = boost::make_shared<FinalNode>();
	} else if (type == "uml:LoopNode") {
		cerr << "||||| Check 'inPartition' for loop node: " << idPartition << endl;
		addActivityGroup(node, activity);
	} else if (type == "uml:ForkNode") { cerr << "uml:ForkNode" << endl;
		actNode = boost::make_shared<ForkNode>();
	} else if (type == "uml:JoinNode") { cerr << "uml:JoinNode" << endl;
		actNode = boost::make_shared<JoinNode>();
	} else if (type == "uml:MergeNode") { cerr << "uml:MergeNode" << endl;
		actNode = boost::make_shared<MergeNode>();
	} else if (type == "uml:DecisionNode") { cerr << "uml:DecisionNode" << endl;
		actNode = boost::make_shared<DecisionNode>();
	} else if (type == "uml:ObjectNode" || type == "uml:CentralBufferNode") { cerr << "uml:ObjectNode" << endl;
		actNode = boost::make_shared<ObjectNode>(name);
		string type2 = node->getProperty2("type");
		map<string, shared_ptr<Class> >::iterator itC = _idClass.find(type2);
		if (itC != _idClass.end()) 
		{
			(shared_dynamic_cast<ObjectNode>(actNode))->setResourceType(itC->second);
		} 
	}
	/* TODO
	 else if (type=="uml:CallBehaviorAction")
	 {
	 }
	 else if (type=="uml:OpaqueAction")
	 {
	 }
	 else if (type=="uml:AcceptEventAction")
	 {
	 }
	 else if (type=="uml:TriggerAction")
	 {
	 }
	 else if (type=="uml:ClearVariableAction")
	 {
	 }
	 else if (type=="uml:WriteVariableAction")
	 {
	 }
	 else if (type=="uml:AddVariableValueAction")
	 {
	 }
	 else if (type=="uml:RemoveVariableValueAction")
	 {
	 } */
	else if (type == "uml:AcceptEventAction") {
		shared_ptr<ActionNode> an = boost::make_shared<ActionNode>(name);
		shared_ptr<AcceptEventAction> act = boost::make_shared<AcceptEventAction>();

		shared_ptr<XmlNode> triggerNode = node->getChildByName("trigger");
		if (triggerNode) {
			shared_ptr<Trigger> trigger = boost::make_shared<Trigger>(
					node->getProperty("name"));
			trigger->setId(triggerNode->getProperty("id"));

			if (triggerNode->hasProperty("event")) {
				string idEvent = triggerNode->getProperty("event");

				map<string, shared_ptr<Event> >::iterator it = _events.find(
						idEvent);
				if (it != _events.end()) {
					trigger->setEvent(_events[idEvent]);
					act->setTrigger(trigger);
				} else
					cerr << "Could not find event : " << idEvent << endl;
			}
		}

		an->setAction(act);
		actNode = an;
	} else if (type == "uml:ValueSpecificationAction") {
		shared_ptr<XmlNode> valueNode = node->getChildByName("value");
		string expressionValue = valueNode->getProperty("value");
		shared_ptr<Expression> expression = boost::make_shared<Expression>(
				expressionValue, _model->getBasicType("boolean"));
		_activityExpressions[node->getProperty("id")] = expression;
	} else if (type == "uml:CallOperationAction") {
		shared_ptr<ActionNode> an = boost::make_shared<ActionNode>(name);
		shared_ptr<CallOperationAction> act = boost::make_shared<CallOperationAction>();
		shared_ptr<XmlNode> opNode = node->getChildByName("operation");
		if (opNode) {
			string opid;
			opid = opNode->getProperty("idref");
			if (_idOperations.find(opid) != _idOperations.end())
				act->setOperation(_idOperations[opid]);
			else {
				//cerr << "ERREUR PAS OPERATION... " << opid<< endl;
				_callOperations[act] = opid;
			}
		} else {
			string opid;
			opid = node->getProperty("operation");
			if (_idOperations.find(opid) != _idOperations.end())
				act->setOperation(_idOperations[opid]);
			else {
				//cerr << "ERREUR PAS OPERATION... " << opid<< endl;
				_callOperations[act] = opid;
			}
		}
		an->setAction(act);
		addPins(an,node);
		actNode = an;
	} else if (type == "uml:CallBehaviorAction") {
		shared_ptr<ActionNode> an = boost::make_shared<ActionNode>(name);
		shared_ptr<CallBehaviorAction> cb = boost::make_shared<CallBehaviorAction>();
		shared_ptr<XmlNode> opNode = node->getChildByName("behavior");
		if (opNode) {
			string opid;
			opid = opNode->getProperty("idref");
			if (_idBehaviors.find(opid) != _idBehaviors.end())
				cb->setBehavior(_idBehaviors[opid]);
			else {
				//cerr << "No such behavior... " << opid<< endl;
				_callBehaviors[cb] = opid;
			}
		} else {
			string opid;
			opid = node->getProperty("behavior");
			if (_idBehaviors.find(opid) != _idBehaviors.end())
				cb->setBehavior(_idBehaviors[opid]);
			else {
				//cerr << "No such behavior... " << opid<< endl;
				_callBehaviors[cb] = opid;
			}
		}
		an->setAction(cb);
		addPins(an,node);
		actNode = an;
	} else if (type == "uml:SendSignalAction") {
		shared_ptr<ActionNode> an = boost::make_shared<ActionNode>(name);
		shared_ptr<SendSignalAction> act = boost::make_shared<SendSignalAction>();
		shared_ptr<XmlNode> sigNode = node->getChildByName("signal");
		if (sigNode) {
			string signame = sigNode->getProperty("name");
			if (_signals.find(signame) == _signals.end())
				_signals[signame] = boost::make_shared<Signal>(signame);
			else
				cerr << " Signal : " << signame << " not found" << endl;
			act->setSignalClass(_signals[signame]);
		} else {
			string signame = node->getProperty("signal");
			if (_signals.find(signame) == _signals.end())
				_signals[signame] = boost::make_shared<Signal>(signame);
			act->setSignalClass(_signals[signame]);
		}

		shared_ptr<XmlNode> targetNode = node->getChildByName("argument");
		if (targetNode) {
			string targetName = targetNode->getProperty("name");
			shared_ptr<XmlNode> typeNode = targetNode->getChildByName("type");
			string classid = typeNode->getProperty("idref");
			act->setTargetName(targetName);
			act->setTargetClass(_idClass[classid]);
		}

		an->setAction(act);
		actNode = an;
	} else if (type == "uml:AddStructuralFeatureValueAction") {
		shared_ptr<ActionNode> an = boost::make_shared<ActionNode>(name);
		shared_ptr<Classifier> type;
		shared_ptr<XmlNode> pinNode = node->getChildByName("object");

		string attrName = pinNode->getProperty("name");

		shared_ptr<XmlNode> typeNode = pinNode->getChildByName("type");
		if (typeNode && typeNode->getProperty("type") == "uml:PrimitiveType") {
			string href = typeNode->getProperty("href");
			string strType = href.substr(href.rfind('#') + 1);
			boost::to_lower(strType);
			type = _model->getBasicType(strType);
		}

		shared_ptr<XmlNode> valueNode = pinNode->getChildByName("value");
		if (valueNode) {
			string value = valueNode->getProperty("value");
			shared_ptr<Expression> expression = boost::make_shared<Expression>(value, type);
			shared_ptr<AddStructuralFeatureValueAction> act =
					boost::make_shared<AddStructuralFeatureValueAction>();
			act->setValue(expression);
			act->setProperty(attrName);
			an->setAction(act);
		}

		actNode = an;

	}
	// ...
	else {
		cerr << "Unknown state: " << type << endl;
	}
	if (actNode) {
		cerr << "<><><><> ENTERING INTERESTING CODE <><><><>" << endl;
	
		_activityNodes[id] = actNode;
		
		loopNode->addNode(actNode);
		activity->addNode(actNode);
		map<string, shared_ptr<ActivityPartition> >::iterator it =
				_partitions.find(idPartition);
		cerr << "mapSize: " << _partitions.size() << endl;
		if (it != _partitions.end()) {
			actNode->addPartition(it->second);
			it->second->addNode(actNode);
		} else {
                       shared_ptr<ActivityPartition> part = loopNode->getPartition()[0];
                       actNode->addPartition(part);
                       part->addNode(actNode);
		}

		actNode->setDescription(getComment(node));
		actNode->setSummary(getSummary(node));
		actNode->setTags(getTags(node));
	}

	// Bouml preserved body end 000205E7
}

//TODOpedagogiq
void ModelLoader2::addActivityParameter(const shared_ptr<XmlNode> & paramNode,
		shared_ptr<Activity> & activity) {
	// Bouml preserved body begin 00020667

	//cerr << " ################### PARAMETERS " << endl;

	string type, attrName;
	paramNode->getPropertyString("name", attrName);

	shared_ptr<XmlNode> typeNode = paramNode->getChildByName("type");

	shared_ptr<Classifier> attributeType;
	if (typeNode && typeNode->getProperty("type") == "uml:Class") {
		string typeId = typeNode->getProperty("idref");
		map<string, shared_ptr<Classifier> >::iterator it = _classifiers.find(
				typeId);
		if (it != _classifiers.end())
			attributeType = _classifiers[typeId];
		else {
			boost::to_lower(typeId);
			attributeType = _model->getBasicType(typeId);
		}
	} else if (typeNode && typeNode->getProperty("type") == "uml:PrimitiveType") {
		cerr << " TYPE ICI" << endl;
		string href = typeNode->getProperty("href");
		string strType = href.substr(href.rfind('#') + 1);
		boost::to_lower(strType);
		attributeType = _model->getBasicType(strType);
	} else if (!typeNode) {
		typeNode = paramNode;
		string typeId = typeNode->getProperty2("type");
		map<string, shared_ptr<Classifier> >::iterator it = _classifiers.find(
				typeId);
		if (it != _classifiers.end())
			attributeType = _classifiers[typeId];
		else {
			if (typeId != "") {
				boost::to_lower(typeId);
				attributeType = _model->getBasicType(typeId);
			}
		}
	}

	//cerr << " Adding Parameter : " << attrName << " : " << attributeType->getName() << " dans " << activity->getName() << endl;
	shared_ptr<ActivityParameterNode> parameterNode =
			boost::make_shared<ActivityParameterNode>(attrName);
	shared_ptr<Parameter> parameter = boost::make_shared<Parameter>(attrName, "read");
	parameterNode->setParameter(parameter);
	parameter->setType(attributeType);
	activity->addNode(parameterNode);

	/*
	 attr->setDescription(getComment(paramNode));
	 attr->setSummary(getSummary(paramNode));
	 attr->setTags(getTags(paramNode));
	 */

	// Bouml preserved body end 00020667
}

//TODO: containedEdge avec des ObjectFlow
//Gerer diff entre ObjectFlow et ControlFlow
void ModelLoader2::addActivityEdge(const shared_ptr<XmlNode> & node,
		shared_ptr<Activity> & activity) {
	// Bouml preserved body begin 000206E7
	string type = node->getProperty("type"); // "uml:ObjectFlow" "uml:ControlFlow"
	string sourceid = node->getProperty("source");
	string targetid = node->getProperty("target");
	string name = node->getProperty("name");

	if(type =="uml:ObjectFlow")
	{
		shared_ptr<ActivityNode> source;
		shared_ptr <ActivityNode> target;

		if(_activityNodes.find(sourceid) != _activityNodes.end())
			source = _activityNodes[sourceid];
		else if (_objectNodes.find(sourceid) != _objectNodes.end())
		{
			source = shared_dynamic_cast<ActivityNode>(_objectNodes[sourceid]);
		}
		else
		{
			cerr << "edge Source not found: " << sourceid << endl;
			return;
		}
		if(_activityNodes.find(targetid) != _activityNodes.end())
			target = _activityNodes[targetid];
		else if (_objectNodes.find(targetid) != _objectNodes.end())
		{
			target = shared_dynamic_cast<ActivityNode>(_objectNodes[targetid]);
		}
		else
		{
			cerr << "[ModelLoader2 Warning] edge Target not found: " << targetid;
			cerr << " from " << sourceid << " " << source->getFullName() << endl;
			return;
		}

		//cout << "ObjectFlow is composed of a source "<<sourceid << " " <<source->getName()<< " and a target "<<targetid<<" "<<target->getName()<<endl;

		shared_ptr<ObjectFlow> edge = boost::make_shared<ObjectFlow>(name);
		source->addOutgoing(edge);
		target->addIncoming(edge);
		edge->setSource(source);
		edge->setTarget(target);
		activity->addEdge(edge);
		edge->setDescription(getComment(node));
		edge->setSummary(getSummary(node));
		edge->setTags(getTags(node));

	}
	else if (type=="uml:ControlFlow")
	{
		shared_ptr<ActivityNode> source;
		shared_ptr <ActivityNode> target;

		if(_activityNodes.find(sourceid) != _activityNodes.end())
			source = _activityNodes[sourceid];
		else
		{
			cerr << "edge Source not found: " << sourceid << endl;
			return;
		}

		if(_activityNodes.find(targetid) != _activityNodes.end())
			target = _activityNodes[targetid];
		else
		{
			cerr << "[ModelLoader2 Warning] edge Target not found: " << targetid;
			cerr << " from " << sourceid << " " << source->getFullName() << endl;
			return;
		}

		//cout << "ControlFlow is composed of a source "<<source->getName()<< " and a target "<<target->getName()<<endl;

		shared_ptr<ControlFlow> edge = boost::make_shared<ControlFlow>(name);
		source->addOutgoing(edge);
		target->addIncoming(edge);
		edge->setSource(source);
		edge->setTarget(target);
		activity->addEdge(edge);
		edge->setDescription(getComment(node));
		edge->setSummary(getSummary(node));
		edge->setTags(getTags(node));
		
		// guard
		shared_ptr<XmlNode> guardNode;
		guardNode = node->getChildByName("guard"); // Bouml format
		string guardEx;
		if (guardNode)
		{
			cerr << "is guard node" << endl;
			//guardEx = guardNode->getChildByName("specification")->getChildByName("body")->getInnerText();
			guardEx = guardNode->getChildByName("body")->getInnerText();
			shared_ptr<Constraint> guard = boost::make_shared<Constraint>();
			/*
			shared_ptr<OpaqueExpression> exp (new OpaqueExpression());
			guard->setSpecification(exp);
			exp->setBody(guardEx);
			exp->setLanguage("OCL");
			*/
			shared_ptr<Expression> exp = boost::make_shared<Expression>(guardEx, _model->getBasicType("boolean"));
			guard->setSpecification(exp);
			edge->setGuard(guard);
		}
		else // test modelio format
		{
			guardNode = node->getChildByName("ownedRule");
			string guardEx;
			if (guardNode)
			{
				guardEx = guardNode->getChildByName("specification")->getProperty("value");
				shared_ptr<Constraint> guard = boost::make_shared<Constraint>();
				shared_ptr<Expression> exp = boost::make_shared<Expression>(guardEx,
						_model->getBasicType("boolean"));
				guard->setSpecification(exp);
				edge->setGuard(guard);
			}
		}
	}
	else
	{
		cerr <<"This type "<<type<<" is not recognized"<<endl;
		return;
	}


	// activityEdges[id] = edge;
	// Bouml preserved body end 000206E7
}

void ModelLoader2::addActivityEdge(const shared_ptr<XmlNode> & node,
		shared_ptr<LoopNode> & activity) {
	// Bouml preserved body begin 000206E7
	string type = node->getProperty("type"); // "uml:ObjectFlow" "uml:ControlFlow"
	string sourceid = node->getProperty("source");
	string targetid = node->getProperty("target");
	string name = node->getProperty("name");
	if(type =="uml:ObjectFlow")
		{
			shared_ptr<ActivityNode> source;
			shared_ptr <ActivityNode> target;

			if(_activityNodes.find(sourceid) != _activityNodes.end())
				source = _activityNodes[sourceid];
			else if (_objectNodes.find(sourceid) != _objectNodes.end())
			{
				source = shared_dynamic_cast<ActivityNode>(_objectNodes[sourceid]);
			}
			else
			{
				cerr << "edge Source not found: " << sourceid << endl;
				return;
			}
			if(_activityNodes.find(targetid) != _activityNodes.end())
				target = _activityNodes[targetid];
			else if (_objectNodes.find(targetid) != _objectNodes.end())
			{
				target = shared_dynamic_cast<ActivityNode>(_objectNodes[targetid]);
			}
			else
			{
				cerr << "[ModelLoader2 Warning] edge Target not found: " << targetid;
				cerr << " from " << sourceid << " " << source->getFullName() << endl;
				return;
			}

			cout << "ObjectFlow is composed of a source "<<sourceid << " " <<source->getName()<< " and a target "<<targetid<<" "<<target->getName()<<endl;

			shared_ptr<ObjectFlow> edge = boost::make_shared<ObjectFlow>(name);
			source->addOutgoing(edge);
			target->addIncoming(edge);
			edge->setSource(source);
			edge->setTarget(target);
			activity->addEdge(edge);
			edge->setDescription(getComment(node));
			edge->setSummary(getSummary(node));
			edge->setTags(getTags(node));

		}
		else if (type=="uml:ControlFlow")
		{
			shared_ptr<ActivityNode> source;
			shared_ptr <ActivityNode> target;

			if(_activityNodes.find(sourceid) != _activityNodes.end())
				source = _activityNodes[sourceid];
			else
			{
				cerr << "edge Source not found: " << sourceid << endl;
				return;
			}

			if(_activityNodes.find(targetid) != _activityNodes.end())
				target = _activityNodes[targetid];
			else
			{
				cerr << "[ModelLoader2 Warning] edge Target not found: " << targetid;
				cerr << " from " << sourceid << " " << source->getFullName() << endl;
				return;
			}

			//cout << "ControlFlow is composed of a source "<<source->getName()<< " and a target "<<target->getName()<<endl;

			shared_ptr<ControlFlow> edge = boost::make_shared<ControlFlow>(name);
			source->addOutgoing(edge);
			target->addIncoming(edge);
			edge->setSource(source);
			edge->setTarget(target);
			activity->addEdge(edge);
			edge->setDescription(getComment(node));
			edge->setSummary(getSummary(node));
			edge->setTags(getTags(node));
		}
		else
		{
			cerr <<"This type "<<type<<" is not recognized"<<endl;
			return;
		}
	// Bouml preserved body end 000206E7
}

//TODO
void ModelLoader2::addActivityGroup(const shared_ptr<XmlNode> & node,
		shared_ptr<Activity> & activity) {
	// Bouml preserved body begin 00020767
	//cerr << " &&&&&&&&&&&&&&&& ADDACTIVITYGROUP : "
	//		<< node->getProperty("type") << endl;

	if (node->getProperty("type") == "uml:ActivityPartition")
	{
		shared_ptr<ActivityPartition> partition = boost::make_shared<ActivityPartition>(node->getProperty("name"));
		_partitions[node->getProperty("id")] = partition;
		activity->addPartition(partition);

/*
		if(_waitingNodes.size()!=0)
		{
			map<string,string>::iterator it;

			for(it=_waitingNodes.begin();it!=_waitingNodes.end();it++)
			{
				if(it->second == node->getProperty("id"))
				{
					_activityNodes[it->first]->addPartition(partition);
					//_waitingNodes.erase(it);
				}
			}
		}
*/
	}
	else if (node->getProperty("type") == "uml:LoopNode")
	{
		shared_ptr<LoopNode> loopNode = boost::make_shared<LoopNode>(node->getProperty("name"));
		loopNode->setDescription(getComment(node));

		activity->addNode(loopNode);

		cerr << " @@@@@@@@@@@@@ LOOP : " << node->getProperty("id") << endl;
		string idPartition = node->getProperty("inPartition");
		map<string, shared_ptr<ActivityPartition> >::iterator itP = _partitions.find(idPartition);
		if (itP != _partitions.end()) {
			loopNode->addPartition(itP->second);
			itP->second->addNode(loopNode);
                }
        
        string isTestedFirst = node->getProperty("isTestedFirst");
        if (isTestedFirst == "true")
        {
        	loopNode->setTestedFirst(true);
        }
        else
        {
        	loopNode->setTestedFirst(false);
       	}
        

		_activityNodes[node->getProperty("id")] = loopNode;
		string testId = node->getProperty("test");
		map<string, shared_ptr<Expression> >::iterator it =
				_activityExpressions.find(testId);
		if (it != _activityExpressions.end()) {
			loopNode->setCondition(it->second);
		} else
			cerr << "Test for LoopNode : " << loopNode->getName()
					<< " not found" << endl;

		shared_ptr<XmlNode> child = node->getFirstChild();
		while (child) {
			if (child->getName() == "node") //actionNode
			{
				string inPart = node->getProperty("inPartition");
				child->setProperty(string("inPartition"), inPart);
				cerr << "././././././././././. transferring property: " << inPart << endl;
				addActivityNode(child, loopNode, activity);
			}
			child = child->getNext();
		}
		child = node->getFirstChild();
		while (child) {
			if (child->getName() == "edge")
				addActivityEdge(child, loopNode);
			child = child->getNext();
		}
	}

	// Bouml preserved body end 00020767
}

string ModelLoader2::getComment(const shared_ptr<XmlNode> & node) {
	// Bouml preserved body begin 00020BE7
	string comment;
	shared_ptr<XmlNode> commentNode = node->getChildByName("ownedComment");
	if (commentNode) {
		comment = commentNode->getProperty("body");
		if (comment == "") {
			commentNode = commentNode->getChildByName("body");
			if (commentNode)
			{
				comment = commentNode->getInnerText();
cerr << " ################################################################################################## "<< endl;
cerr << " ##### " << comment << endl;
cerr << " ################################################################################################## "<< endl;
			}
		}
	}
	return comment;

	// Bouml preserved body end 00020BE7
}

map<string, string> ModelLoader2::getTags(const shared_ptr<XmlNode> & node) {
	string nodeName;
	map<string, string> tags;
	node->getPropertyString("name", nodeName);
	bool found = false;
	shared_ptr<XmlNode> child = node->getFirstChild();
	while (child) {
		string childName = child->getName();
		if (childName == "Extension") {
			shared_ptr<XmlNode> subChild = child->getFirstChild();
			while (subChild) {
				//	bool found= false;
				if (subChild->getName() == "taggedValue" && !found) {
					// std::cout << "processing tagged name  :"<<subChild->getName()<< std::endl;
					string tagName, tagValue;
					subChild->getPropertyString("tag", tagName);
					subChild->getPropertyString("value", tagValue);
					if (tagName != "summary" && tagName != "description") {
						tags[tagName] = tagValue;
					}

				}
				subChild = subChild->getNext();
			}
			//     	 for ( map<string,string>::iterator iter = tags.begin( ); iter != tags.end( );iter++ )
			//      	  		 std::cout<< " tag name : "<< iter->first << " tagValue : " << iter->second << std::endl ;

		}

		child = child->getNext();
	}

	return tags;
}

string ModelLoader2::getSummary(const shared_ptr<XmlNode> & node) {
	string summary = "";
	string nodeName;
	node->getPropertyString("name", nodeName);
	bool found = false;
	shared_ptr<XmlNode> child = node->getFirstChild();
	while (child) {
		string childName = child->getName();
		if (childName == "Extension") {
			shared_ptr<XmlNode> subChild = child->getFirstChild();
			while (subChild) {
				if (subChild->getName() == "taggedValue" && !found) {
					string tagName, tagValue;
					subChild->getPropertyString("tag", tagName);
					subChild->getPropertyString("value", tagValue);
					if (tagName == "summary") {
						summary = tagValue;
					}
				}
				subChild = subChild->getNext();
			}
		}
		child = child->getNext();
	}
	return summary;
}

void ModelLoader2::addMachineRegion(const shared_ptr<XmlNode> & node,
		shared_ptr<StateMachine> & machine, shared_ptr<Region> region, map<
				string, shared_ptr<Vertex> > & vertices) {
	// Bouml preserved body begin 00021067
	shared_ptr<XmlNode> child = node->getFirstChild();
	while (child) {
		if (child->getName() == "subvertex") {
			string type = child->getProperty("type");
			string name = child->getProperty("name");
			string childId = child->getProperty("id");
			if (type == "uml:State") {
				//cerr << "State : " << name << endl;
				shared_ptr<State> state = boost::make_shared<State>(name);
				state->setDescription(getComment(child));
				state->setSummary(getSummary(child));
				state->setTags(getTags(child));
				region->addVertices(state);
				if (child->hasProperty("submachine")) {
					//cerr << "HAS SUBMACHINE" << endl;
					_subStates[childId] = state;
					_subStatesID[childId] = child->getProperty("submachine");
				}
				vertices[childId] = state;

				shared_ptr<XmlNode> vertexChild = child->getFirstChild();
				while (vertexChild) {
					if (vertexChild->getName() == "region") {
						shared_ptr<Region> subregion = boost::make_shared<Region>();
						state->addRegion(subregion);
						addMachineRegion(vertexChild, machine, subregion,
								vertices);
						//cerr << "HAS SUBREGION....." << endl;
					} else if (vertexChild->getName() == "Entry") {
						string
								opEntryName = vertexChild->getChildByName(
										"body")->getInnerText();
						try {
							shared_ptr<Operation>
									op =
											MAP_AT(
													shared_dynamic_cast<Class> (
															machine->getOwner())->getOperations(),
													opEntryName);
							state->setEntryBehavior(op);
						} catch (std::out_of_range&) {
							cerr << "can't found " << opEntryName
									<< " for entry behavior of state " << name
									<< endl;
						}
					} else if (vertexChild->getName() == "Exit") {
						string
								opExitName =
										vertexChild->getChildByName("body")->getInnerText();
						try {
							shared_ptr<Operation>
									op =
											MAP_AT(
													shared_dynamic_cast<Class> (
															machine->getOwner())->getOperations(),
													opExitName);
							state->setExitBehavior(op);
						} catch (std::out_of_range&) {
							cerr << "can't found " << opExitName
									<< " for exit behavior of state " << name
									<< endl;
						}
					} else if (vertexChild->getName() == "Do" || vertexChild->getName() == "doBehavior" || vertexChild->getName() == "doActivity") 
					{
						string opDoName;
                                                shared_ptr<XmlNode> bodyVertexChild = vertexChild->getChildByName("body");
						if (bodyVertexChild) 
						{
						   opDoName = bodyVertexChild->getInnerText();
						   try {
							shared_ptr<Operation> op = MAP_AT( shared_dynamic_cast<Class> ( machine->getOwner())->getOperations(), opDoName);
							state->setDoBehavior(op);
						   } catch (std::out_of_range&) {
							cerr << "can't found " << opDoName << " for do behavior of state " << name << endl;
						   }
						}
						else 
						{
						   string opID = vertexChild->getProperty("specification");
						   if (_idOperations.find(opID) != _idOperations.end())
						   {
							shared_ptr<Operation> op = _idOperations[opID]; 
							state->setDoBehavior(op);
						   }
						}
					}
					vertexChild = vertexChild->getNext();
				}
				if (isStereotypedNonInterupt(child)) {
					state->setNonInterupt(true);
				}
			} else if (type == "uml:Pseudostate") {
				shared_ptr<PseudoState> ps = boost::make_shared<PseudoState>(name);
				ps->setDescription(getComment(child));
				ps->setSummary(getSummary(child));
				ps->setTags(getTags(child));
				region->addVertices(ps);
				machine->addConnectionPoint(ps);
				vertices[childId] = ps;
				string kind; 
				if (!child->hasProperty("kind"))
					kind = child->getProperty("name");
                                else 
					kind = child->getProperty("kind");
				if (kind == "initial" || kind == "Initial" || kind == "Initial State")
					ps->setKind(INITIAL);
				else if (kind == "join")
					ps->setKind(JOIN);
				else if (kind == "fork")
					ps->setKind(FORK);
				else if (kind == "deepHistory")
					ps->setKind(DEEPHISTORY);
				else if (kind == "shallowHistory")
					ps->setKind(SHALLOWHISTORY);
				else if (kind == "choice")
					ps->setKind(CHOICE);
				else if (kind == "junction")
					ps->setKind(JUNCTION);
				else if (kind == "terminate")
					ps->setKind(TERMINATE);
				else if (kind == "exitPoint")
					ps->setKind(EXITPOINT);
				else if (kind == "entryPoint")
					ps->setKind(ENTRYPOINT);
			} else if (type == "uml:Finalstate" || type == "uml:FinalState") {
				shared_ptr<FinalState> fs = boost::make_shared<FinalState>(name);
				fs->setDescription(getComment(child));
				fs->setSummary(getSummary(child));
				fs->setTags(getTags(child));
				region->addVertices(fs);
				//machine->addConnectionPoint(fs);
				vertices[childId] = fs;

			}
		} else if (child->getName() == "transition")
			addMachineTransition(child, machine, vertices);
		child = child->getNext();
	}

	// Bouml preserved body end 00021067
}

//non gestion des triggers
//Attention ! Les Opaque expressions ne sont pas impl�ment� mais sont utilis� pour le moment dans les modelLoader
//(par contre les expressions existe - c'est du ocl)
void ModelLoader2::addMachineTransition(const shared_ptr<XmlNode> & node,
		shared_ptr<StateMachine> & machine,
		map<string, shared_ptr<Vertex> > & vertices) {
	// Bouml preserved body begin 000210E7
	
	//cerr << "##################################" << endl;
	//cerr << "ModelLoader2::addMachineTransition" << endl;

	shared_ptr<Operation> op;

	shared_ptr<Transition> t = boost::make_shared<Transition>();
	string transitionKind = node->getProperty("kind");
	t->setDescription(getComment(node));
	t->setSummary(getSummary(node));
	t->setTags(getTags(node));
	if (transitionKind != "internal") {
		//cerr << "Not internal" << endl;
		shared_ptr<Region> region = machine->getRegion().back();
		region->addTransition(t);
	}

	// Trigger
	shared_ptr<XmlNode> triggerNode;
	triggerNode = node->getChildByName("trigger");
	if (triggerNode) {
		//cerr << "is trigger node" << endl;
		string triggerName = triggerNode->getProperty("name");

		if (triggerNode->hasProperty("event")) {
			//cerr << "has prop event" << endl;
			string eventID = triggerNode->getProperty("event");
			shared_ptr<Event> evt = _events[eventID];
			if (evt) {
				shared_ptr<Trigger> trigger = boost::make_shared<Trigger>(evt->getName());
				trigger->setEvent(evt);
				t->addTrigger(trigger);
			} else
				cerr << "Error : Event not found for trigger : " << triggerName
						<< " in machine " << machine->getFullName() << endl;
		} else {
			shared_ptr<Trigger> trigger = boost::make_shared<Trigger>(triggerName);
			if (boost::algorithm::starts_with(triggerName, "after")) {
				shared_ptr<TimeEvent> timeevent = boost::make_shared<TimeEvent>(triggerName);
				string timeStr = triggerName.substr(6);
				shared_ptr<LiteralReal> time = boost::make_shared<LiteralReal>(timeStr);
				timeevent->setWhen(time);
				trigger->setEvent(timeevent);
			} else {
				//cerr << " ###### Trigger : " << triggerName << endl;
				if (_signals.find(triggerName) == _signals.end())
					_signals[triggerName] = boost::make_shared<Signal>(triggerName);
				shared_ptr<SignalEvent> signalevent = boost::make_shared<SignalEvent>(triggerName);
				trigger->setEvent(signalevent);
				signalevent->setSignalClass(_signals[triggerName]);
			}
			t->addTrigger(trigger);
		}
	}

	// guard
	shared_ptr<XmlNode> guardNode;
	guardNode = node->getChildByName("guard"); // Bouml format
	string guardEx;
	if (guardNode) {
		//cerr << "is guard node" << endl;
		guardEx = guardNode->getChildByName("specification")->getChildByName(
				"body")->getInnerText();
		shared_ptr<Constraint> guard = boost::make_shared<Constraint>();
		/*
		 shared_ptr<OpaqueExpression> exp (new OpaqueExpression());
		 guard->setSpecification(exp);
		 exp->setBody(guardEx);
		 exp->setLanguage("OCL");
		 */
		shared_ptr<Expression> exp = boost::make_shared<Expression>(guardEx,
				_model->getBasicType("boolean"));
		guard->setSpecification(exp);
		t->setGuard(guard);
	} else // test modelio format
	{
		guardNode = node->getChildByName("ownedRule");
		string guardEx;
		if (guardNode) {
			guardEx = guardNode->getChildByName("specification")->getProperty(
					"value");
			shared_ptr<Constraint> guard = boost::make_shared<Constraint>();
			shared_ptr<Expression> exp = boost::make_shared<Expression>(guardEx,
					_model->getBasicType("boolean"));
			guard->setSpecification(exp);
			t->setGuard(guard);
		}
	}

	// Effect
	shared_ptr<XmlNode> effectNode;
	effectNode = node->getChildByName("effect");
	if (effectNode) {
		//cerr << "is effect node" << endl;
		string type = effectNode->getProperty("type");
		if (type == "uml:OpaqueBehavior") {
			if (effectNode->hasProperty("specification")) {
				string opName = effectNode->getProperty("specification");

				op = MAP_AT(_idOperations, opName);
				if (!op)
					cerr << "" << " Operation not found ..." << endl;

				if (transitionKind != "internal" && op) {
					shared_ptr<CallOperationAction> action = boost::make_shared<CallOperationAction>();
					t->setEffect(action);
					action->setOperation(op);
					
					//cerr << "setting internal transition op" << endl;
				}
				
				//cerr << "has prop 'specification'" << endl;
			} else {
				string sigName = effectNode->getProperty("name");
				shared_ptr<Signal> sig;
				map<string, shared_ptr<Signal> >::iterator itSig;
				for (itSig = _signals.begin(); itSig != _signals.end(); itSig++) {
					if (itSig->second->getName() == sigName)
						sig = itSig->second;
				}
				shared_ptr<SendSignalAction> action = boost::make_shared<SendSignalAction>();
				t->setEffect(action);
				action->setSignalClass(sig);
				
				//cerr << "doesn't have prop 'specification'" << endl;
			}
		}
	}

	if (transitionKind != "internal") {
		// Source
		string sourceID = node->getProperty("source");
		map<string, shared_ptr<Vertex> >::iterator itV =
				vertices.find(sourceID);
		if (itV == vertices.end())
			cerr << ""  << " Vertex not found ...(" << sourceID << ")" << endl;
		else {
			shared_ptr<Vertex> vertex = itV->second;
			//TODO en une fois?
			t->setSource(vertex);
			vertex->addOutgoing(t);
		}
		// target
		string targetID = node->getProperty("target");
		itV = vertices.find(targetID);
		if (itV == vertices.end())
			cerr << "" << " Vertex not found ...(" << targetID << ")" << endl;
		else {
			shared_ptr<Vertex> vertex = itV->second;
			t->setTarget(vertex);
			vertex->addIncoming(t);
		}
	} else {
		string sourceID = node->getProperty("source");
		map<string, shared_ptr<Vertex> >::iterator itV =
				vertices.find(sourceID);
		if (itV == vertices.end())
			cerr << "" << " Vertex not found ...(" << sourceID << ")" << endl;
		else {
			shared_ptr<XmlNode> comment = node->getChildByName("ownedComment");
			if (comment) {
				string
						evtType =
								comment->getChildByName("body")->getChildByName(
										"text")->getString();
				shared_ptr<Vertex> vertex = itV->second;
				if (evtType == "DO")
					(shared_dynamic_cast<State> (vertex))->setDoBehavior(op);
				else if (evtType == "ENTRY")
					(shared_dynamic_cast<State> (vertex))->setEntryBehavior(op);
				if (evtType == "EXIT")
					(shared_dynamic_cast<State> (vertex))->setExitBehavior(op);
					
				//cerr << "setting Behavior, with comment.... evtType: " << evtType << endl;
			} else {
				shared_ptr<Vertex> vertex = itV->second;
				(shared_dynamic_cast<State> (vertex))->setDoBehavior(op);
				//cerr << "setting Behavior, no comment..."<< endl;
			}
		}
	}

	// Bouml preserved body end 000210E7
}

void ModelLoader2::addActivitiesAndStateMachines() {
	// Bouml preserved body begin 00021567

	for (size_t i = 0; i < _activities.size(); i++) {
		addActivity(_activities[i].first, _activities[i].second);
	}
	for (size_t i = 0; i < _stateMachines.size(); i++) {
		addStateMachine(_stateMachines[i].first, _stateMachines[i].second);
	}

	map<string, shared_ptr<State> >::iterator it;
	for (it = _subStates.begin(); it != _subStates.end(); it++) {
		shared_ptr<State> state = (*it).second;
		string smid = _subStatesID[(*it).first];
		shared_ptr<StateMachine> machine = _subStateMachines[smid];
		machine->addSubMachineState(state);
		state->setSubMachine(machine);
		//cerr << " ###### " << state->getName() << " : " << machine->getName() << endl;
	}

	// Bouml preserved body end 00021567
}

void ModelLoader2::addCallOperations(void) {
	map<shared_ptr<CallOperationAction> , string>::iterator it;
	for (it = _callOperations.begin(); it != _callOperations.end(); it++) {
		string opid = it->second;
		shared_ptr<CallOperationAction> act = it->first;
		if (_idOperations.find(opid) != _idOperations.end())
			act->setOperation(_idOperations[opid]);
		else
		{
			cerr << "[ModelLoader2 Warning] Operation: " << opid
					<<" not found for ActionNode " << act->getName() << endl;
		}
	}
}

void ModelLoader2::addCallBehaviors(void) {
	map<shared_ptr<CallBehaviorAction> , string>::iterator it;
	for (it = _callBehaviors.begin(); it != _callBehaviors.end(); it++) {
		string opid = it->second;
		shared_ptr<CallBehaviorAction> cb = it->first;
		if (_idBehaviors.find(opid) != _idBehaviors.end())
			cb->setBehavior(_idBehaviors[opid]);
		else
		{
			cerr << "[ModelLoader2 Warning] Behavior: " << opid
					<<" not found for ActionNode " << cb->getName() << endl;
		}
	}
}

void ModelLoader2::addChangeEvent(const shared_ptr<XmlNode> & classNode,
		const shared_ptr<Package> & pkg) {
	string type;

	classNode->getPropertyString("type", type);
	if (type != "uml:ChangeEvent") {
		cerr << "Change Event is a " << type << " and must be a changeEvent"
				<< endl;
		return;
	}
	string id = classNode->getProperty("id");

	shared_ptr<ChangeEvent> changeEvent = boost::make_shared<ChangeEvent>("ChangeEvent");

	if (classNode->getChild("changeExpression")) {
		string changeExp =
				classNode->getChild("changeExpression")->getProperty("value");
		shared_ptr<Expression> exp = boost::make_shared<Expression>(changeExp,
				_model->getBasicType("boolean"));
		changeEvent->setChangeExpression(exp);
	}
	/* add in package*/
	_events[id] = changeEvent;

}

void ModelLoader2::addTimeEvent(const shared_ptr<XmlNode> & classNode,
		const shared_ptr<Package> & pkg) {
	string type;

	classNode->getPropertyString("type", type);
	if (type != "uml:TimeEvent") {
		cerr << "TimeEvent is a " << type << " and must be a timeEvent" << endl;
		return;
	}

	string id = classNode->getProperty("id");

	shared_ptr<TimeEvent> timeEvent = boost::make_shared<TimeEvent>("TimeEvent");

	shared_ptr<XmlNode> when = classNode->getChild("when");
	if (when && when->getChild("expr")) {
		string timeExp = when->getChild("expr")->getProperty("value");
		shared_ptr<LiteralReal> time = boost::make_shared<LiteralReal>(timeExp);
		timeEvent->setWhen(time);

	}
	/* add in package*/
	_events[id] = timeEvent;
}

void ModelLoader2::addPins(shared_ptr<ActionNode> an,shared_ptr<XmlNode> node)
{
	//cout<<"Parsage des pins"<<endl;
	shared_ptr<XmlNode> pins = node->getFirstChild();
	while(pins)
	{
		cout<<"Value pin?";
		if(pins->getName()=="argument")
		{
			cout<<" Yes."<<endl;
			cout<<"Value pins?";
			if(pins->getProperty("type")=="uml:ValuePin")
			{
				cout<<"Yes"<<endl;
				shared_ptr<ValuePin> valuePin = boost::make_shared<ValuePin>();
				valuePin->setId(pins->getProperty("id"));
				valuePin->setName(pins->getProperty("name"));
				shared_ptr<Classifier> ressourceType = _getObjectNodeType(pins);
				if(ressourceType)
					valuePin->setResourceType(ressourceType);

				an->getAction()->addValuePin(valuePin);
				_objectNodes[valuePin->getId()]=valuePin;
			}
			else
			{
				cout<<"No, InputPin"<<endl;
				shared_ptr<InputPin> inputPin  = boost::make_shared<InputPin>();
				inputPin->setId(pins->getProperty("id"));
				inputPin->setName(pins->getProperty("name"));
				shared_ptr<Classifier> ressourceType = _getObjectNodeType(pins);
				if(ressourceType)
					inputPin->setResourceType(ressourceType);

				an->getAction()->addInputPin(inputPin);
				_objectNodes[inputPin->getId()]=inputPin;
			}
			}
			else if(pins->getName()=="result")
			{
			cout<<"No, OutputPin"<<endl;
			shared_ptr<OutputPin> outputPin = boost::make_shared<OutputPin>();
			outputPin->setId(pins->getProperty("id"));
			outputPin->setName(pins->getProperty("name"));
			shared_ptr<Classifier> ressourceType = _getObjectNodeType(pins);
			if(ressourceType)
				outputPin->setResourceType(ressourceType);

			an->getAction()->addOutputPin(outputPin);
			_objectNodes[outputPin->getId()]=outputPin;
		}
		pins = pins->getNext();
	}
}

shared_ptr<Classifier> ModelLoader2::_getObjectNodeType(shared_ptr<XmlNode> node)
{
	shared_ptr<XmlNode> type = node->getChild("type");
	if(type)
	{
		string pinType = type->getProperty("type");
		cerr << "Node have a type, is it a Primitive type ?";
		if(pinType == "uml:PrimitiveType")
		{
			cerr << " Yes" << endl;
			string href = type->getProperty("href");
			string strType = href.substr(href.rfind('#') + 1);
			boost::to_lower(strType);
			return _model->getBasicType(strType);
		}
	}
	else
	{
		cerr << "No primitive type" << endl;
		string idClass = node->getProperty("type");
		map<string, shared_ptr<Classifier> >::iterator it = _classifiers.find(idClass);
		if (it != _classifiers.end())
			return _classifiers[idClass];
		else {
			if (idClass != "") {
				boost::to_lower(idClass);
				return _model->getBasicType(idClass);
			}
		}
	}

	return shared_ptr<Classifier>();
}
