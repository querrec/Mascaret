#include "VEHA/Behavior/BasicActions/StopAnimationAction.h"
#include "VEHA/Behavior/BasicActions/StopAnimationBehaviorExecution.h"

namespace VEHA
{

StopAnimationAction::StopAnimationAction()
{
	_kind = "StopAnimation";
}

StopAnimationAction::~StopAnimationAction()
{
}

shared_ptr<BehaviorExecution> StopAnimationAction::createBehaviorExecution(shared_ptr<InstanceSpecification> host, const Parameters& p, bool sync)
{
	shared_ptr<StopAnimationBehaviorExecution> behavior(new StopAnimationBehaviorExecution(shared_dynamic_cast<StopAnimationAction>(shared_from_this()),host,p));
	return behavior;
}

}
