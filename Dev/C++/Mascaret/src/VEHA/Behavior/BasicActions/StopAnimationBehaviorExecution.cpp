#include "VEHA/Behavior/Common/Signal.h"
#include "VEHA/Behavior/BasicActions/StopAnimationBehaviorExecution.h"
#include "VEHA/Behavior/StateMachine/StateMachineBehaviorExecution.h"
#include "VEHA/Kernel/InstanceSpecification.h"
#include "VEHA/Kernel/Environment.h"
#include "VEHA/Entity/Entity.h"
#include "HAVE/VirtualHuman.h"
#include "HAVE/Body.h"
#include <boost/bind.hpp>


namespace VEHA
{

StopAnimationBehaviorExecution::StopAnimationBehaviorExecution(shared_ptr<StopAnimationAction> action,
		shared_ptr<InstanceSpecification> host, const Parameters& p):BehaviorExecution(action,host,p), _action(action)
{
}

StopAnimationBehaviorExecution::~StopAnimationBehaviorExecution()
{
}

double StopAnimationBehaviorExecution::execute(double dt)
{
	cerr << " Stoping Animation : " << _action->getAnimationName() << endl;
	if(shared_dynamic_cast<Entity>(getHost()))
		(shared_dynamic_cast<Entity>(getHost()))->stopAnimation(_action->getAnimationName());
	else if(shared_dynamic_cast<HAVE::VirtualHuman>(getHost()))
		(shared_dynamic_cast<HAVE::VirtualHuman>(getHost()))->getBody()->stopAnimation(_action->getAnimationName());
	return 0;
}

} // Namespace
