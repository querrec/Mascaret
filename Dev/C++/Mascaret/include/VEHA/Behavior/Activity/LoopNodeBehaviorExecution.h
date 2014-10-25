#ifndef _v2_VEHA_Behavior_Activity_LoopNodeBehaviorExecution_H
#define _v2_VEHA_Behavior_Activity_LoopNodeBehaviorExecution_H

#include "Tools/veha_plateform.h"
#include "VEHA/Behavior/Common/BehaviorExecution.h"
#include "VEHA/Behavior/Common/Parameters.h"
#include "VEHA/Behavior/Activity/Activity.h"
#include "VEHA/Behavior/Activity/LoopNodeBehavior.h"


namespace VEHA
{

class VEHA_API LoopNodeBehaviorExecution : public BehaviorExecution, boost::noncopyable
{
public :
	LoopNodeBehaviorExecution(shared_ptr<LoopNodeBehavior> loopNodeBehavior, shared_ptr<InstanceSpecification> host, const Parameters& p, bool sync = false);
	virtual ~LoopNodeBehaviorExecution();

	virtual double execute(double dt);
private :
	int _stateActivity;
	vector<shared_ptr<ActionNode> > _actions;
	vector<shared_ptr<ActionNode> > _execs;
	map<string, shared_ptr<InstanceSpecification> > _affectations;
	shared_ptr<Activity> _activity;

	shared_ptr<LoopNodeBehavior> _loopNodeBehavior;
	shared_ptr<LoopNode> _loopNode;



};

}
#endif
