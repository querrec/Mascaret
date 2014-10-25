#include "VEHA/Behavior/StateMachine/State.h"
#include "VEHA/Behavior/StateMachine/StateMachine.h"
#include "VEHA/Kernel/Operation.h"
#include "VEHA/Kernel/InstanceSpecification.h"
#include "VEHA/Behavior/Common/Behavior.h"
#include "VEHA/Behavior/Common/BehaviorExecution.h"
#include "VEHA/Behavior/Common/behaviorScheduler.h"


namespace VEHA
{

State::State(const string& name)
:Vertex(name),_isSimple(true), _doBehavior(), _isSubMachineState(false)
{
}

State::~State()
{
}

shared_ptr<BehaviorExecution> State::activate(shared_ptr<InstanceSpecification> entity, const Parameters & param)
{
	shared_ptr<BehaviorExecution> be;
        if (_isSubMachineState) 
        {
           shared_ptr<StateMachine> subMachine = getSubMachine();
	   Parameters param;
	   be = BehaviorScheduler::getInstance()->executeBehavior(subMachine,entity,param);
        }
        else if (!_isSimple)
        {
           cerr << " !!!!!!!!!!! SUBREGION !!!!!!!" << endl;
        }

	shared_ptr<Operation> doBehavior=getDoBehavior();
	if (doBehavior )
   	{
	   be = BehaviorScheduler::getInstance()->executeBehavior(doBehavior->getMethod(),entity,param);
	}

	return be;
}

void State::simulate(shared_ptr<InstanceSpecification> entity)
{
	shared_ptr<BehaviorExecution> be;
        if (_isSubMachineState) 
        {
           shared_ptr<StateMachine> subMachine = getSubMachine();
	   Parameters param;
	   BehaviorScheduler::getInstance()->executeBehavior(subMachine,entity,param);
        }
        else if (!_isSimple)
        {
           cerr << " !!!!!!!!!!! SUBREGION !!!!!!!" << endl;
        }

	shared_ptr<Operation> doBehavior=getDoBehavior();
	if (doBehavior )
   	{
	   Parameters param;
	   BehaviorScheduler::getInstance()->executeBehavior(doBehavior->getMethod(),entity,param,true);
	}

}

void State::desactivate(void)
{
}

}
