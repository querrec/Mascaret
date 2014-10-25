#include "BEHAVE/AgentBehaviorExecution.h"

namespace BEHAVE
{

AgentBehaviorExecution::AgentBehaviorExecution(shared_ptr<Behavior> specif,shared_ptr<InstanceSpecification> host, const Parameters& p) : BehaviorExecution(specif,host,p)
{}

AgentBehaviorExecution::~AgentBehaviorExecution()
{}

/*
double AgentBehaviorExecution::execute()
{
   action();
   if (!done()) return 0.2;
   else return 0;
}
 */

}
