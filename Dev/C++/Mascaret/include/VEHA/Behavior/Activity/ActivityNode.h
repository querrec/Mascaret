#ifndef _v2_VEHA_Behavior_Activity_ActivityNode_H
#define _v2_VEHA_Behavior_Activity_ActivityNode_H

#include "Tools/veha_plateform.h"
#include "VEHA/Kernel/Element.h"

namespace VEHA
{
class ActivityEdge;
class Activity;
class ActivityPartition;
class ActionNode;
class ObjectNode;
class Action;
class VEHA_API ActivityNode  : public Element
{
public :
	ActivityNode(const string& name, const string& kind);
	virtual ~ActivityNode();

protected :
	string _kind;
	vector< shared_ptr<ActivityEdge> >  _incoming;
	vector< shared_ptr<ActivityEdge> >  _outgoing;

	vector< shared_ptr<ActivityPartition> >  _partition;
	weak_ptr<Activity>  _activity;
	bool _running;

public :
	inline string getKind(void) {return _kind;}
	inline void setKind(string kind) {_kind = kind;}

	inline vector< shared_ptr<ActivityEdge> > getIncoming(void) { return _incoming;}
	inline shared_ptr<ActivityEdge> getIncoming_at(int index) { return _incoming[index];}
	inline void addIncoming(shared_ptr<ActivityEdge> value) { _incoming.push_back(value);}
	virtual vector< shared_ptr<ActionNode> > getIncomingActionNode();
	virtual vector< shared_ptr<ObjectNode> > getIncomingObjectNode();
	virtual vector< shared_ptr<ActionNode> > getIncomingActionNodeByPartition(string partitionName);

	inline vector< shared_ptr<ActivityEdge> > getOutgoing(void) { return _outgoing;}
	inline shared_ptr<ActivityEdge> getOutgoing_at(int index) { return _outgoing[index];}
	inline void addOutgoing(shared_ptr<ActivityEdge> value) { _outgoing.push_back(value);}
	virtual vector< shared_ptr<ActionNode> > getOutgoingActionNode();
	virtual vector<shared_ptr<ActionNode> > getPossibleOutgoingActionNode();
	virtual vector< shared_ptr<ObjectNode> > getOutgoingObjectNode();
	virtual vector< shared_ptr<ActionNode> > getOutgoingActionNodeByPartition(const string& partitionName);
	
	virtual vector< shared_ptr<ActivityEdge> > getOutgoingActionNodeEdges();

	virtual vector< shared_ptr<ActivityEdge> > getOutgoingControlFlowEdges();

	inline vector< shared_ptr<ActivityPartition> > getPartition(void) { return _partition;}
	inline shared_ptr<ActivityPartition> getPartition_at(int index) { return _partition[index];}
	inline void addPartition(shared_ptr<ActivityPartition> value) { _partition.push_back(value);}

	void setActivity(shared_ptr<Activity> value) {_activity = value;};
	inline shared_ptr<Activity> getActivity(void) { return _activity.lock();}

	virtual inline bool isRunning (void) { return _running ;}
};
}
#endif
