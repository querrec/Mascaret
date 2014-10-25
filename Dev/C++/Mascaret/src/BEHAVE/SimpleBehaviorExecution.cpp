#include "BEHAVE/SimpleBehaviorExecution.h"

namespace BEHAVE
{

SimpleBehaviorExecution::SimpleBehaviorExecution(shared_ptr<Behavior> specif,shared_ptr<InstanceSpecification> host, const Parameters& p) : AgentBehaviorExecution(specif,host,p), _interval(0.2)
{}

SimpleBehaviorExecution::~SimpleBehaviorExecution()
{}

double SimpleBehaviorExecution::execute(double dt)
{
	action();
	if (!done()) return _interval;
	else return 0;
}

}
