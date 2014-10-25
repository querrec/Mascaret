#include "BEHAVE/CyclicBehaviorExecution.h"

namespace BEHAVE
{

CyclicBehaviorExecution::CyclicBehaviorExecution(shared_ptr<Behavior> specif,shared_ptr<InstanceSpecification> host, const Parameters& p) : SimpleBehaviorExecution(specif,host,p)
{}

CyclicBehaviorExecution::~CyclicBehaviorExecution()
{}

bool CyclicBehaviorExecution::done()
{
	return false;
}

}
