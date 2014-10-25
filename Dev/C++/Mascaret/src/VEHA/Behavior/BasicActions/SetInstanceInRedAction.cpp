#include "VEHA/Behavior/BasicActions/SetInstanceInRedAction.h"
#include "VEHA/Behavior/BasicActions/SetInstanceInRedBehaviorExecution.h"

namespace VEHA
{

SetInstanceInRedAction::SetInstanceInRedAction()
{
	_kind = "SetInRed";
}

SetInstanceInRedAction::~SetInstanceInRedAction()
{
}

shared_ptr<BehaviorExecution> SetInstanceInRedAction::createBehaviorExecution(shared_ptr<InstanceSpecification> host, const Parameters& p, bool sync)
{
	shared_ptr<SetInstanceInRedBehaviorExecution> behavior(new SetInstanceInRedBehaviorExecution(shared_dynamic_cast<SetInstanceInRedAction>(shared_from_this()),host,p));
	return behavior;
}

}
