#ifndef _v2_VEHA_Behavior_Activity_ActivityPartition_H
#define _v2_VEHA_Behavior_Activity_ActivityPartition_H

#include "Tools/veha_plateform.h"
#include "VEHA/Kernel/Element.h"
#include "BEHAVE/Role.h"


namespace VEHA
{
class Activity;
class ActivityNode;
class ActivityEdge;
class VEHA_API ActivityPartition : public Element
{
public :
	ActivityPartition(const string& name);
	virtual ~ActivityPartition();

	inline vector< shared_ptr<ActivityEdge> > getEdge(void) { return _edge;}
	inline shared_ptr<ActivityEdge> getEdge_at(int index) { return _edge[index];}
	inline void addEdge(shared_ptr<ActivityEdge> value) { _edge.push_back(value);}

	inline vector< shared_ptr<ActivityNode> > getNode(void) { return _node;}
	inline shared_ptr<ActivityNode> getNode_at(int index) { return _node[index];}
	inline void addNode(shared_ptr<ActivityNode> value) { _node.push_back(value);}

	inline shared_ptr<BEHAVE::Role> getRole(void) {return _role.lock();}
	inline void setRole(shared_ptr<BEHAVE::Role> value) {_role = value;}

	inline void setActivity(shared_ptr<Activity> value) {_activity = value;}
	inline shared_ptr<Activity> getActivity(void) { return _activity.lock();}
protected:
	weak_ptr<BEHAVE::Role> _role;
	vector< shared_ptr<ActivityEdge> >  _edge;
	vector< shared_ptr<ActivityNode> >  _node;
	weak_ptr<Activity>  _activity;
};
}
#endif
