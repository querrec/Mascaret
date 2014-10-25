#ifndef _v2_VEHA_Behavior_Activity_Activity_H
#define _v2_VEHA_Behavior_Activity_Activity_H

#include "Tools/veha_plateform.h"
#include "VEHA/Behavior/Common/Behavior.h"
#include "VEHA/Behavior/Activity/ActivityNode.h"
#include "VEHA/Behavior/Activity/ActivityPartition.h"
#include "VEHA/Behavior/Activity/ActivityEdge.h"


namespace VEHA
{
class VEHA_API Activity : public Behavior
{
public :
	Activity(const string& name);
	virtual ~Activity();

protected :
	vector< shared_ptr<ActivityNode> >  _node;
	vector< shared_ptr<ActivityPartition> >  _partitions;
	vector< shared_ptr<ActivityEdge> >  _edges;

	shared_ptr<ActivityNode> _initial;

public :
	inline vector< shared_ptr<ActivityNode> > getNode(void) { return _node;}
	inline shared_ptr<ActivityNode> getNode_at(int index) { return _node[index];}
	inline void addNode(shared_ptr<ActivityNode> value)
	{
		_node.push_back(value);
		value->setActivity(shared_dynamic_cast<Activity>(shared_from_this()));
	}

	inline vector< shared_ptr<ActivityPartition> > getPartition(void) { return _partitions;}
	inline shared_ptr<ActivityPartition> getPartition_at(int index) { return _partitions[index];}
	inline void addPartition(shared_ptr<ActivityPartition> value)
	{
		_partitions.push_back(value);
		value->setActivity(shared_dynamic_cast<Activity>(shared_from_this()));
	}

	inline vector< shared_ptr<ActivityEdge> > getEdge(void) { return _edges;}
	inline shared_ptr<ActivityEdge> getEdge_at(int index) { return _edges[index];}
	inline void addEdge(shared_ptr<ActivityEdge> value)
	{
		_edges.push_back(value);
		value->setActivity(shared_dynamic_cast<Activity>(shared_from_this()));
	}

	inline void setInitialNode(shared_ptr<ActivityNode> initial) {_initial = initial;}
	inline shared_ptr<ActivityNode> getInitialNode(void) { return _initial;}

	virtual shared_ptr<BehaviorExecution> createBehaviorExecution(shared_ptr<InstanceSpecification> host,const Parameters& p, bool sync =false);
};

}
#endif
