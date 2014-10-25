#include "VEHA/Behavior/BasicActions/CreateEntityAction.h"
#include "VEHA/Behavior/BasicActions/CreateEntityBehaviorExecution.h"

namespace VEHA
{

CreateEntityAction::CreateEntityAction()
{
	_kind = "CreateEntity";
}

CreateEntityAction::~CreateEntityAction()
{
}

shared_ptr<BehaviorExecution> CreateEntityAction::createBehaviorExecution(shared_ptr<InstanceSpecification> host, const Parameters& p)
{
	shared_ptr<CreateEntityBehaviorExecution> behavior(new CreateEntityBehaviorExecution(shared_dynamic_cast<CreateEntityAction>(shared_from_this()),host,p));
	return behavior;
}

}
