#include "VEHA/Behavior/BasicActions/CallBehaviorAction.h"
#include "VEHA/Behavior/BasicActions/CallBehaviorBehaviorExecution.h"
#include "VEHA/Kernel/Operation.h"
#include "VEHA/Kernel/Parameter.h"
#include <boost/algorithm/string.hpp>

namespace VEHA
{

CallBehaviorAction::CallBehaviorAction()
{
	_kind = "CallBehavior";
}

CallBehaviorAction::~CallBehaviorAction()
{
}

shared_ptr<BehaviorExecution> CallBehaviorAction::createBehaviorExecution(shared_ptr<InstanceSpecification> host, const Parameters& p, bool sync)
{
	shared_ptr<CallBehaviorBehaviorExecution> behavior(new CallBehaviorBehaviorExecution(shared_dynamic_cast<CallBehaviorAction>(shared_from_this()),host,p));
	return behavior;
}

}
