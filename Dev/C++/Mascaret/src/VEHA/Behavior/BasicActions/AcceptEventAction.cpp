#include "VEHA/Behavior/BasicActions/AcceptEventAction.h"
#include "VEHA/Behavior/BasicActions/AcceptEventBehaviorExecution.h"

namespace VEHA
{

AcceptEventAction::AcceptEventAction()
{
	_kind = "AcceptEvent";
}

AcceptEventAction::~AcceptEventAction()
{
}

shared_ptr<BehaviorExecution> AcceptEventAction::createBehaviorExecution(shared_ptr<InstanceSpecification> host, const Parameters& p, bool sync)
{
	shared_ptr<AcceptEventBehaviorExecution> behavior(new AcceptEventBehaviorExecution(shared_dynamic_cast<AcceptEventAction>(shared_from_this()),host,p));
	return behavior;
}

}
