#include "VEHA/Behavior/BasicActions/SetInstanceTransparencyAction.h"
#include "VEHA/Behavior/BasicActions/SetInstanceTransparencyBehaviorExecution.h"

namespace VEHA
{

SetInstanceTransparencyAction::SetInstanceTransparencyAction()
{
	_kind = "SetTransparency";
}

SetInstanceTransparencyAction::~SetInstanceTransparencyAction()
{
}

shared_ptr<BehaviorExecution> SetInstanceTransparencyAction::createBehaviorExecution(shared_ptr<InstanceSpecification> host, const Parameters& p, bool sync)
{
	shared_ptr<SetInstanceTransparencyBehaviorExecution> behavior(new SetInstanceTransparencyBehaviorExecution(shared_dynamic_cast<SetInstanceTransparencyAction>(shared_from_this()),host,p));
	return behavior;
}

}
