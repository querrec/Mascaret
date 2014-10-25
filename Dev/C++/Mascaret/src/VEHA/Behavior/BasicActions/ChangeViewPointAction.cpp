#include "VEHA/Behavior/BasicActions/ChangeViewPointAction.h"
#include "VEHA/Behavior/BasicActions/ChangeViewPointBehaviorExecution.h"

namespace VEHA
{

ChangeViewPointAction::ChangeViewPointAction()
{
	_kind = "ChangeViewPoint";
}

ChangeViewPointAction::~ChangeViewPointAction()
{
}

shared_ptr<BehaviorExecution> ChangeViewPointAction::createBehaviorExecution(shared_ptr<InstanceSpecification> host, const Parameters& p, bool sync)
{
	shared_ptr<ChangeViewPointBehaviorExecution> behavior(new ChangeViewPointBehaviorExecution(shared_dynamic_cast<ChangeViewPointAction>(shared_from_this()),host,p));
	return behavior;
}

}
