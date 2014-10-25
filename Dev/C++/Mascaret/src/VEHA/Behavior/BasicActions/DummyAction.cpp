#include "VEHA/Behavior/BasicActions/DummyAction.h"
#include "VEHA/Behavior/BasicActions/DummyBehaviorExecution.h"

namespace VEHA
{

DummyAction::DummyAction()
{
	_kind = "Dummy";
}

DummyAction::~DummyAction()
{
}

shared_ptr<BehaviorExecution> DummyAction::createBehaviorExecution(shared_ptr<InstanceSpecification> host, const Parameters& p, bool sync)
{
	shared_ptr<DummyBehaviorExecution> behavior(new DummyBehaviorExecution(shared_dynamic_cast<DummyAction>(shared_from_this()),host,p));
	return behavior;
}

}
