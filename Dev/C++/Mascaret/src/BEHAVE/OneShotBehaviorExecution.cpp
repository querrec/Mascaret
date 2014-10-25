#include "BEHAVE/OneShotBehaviorExecution.h"

namespace BEHAVE
{

OneShotBehaviorExecution::OneShotBehaviorExecution(shared_ptr<Behavior> specif,shared_ptr<InstanceSpecification> host, const Parameters& p) : SimpleBehaviorExecution(specif,host,p)
{}

OneShotBehaviorExecution::~OneShotBehaviorExecution()
{}

bool OneShotBehaviorExecution::done()
{
	return true;
}

}
