#include "VEHA/Behavior/Activity/LoopNode.h"
#include "VEHA/Behavior/Activity/LoopNodeBehavior.h"
#include "VEHA/Behavior/Activity/LoopNodeBehaviorExecution.h"


namespace VEHA
{

LoopNodeBehavior::LoopNodeBehavior(const string& name, shared_ptr<LoopNode> loopNode,map < string, shared_ptr<InstanceSpecification> > affectations) : Behavior(name), _affectations(affectations), _loopNode(loopNode)
{
}

LoopNodeBehavior::~LoopNodeBehavior()
{
}

shared_ptr<BehaviorExecution>  LoopNodeBehavior::createBehaviorExecution(shared_ptr<InstanceSpecification> host,const Parameters& p, bool sync)
{

   shared_ptr<LoopNodeBehaviorExecution> behavior(new LoopNodeBehaviorExecution(shared_dynamic_cast<LoopNodeBehavior>(shared_from_this()),host,p,sync));
   return behavior;

}

}
