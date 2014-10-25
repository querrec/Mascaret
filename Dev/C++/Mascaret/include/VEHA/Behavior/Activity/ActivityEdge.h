#ifndef _v2_VEHA_Behavior_Activity_ActivityEdge_H
#define _v2_VEHA_Behavior_Activity_ActivityEdge_H

#include "Tools/veha_plateform.h"
#include "VEHA/Kernel/Element.h"
#include "VEHA/Kernel/Constraint.h"


namespace VEHA
{
class Activity;
class ActivityPartition;
class ActivityNode;
class VEHA_API ActivityEdge : public Element
{
public :
	ActivityEdge(const string& name);
	virtual ~ActivityEdge();

protected :
	weak_ptr<ActivityNode>  _target;
	weak_ptr<ActivityNode>  _source;
	shared_ptr<Constraint>  _guard;
	vector< shared_ptr<ActivityPartition> >  _partition;
	weak_ptr<Activity>  _activity;
	
	string _kind;

public :
	inline string getKind(void) {return _kind;}
	inline void setKind(string kind) {_kind = kind;}
	
	inline void setTarget(shared_ptr<ActivityNode> value) {_target = value;}
	inline shared_ptr<ActivityNode> getTarget(void) { return _target.lock();}

	inline void setSource(shared_ptr<ActivityNode> value) {_source = value;}
	inline shared_ptr<ActivityNode> getSource(void) { return _source.lock();}

	inline void setGuard(shared_ptr<Constraint> value) {_guard = value;}
	inline shared_ptr<Constraint> getGuard(void) { return _guard;}

	inline vector< shared_ptr<ActivityPartition> > getPartition(void) { return _partition;}
	inline shared_ptr<ActivityPartition> getPartition_at(int index) { return _partition[index];}
	inline void addPartition(shared_ptr<ActivityPartition> value) { _partition.push_back(value);}

	inline void setActivity(shared_ptr<Activity> value) {_activity = value;}
	inline shared_ptr<Activity> getActivity(void) { return _activity.lock();}
};

}
#endif
