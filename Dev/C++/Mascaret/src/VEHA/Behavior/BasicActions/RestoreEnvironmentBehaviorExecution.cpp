#include "VEHA/Behavior/Common/Signal.h"
#include "VEHA/Behavior/BasicActions/RestoreEnvironmentBehaviorExecution.h"
#include "VEHA/Entity/Entity.h"
#include "VEHA/Entity/EntityClass.h"
#include "VEHA/Entity/ShapeSpecification.h"
#include "VEHA/Kernel/Environment.h"
#include <boost/bind.hpp>


namespace VEHA
{

RestoreEnvironmentBehaviorExecution::RestoreEnvironmentBehaviorExecution(shared_ptr<RestoreEnvironmentAction> action,
		shared_ptr<InstanceSpecification> host, const Parameters& p):BehaviorExecution(action,host,p), _action(action)
{
}

RestoreEnvironmentBehaviorExecution::~RestoreEnvironmentBehaviorExecution()
{
}

double RestoreEnvironmentBehaviorExecution::execute(double dt)
{
	//Interface3D::get()->restoreAll();
	return 0;
}

} // Namespace
