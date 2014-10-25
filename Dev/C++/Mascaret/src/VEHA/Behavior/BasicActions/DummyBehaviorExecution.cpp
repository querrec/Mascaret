#include "VEHA/Behavior/BasicActions/DummyBehaviorExecution.h"
#include "VEHA/Kernel/InstanceSpecification.h"
#include "MascaretApplication.h"
#include <boost/bind.hpp>


namespace VEHA
{
DummyBehaviorExecution::DummyBehaviorExecution(shared_ptr<DummyAction> action,
		shared_ptr<InstanceSpecification> host, const Parameters& p):BehaviorExecution(action,host,p)
{
}

DummyBehaviorExecution::~DummyBehaviorExecution()
{
}

double DummyBehaviorExecution::execute(double dt)
{
	return 0.0;
}

} // Namespace
