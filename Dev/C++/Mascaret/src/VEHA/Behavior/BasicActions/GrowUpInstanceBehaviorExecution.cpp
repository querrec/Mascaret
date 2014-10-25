#include "VEHA/Behavior/Common/Signal.h"
#include "VEHA/Behavior/BasicActions/GrowUpInstanceBehaviorExecution.h"
#include "VEHA/Entity/Entity.h"
#include "VEHA/Entity/ShapeSpecification.h"
#include "VEHA/Entity/EntityClass.h"
#include "VEHA/Kernel/Environment.h"
#include <boost/bind.hpp>

namespace VEHA
{

GrowUpInstanceBehaviorExecution::GrowUpInstanceBehaviorExecution(shared_ptr<GrowUpInstanceAction> action,
		shared_ptr<InstanceSpecification> host, const Parameters& p):BehaviorExecution(action,host,p), _action(action), _first(true)
{
}

GrowUpInstanceBehaviorExecution::~GrowUpInstanceBehaviorExecution()
{
}

double GrowUpInstanceBehaviorExecution::execute(double dt)
{
	shared_ptr<Entity> entity=shared_dynamic_cast<Entity>(_action->getTarget());
	if(entity && entity->getActiveShape())
	{
		if(!_first)
		{
			Vector3 scale(1.0,1.0,1.0);
			entity->getActiveShape()->setScale(scale);
			return 0;
		}
		Vector3 scale(1.4,1.4,1.4);
		entity->getActiveShape()->setScale(scale);
	}
	else return 0;
	_first=false;
	return 5;
}

} // Namespace
