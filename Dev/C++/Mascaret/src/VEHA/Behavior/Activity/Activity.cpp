#include "VEHA/Kernel/Expression.h"
#include "VEHA/Behavior/BasicActions/Action.h"
#include "VEHA/Behavior/Activity/Activity.h"
#include "VEHA/Behavior/Activity/ActivityNode.h"
#include "VEHA/Behavior/Activity/ActionNode.h"
#include "VEHA/Behavior/Activity/ActivityBehaviorExecution.h"

namespace VEHA
{

Activity::Activity(const string & name)
:Behavior(name)
{
}

Activity::~Activity()
{
}

shared_ptr<BehaviorExecution> Activity::createBehaviorExecution(shared_ptr<InstanceSpecification> host,const Parameters& p, bool sync)
{
	shared_ptr<ActivityBehaviorExecution> behavior(new ActivityBehaviorExecution(shared_dynamic_cast<Activity>(shared_from_this()),host,p,sync));
	return behavior;
}

}
