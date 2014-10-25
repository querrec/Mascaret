#ifndef XMI14_LOADER_H
#define XMI14_LOADER_H

#include "Tools/veha_plateform.h"
#include "Tools/Loaders/AbstractModelLoader.h"

using namespace VEHA;
using std::map;
using std::vector;
using std::pair;

class VEHA_API XMI11Loader : public AbstractModelLoader
{

public:
	XMI11Loader();
	virtual ~XMI11Loader();

protected:
	// Structural
	// Class Model
	virtual shared_ptr<Model> getModel(shared_ptr<XmlNode> node);
	virtual void addStereoType(shared_ptr<XmlNode> node);
	virtual shared_ptr<Package> getPackage(shared_ptr<XmlNode> node, shared_ptr<Model> model);
	virtual shared_ptr<PrimitiveType> getDatatype(shared_ptr<XmlNode> node);
	virtual shared_ptr<Class> getClass(shared_ptr<XmlNode> node);
	virtual void addGeneralization(shared_ptr<XmlNode> node, shared_ptr<Class> c);
	virtual shared_ptr<Operation> getOperation(shared_ptr<XmlNode> node,shared_ptr<Class> c);
	virtual shared_ptr<Property> getAttribute(shared_ptr<XmlNode> node,shared_ptr<Class> c);
	virtual shared_ptr<Property> getAssociation(shared_ptr<XmlNode> node,shared_ptr<Class> c);
	virtual shared_ptr<Parameter> getParameter(shared_ptr<XmlNode> node);

	// Behavior
	// State Machines
	virtual shared_ptr<StateMachine> getStateMachine(shared_ptr<XmlNode> node);
	virtual shared_ptr<State> getState(shared_ptr<XmlNode> node);
	virtual shared_ptr<PseudoState> getPseudoState(shared_ptr<XmlNode> node);
	virtual shared_ptr<Transition> getTransition(shared_ptr<XmlNode> node);
	virtual shared_ptr<Region> getRegion(shared_ptr<XmlNode> node);
	virtual shared_ptr<Guard> getGuard(shared_ptr<XmlNode> node);
	virtual shared_ptr<SignalEvent> getSignalEvent(shared_ptr<XmlNode> node);
	virtual shared_ptr<TimeEvent> getTimeEvent(shared_ptr<XmlNode> node);

	// Activities
	virtual shared_ptr<Activity> getActivity(shared_ptr<XmlNode> node);
	virtual shared_ptr<ActionNode> getActionNode(shared_ptr<XmlNode> node);
	virtual shared_ptr<ControlNode> getControlNode(shared_ptr<XmlNode> node);
	virtual shared_ptr<ActivityEdge> getActivityEdge(shared_ptr<XmlNode> node);
	virtual shared_ptr<ObjectNode> getObjectNode(shared_ptr<XmlNode> node);
	virtual shared_ptr<Action> getAction(shared_ptr<XmlNode> node);

	// Utilities
	string getElementName(shared_ptr<XmlNode> node);
	string getElementId(shared_ptr<XmlNode> node);
	string getElementSummary(shared_ptr<XmlNode> node);
	string getElementDescription(shared_ptr<XmlNode> node);
	string getElementStereotypeRef(shared_ptr<XmlNode> node);

	// second pass
	void linkParametersType();
	void linkGeneralizations();


	map<string, shared_ptr<PrimitiveType> > _basicTypes;
	map<string, string> _sBasicTypes;
	map<string, string> _stereotypes;

	map<string, shared_ptr<Package> > _packages;
	map<string, shared_ptr<Class> > _classes;
	map<string, shared_ptr<Signal> > _signals;
	map<string, shared_ptr<Classifier> > _classifiers;
	map<string, shared_ptr<Parameter> > _parameters;
	map<string, shared_ptr<Operation> > _operationType;
	map<string, string > _paramToType;
	vector<pair<string, string> > _generalisations;
	//vector<shared_ptr<XMIClasse> > _xmiClasses;
	//map<string, shared_ptr<Trigger> > _idEvents;
	map<string, shared_ptr<Operation> > _idOperations;

	vector<pair<string, string> > _opaqueBehaviorsCode;

	//map<string, shared_ptr<OrganisationalStructure> > _organisations;
	//map<string, shared_ptr<RoleClass> > _roleClasses;
	map<string,string> namespaces;
};

#endif
