#include "VEHA/Behavior/Common/behaviorScheduler.h"
#include "VEHA/Behavior/Activity/LoopNode.h"
#include "VEHA/Behavior/Activity/LoopNodeBehavior.h"
#include "VEHA/Behavior/Activity/Activity.h"


namespace VEHA
{

LoopNode::LoopNode(const string& name, const string & kind) : ActionNode(name,kind),
	_testedFirst(false)
{
}

LoopNode::~LoopNode()
{
}

void LoopNode::start(shared_ptr<InstanceSpecification> host, map < string, shared_ptr<InstanceSpecification> > affectations, bool sync)
{
   cerr<<" ##### Loop Node : " << getName() << endl;
   /*
   cerr << _finalCondition->getExpression() << endl;

   OclParser::Context context = getActivity()->getContext();
   _condition->evaluateExpression(context); 
   //bool result = _finalCondition->evaluateExpression(context); 

   
   Parameters param;
   shared_ptr<LoopNodeBehavior> lnb (new LoopNodeBehavior(getName(), shared_dynamic_cast<LoopNode>(shared_from_this()), affectations));
   _currentExecution = BehaviorScheduler::getInstance()->executeBehavior(lnb, host, param,sync);
   */
}

void LoopNode::stop(void)
{
}

bool LoopNode::eval(void)
{
   OclParser::Context context = getActivity()->getContext();

   map<string,AnyValue>::iterator it = context.begin();
   while (it!=context.end()) 
   {
	cerr << it->first << " : " << it->second << endl;
        it++;
   }

   if(!_condition)
   {
       cerr << "[LoopNode WARNING] " << getName() << ", no condition found. (return as valid by default)." << endl;
       return true;
   }

   return _condition->evaluateExpression(context);
}
}
