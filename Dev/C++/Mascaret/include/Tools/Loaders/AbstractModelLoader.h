#ifndef MODEL_LOADER_H
#define MODEL_LOADER_H

#include "Tools/veha_plateform.h"

class XmlNode;
namespace VEHA
{
class Classifier;
class Class;
class Package;
class PrimitiveType;
class Operation;
class Property;
class Parameter;
class Model;

class Activity;
class ActivityNode;
class ActionNode;
class ObjectNode;
class ControlNode;
class ActivityEdge;

class Action;
class State;
class Transition;
class PseudoState;
class Region;
class Guard;
class StateMachine;
class SignalEvent;
class TimeEvent;
class Signal;
}

using boost::shared_ptr;
using namespace VEHA;

using std::string;

class VEHA_API AbstractModelLoader
{
public:
	virtual shared_ptr<Model> parseFile(const string& filename);
	virtual shared_ptr<Model> parseString(const string& str);

protected:
	// Structural
	// Class Model
	virtual shared_ptr<Model> getModel(shared_ptr<XmlNode> node)=0;
	virtual void addStereoType(shared_ptr<XmlNode> node)=0;
	virtual shared_ptr<Package> getPackage(shared_ptr<XmlNode> node, shared_ptr<Model> model)=0;
	virtual shared_ptr<PrimitiveType> getDatatype(shared_ptr<XmlNode> node)=0;
	virtual shared_ptr<Class> getClass(shared_ptr<XmlNode> node)=0;
	virtual shared_ptr<Operation> getOperation(shared_ptr<XmlNode> node,shared_ptr<Class> c)=0;
	virtual shared_ptr<Property> getAttribute(shared_ptr<XmlNode> node,shared_ptr<Class> c)=0;
	virtual shared_ptr<Property> getAssociation(shared_ptr<XmlNode> node,shared_ptr<Class> c)=0;
	virtual shared_ptr<Parameter> getParameter(shared_ptr<XmlNode> node)=0;

	// Behavior
	// State Machines
	virtual shared_ptr<StateMachine> getStateMachine(shared_ptr<XmlNode> node)=0;
	virtual shared_ptr<State> getState(shared_ptr<XmlNode> node)=0;
	virtual shared_ptr<PseudoState> getPseudoState(shared_ptr<XmlNode> node)=0;
	virtual shared_ptr<Transition> getTransition(shared_ptr<XmlNode> node)=0;
	virtual shared_ptr<Region> getRegion(shared_ptr<XmlNode> node)=0;
	virtual shared_ptr<Guard> getGuard(shared_ptr<XmlNode> node)=0;
	virtual shared_ptr<SignalEvent> getSignalEvent(shared_ptr<XmlNode> node)=0;
	virtual shared_ptr<TimeEvent> getTimeEvent(shared_ptr<XmlNode> node)=0;

	// Activities
	virtual shared_ptr<Activity> getActivity(shared_ptr<XmlNode> node)=0;
	virtual shared_ptr<ActionNode> getActionNode(shared_ptr<XmlNode> node)=0;
	virtual shared_ptr<ControlNode> getControlNode(shared_ptr<XmlNode> node)=0;
	virtual shared_ptr<ActivityEdge> getActivityEdge(shared_ptr<XmlNode> node)=0;
	virtual shared_ptr<ObjectNode> getObjectNode(shared_ptr<XmlNode> node)=0;
	virtual shared_ptr<VEHA::Action> getAction(shared_ptr<XmlNode> node)=0;

};

#endif
