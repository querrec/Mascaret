#include "VEHA/Behavior/BasicActions/GrowUpInstanceAction.h"
#include "VEHA/Behavior/BasicActions/GrowUpInstanceBehaviorExecution.h"

namespace VEHA
{

GrowUpInstanceAction::GrowUpInstanceAction()
{
	_kind = "GrowUp";
}

GrowUpInstanceAction::~GrowUpInstanceAction()
{
}

shared_ptr<BehaviorExecution> GrowUpInstanceAction::createBehaviorExecution(shared_ptr<InstanceSpecification> host, const Parameters& p, bool sync)
{
	shared_ptr<GrowUpInstanceBehaviorExecution> behavior(new GrowUpInstanceBehaviorExecution(shared_dynamic_cast<GrowUpInstanceAction>(shared_from_this()),host,p));
	return behavior;
}

}
