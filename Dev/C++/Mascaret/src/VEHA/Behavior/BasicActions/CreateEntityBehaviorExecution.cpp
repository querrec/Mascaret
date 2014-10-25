#include "VEHA/Behavior/Common/Signal.h"
#include "VEHA/Behavior/BasicActions/CreateEntityBehaviorExecution.h"
#include "VEHA/Entity/Entity.h"
#include "VEHA/Entity/EntityClass.h"
#include "VEHA/Kernel/Environment.h"
#include "MascaretApplication.h"
#include <boost/bind.hpp>

namespace VEHA
{

CreateEntityBehaviorExecution::CreateEntityBehaviorExecution(shared_ptr<CreateEntityAction> action,
		shared_ptr<InstanceSpecification> host, const Parameters& p):BehaviorExecution(action,host,p), _action(action)
{
}

CreateEntityBehaviorExecution::~CreateEntityBehaviorExecution()
{
}

double CreateEntityBehaviorExecution::execute(double dt)
{
	shared_ptr<Environment> env = MascaretApplication::getInstance()->getEnvironment();

	shared_ptr<EntityClass> classe = _action->getClassifier();
	string name = _action->getName();

	shared_ptr<Entity> entity =
			shared_ptr<Entity>(new Entity(name,classe));
	env->addInstance(entity);

	return 0;
}

} // Namespace
