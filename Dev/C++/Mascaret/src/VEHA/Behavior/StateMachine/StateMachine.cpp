#include "VEHA/Kernel/InstanceSpecification.h"
#include "VEHA/Behavior/StateMachine/StateMachine.h"
#include "VEHA/Behavior/StateMachine/Region.h"
#include "VEHA/Behavior/StateMachine/StateMachineBehaviorExecution.h"
#include "VEHA/Behavior/Common/BehaviorExecution.h"

namespace VEHA
{


StateMachine::StateMachine(const string& name)
:Behavior(name)
{
}

StateMachine::~StateMachine()
{
}

shared_ptr<BehaviorExecution> StateMachine::createBehaviorExecution(shared_ptr<InstanceSpecification> host,const Parameters& p, bool sync)
{
	shared_ptr<StateMachineBehaviorExecution> behavior(new StateMachineBehaviorExecution(shared_dynamic_cast<StateMachine>(shared_from_this()),host,p));
	host->addStateMachineExecution(behavior);
	return behavior;
}

}
