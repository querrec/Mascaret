#include "VEHA/Behavior/BasicActions/CallOperationBehaviorExecution.h"
#include "VEHA/Kernel/InstanceSpecification.h"
#include "VEHA/Kernel/Operation.h"
#include <boost/bind.hpp>


namespace VEHA
{

CallOperationBehaviorExecution::CallOperationBehaviorExecution(shared_ptr<CallOperationAction> action, shared_ptr<InstanceSpecification> host, const Parameters& p):BehaviorExecution(action,host,p), _action(action)
{
	_behaviorExecution=_action->getOperation()->getMethod()->createBehaviorExecution(getHost(),parameters);
}

CallOperationBehaviorExecution::~CallOperationBehaviorExecution()
{
}

void CallOperationBehaviorExecution::stop()
{
	BehaviorExecution::stop();
	_behaviorExecution->stop();
}
void CallOperationBehaviorExecution::restart()
{
	BehaviorExecution::restart();
	_behaviorExecution->restart();
}
void CallOperationBehaviorExecution::pause()
{
	BehaviorExecution::pause();
	_behaviorExecution->pause();
}

double CallOperationBehaviorExecution::execute(double dt)
{
	return _behaviorExecution->execute(dt);
}

} // Namespace
