#include "VEHA/Behavior/StateMachine/PseudoState.h"
#include "VEHA/Kernel/InstanceSpecification.h"

#include "VEHA/Behavior/Common/BehaviorExecution.h"
#include "VEHA/Behavior/Common/behaviorScheduler.h"


namespace VEHA
{

PseudoState::PseudoState(const string& name)
:Vertex(name)
{
}

PseudoState::~PseudoState()
{
}

shared_ptr<BehaviorExecution> PseudoState::activate(shared_ptr<InstanceSpecification> /*host*/, const Parameters & /*p*/)
{
	return shared_ptr<BehaviorExecution>();
}

void PseudoState::desactivate(void)
{
}

}
