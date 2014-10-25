#include "VEHA/Behavior/BasicActions/RestoreEnvironmentAction.h"
#include "VEHA/Behavior/BasicActions/RestoreEnvironmentBehaviorExecution.h"

namespace VEHA
{

RestoreEnvironmentAction::RestoreEnvironmentAction()
{
	_kind = "RestoreEnvironment";
}

RestoreEnvironmentAction::~RestoreEnvironmentAction()
{
}

shared_ptr<BehaviorExecution> RestoreEnvironmentAction::createBehaviorExecution(shared_ptr<InstanceSpecification> host, const Parameters& p, bool sync)
{
	shared_ptr<RestoreEnvironmentBehaviorExecution> behavior(new RestoreEnvironmentBehaviorExecution(shared_dynamic_cast<RestoreEnvironmentAction>(shared_from_this()),host,p));
	return behavior;
}

}
