#ifndef _MODELLOADER2_H
#define _MODELLOADER2_H
#include "Tools/veha_plateform.h"
#include "Tools/Loaders/XmlParser.h"
#include "VEHA/Kernel/Model.h"
#include "VEHA/Kernel/Class.h"
#include "VEHA/Kernel/Package.h"
#include "VEHA/Kernel/PrimitiveType.h"
#include "VEHA/Kernel/Parameter.h"
#include "VEHA/Behavior/Common/Trigger.h"
#include "VEHA/Behavior/Activity/Activity.h"
#include "VEHA/Behavior/Activity/LoopNode.h"
#include "VEHA/Behavior/Activity/ActivityPartition.h"
#include "VEHA/Behavior/Common/Signal.h"
#include "VEHA/Behavior/BasicActions/CallOperationAction.h"
#include "VEHA/Behavior/BasicActions/CallBehaviorAction.h"
#include "VEHA/Behavior/StateMachine/StateMachine.h"
#include "VEHA/Behavior/StateMachine/SignalEvent.h"
#include "VEHA/Behavior/StateMachine/CallEvent.h"
#include "VEHA/Behavior/StateMachine/Vertex.h"

#include "BEHAVE/OrganisationalStructure.h"

#include <set>
using namespace VEHA;
using namespace BEHAVE;

typedef struct ConstraintT{
   string id;
   string constrainedElementID;
   string specification;
} constraintT;


class VEHA_API ModelLoader2 {
public:
	ModelLoader2(string str, bool strIsFileName = true);
        void reload(shared_ptr<Class> classe);
        void _reloadAttributes(shared_ptr<Class> classe, shared_ptr<XmlNode> classNode);
        void _reloadOperations(shared_ptr<Class> classe, shared_ptr<XmlNode> classNode);
        void _reloadClass(shared_ptr<Class> classe, shared_ptr<XmlNode> node);

	vector<shared_ptr<OrganisationalStructure> > getOrganisationalStructure();


protected:
	void addPackage(const shared_ptr<XmlNode> & packageNode, const shared_ptr<Package> & parent);

public:
	void addClass(const shared_ptr<XmlNode> & classNode, const shared_ptr<Package> & pkg);
	void addOrganisationalStructure(const shared_ptr<XmlNode> & orgNode);

	void addSignal(const shared_ptr<XmlNode> & signalNode, const shared_ptr<Package> & pkg);
	void addSignalEvent(const shared_ptr<XmlNode> & signalNode, const shared_ptr<Package> & pkg);
	void addCallEvent(const shared_ptr<XmlNode> & callNode, const shared_ptr<Package> & pkg);
	void addTimeEvent(const shared_ptr<XmlNode> & callNode, const shared_ptr<Package> & pkg);

//	void addTimeEvent(const shared_ptr<XmlNode> & signalNode, const shared_ptr<Package> & pkg);
	void addChangeEvent(const shared_ptr<XmlNode> & signalNode, const shared_ptr<Package> & pkg);

	//TODO: choper contenu de data (tag? description?)

	void addOperation(const shared_ptr<XmlNode> & opNode, const shared_ptr<Class> & cl);

	void addAttribute(const shared_ptr<XmlNode> & attrNode, const shared_ptr<Class> & cl);

	shared_ptr<Property> _addAttribute(const shared_ptr<XmlNode> & attrNode, const shared_ptr<Class> & cl);

	//TODO: rajouter multiplicit�
	void addAssociation();
	void addCallOperations();
	void addCallBehaviors();

	void addPins(shared_ptr<ActionNode> an,shared_ptr<XmlNode> node);

	typedef map<string, vector<pair<shared_ptr<Class>, shared_ptr<XmlNode> > > > properties_t;


private:
	properties_t _idAsso;


public:
	void _addParameters(const shared_ptr<Operation> & op, const shared_ptr<XmlNode> & opNode);

	void _setParametersType();

	shared_ptr<Classifier> _getObjectNodeType(shared_ptr<XmlNode> node);

	void addActivity(const shared_ptr<XmlNode> & activityNode, const shared_ptr<Package> & pkg);
	void addActivityToClass(const shared_ptr<XmlNode> & activityNode, const shared_ptr<Class> & cl);
	shared_ptr<Activity> addActivity(const shared_ptr<XmlNode> & activityNode);

	//TODO ajout apr�s le reste
	void addStateMachine(const shared_ptr<XmlNode> & smNode, const shared_ptr<Package> & pkg);
	void addStateMachineToClass(const shared_ptr<XmlNode> & smNode, const shared_ptr<Class> & classe);

	void addGeneralizations();

	void addDatatype(const shared_ptr<XmlNode> & dtNode);

	void addConditions(void);

	inline bool isStereotypedEntity(const shared_ptr<XmlNode> & node);

	inline bool isStereotypedBlock(const shared_ptr<XmlNode> & node);

	inline bool isStereotypedAgent(const shared_ptr<XmlNode> & node);

	inline bool isStereotypedVirtualHuman(const shared_ptr<XmlNode> & node);

	inline bool isStereotypedRole(const shared_ptr<XmlNode> & node);

	inline bool isStereotypedBuiltIn(const shared_ptr<XmlNode> & node);

	inline bool isStereotypedNonInterupt(const shared_ptr<XmlNode> & node);


private:
	std::set<string> _stereoEntities;
	std::set<string> _stereoAgents;
	std::set<string> _stereoVirtualHumans;
	std::set<string> _stereoRoles;
	std::set<string> _stereoPreconditions;
	std::set<string> _stereoPostconditions;
	std::set<string> _stereoBuiltIn;
	std::set<string> _stereoNonInterupt;

	std::set<string> _stereoBlocks;


public:
	void addStereotypes();
	inline shared_ptr<Model> getModel() { return _model;}


private:
	map<string, shared_ptr<Class> > _idClass;

	shared_ptr<XmlParser> _loader;

	shared_ptr<XmlNode> _xmi;

	shared_ptr<Model> _model;


public:
	typedef vector<pair<string, string> > behaviorsCode_t;


private:
	behaviorsCode_t _opaqueBehaviorsCode;


public:
	typedef vector<pair<string,string> > generalization_t;


private:
	generalization_t _toGeneralize;

	map<string, shared_ptr<Operation> > _idOperations;
	
	map<string, shared_ptr<Behavior> > _idBehaviors;

	map<string, shared_ptr<Parameter> > _parameters;
	map<string, shared_ptr<ActivityPartition> > _partitions;

	map<string , string> _paramToType;

	map<string, shared_ptr<Classifier> > _classifiers;

    map<shared_ptr<CallOperationAction> ,  string> _callOperations;
    
    map<shared_ptr<CallBehaviorAction> ,  string> _callBehaviors;
    
    map<shared_ptr<CallEvent> ,  string> _callEvents;

    vector<ConstraintT> _constraints;


public:
	//FinalNode, Merge et Decision devraent �tre des activityNode !!

	void addActivityNode(const shared_ptr<XmlNode> & node, shared_ptr<Activity> & activity);
	void addActivityNode(const shared_ptr<XmlNode> & node, shared_ptr<LoopNode> & loopNode, shared_ptr<Activity> & activity);

	//TODO
	void addActivityParameter(const shared_ptr<XmlNode> & node, shared_ptr<Activity> & activity);

	//TODO: containedEdge avec des ObjectFlow
	//Gerer diff entre ObjectFlow et ControlFlow
	void addActivityEdge(const shared_ptr<XmlNode> & node, shared_ptr<Activity> & activity);
	void addActivityEdge(const shared_ptr<XmlNode> & node, shared_ptr<LoopNode> & activity);

	//TODO
	void addActivityGroup(const shared_ptr<XmlNode> & node, shared_ptr<Activity> & activity);

private:
	map<string, shared_ptr<Signal> > _signals;
	map<string, shared_ptr<Event> > _events;

	map<string, shared_ptr<ActivityNode> > _activityNodes;
	map<string, string>_waitingNodes;
	map<string, shared_ptr<ObjectNode> >_objectNodes;
    map<string, shared_ptr<Expression> > _activityExpressions;

public:
	string getComment(const shared_ptr<XmlNode> & node);
	string getSummary(const shared_ptr<XmlNode> & node);
	map<string, string > getTags(const shared_ptr<XmlNode> & node);


	void addMachineRegion(const shared_ptr<XmlNode> & node, shared_ptr<StateMachine> & machine, shared_ptr<Region> region, map<string, shared_ptr<Vertex> > & vertices);

	//non gestion des triggers
	//Attention ! Les Opaque expressions ne sont pas impl�ment� mais sont utilis� pour le moment dans les modelLoader
	//(par contre les expressions existe - c'est du ocl)
	void addMachineTransition(const shared_ptr<XmlNode> & node, shared_ptr<StateMachine> & machine, map<string,shared_ptr<Vertex> > & vertices);

	void addActivitiesAndStateMachines();


private:
	vector<pair<shared_ptr<XmlNode>,shared_ptr<Package> > > _activities;
        map<string, shared_ptr<Activity> >_activitiesMethod;

	vector<pair<shared_ptr<XmlNode>,shared_ptr<Package> > > _stateMachines;
        map<string,shared_ptr<StateMachine> > _subStateMachines;
        map<string,shared_ptr<State>  > _subStates;
        map<string,string > _subStatesID;

};
inline bool ModelLoader2::isStereotypedEntity(const shared_ptr<XmlNode> & node) {
	// Bouml preserved body begin 0001FFE7
	string id=node->getProperty("id");
	return (_stereoEntities.find(id)!=_stereoEntities.end());
	// Bouml preserved body end 0001FFE7
}

inline bool ModelLoader2::isStereotypedAgent(const shared_ptr<XmlNode> & node) {
	// Bouml preserved body begin 00020067
	string id=node->getProperty("id");
	return (_stereoAgents.find(id)!=_stereoAgents.end());
	// Bouml preserved body end 00020067
}
inline bool ModelLoader2::isStereotypedVirtualHuman(const shared_ptr<XmlNode> & node) {
	string id=node->getProperty("id");
	return (_stereoVirtualHumans.find(id)!=_stereoVirtualHumans.end());
}
inline bool ModelLoader2::isStereotypedRole(const shared_ptr<XmlNode> & node) {
	// Bouml preserved body begin 000200E7
	string id=node->getProperty("id");
	return (_stereoRoles.find(id)!=_stereoRoles.end());
	// Bouml preserved body end 000200E7
}

inline bool ModelLoader2::isStereotypedBuiltIn(const shared_ptr<XmlNode> & node) {
	// Bouml preserved body begin 000219E7
	string id=node->getProperty("id");
	return (_stereoBuiltIn.find(id)!=_stereoBuiltIn.end());
	// Bouml preserved body end 000219E7
}
inline bool ModelLoader2::isStereotypedNonInterupt(const shared_ptr<XmlNode> & node) {
	// Bouml preserved body begin 000219E7
	string id=node->getProperty("id");
	return (_stereoNonInterupt.find(id)!=_stereoNonInterupt.end());
	// Bouml preserved body end 000219E7
}
inline bool ModelLoader2::isStereotypedBlock(const shared_ptr<XmlNode> & node) {
	string id=node->getProperty("id");
	return (_stereoBlocks.find(id)!=_stereoBlocks.end());
}
#endif
