#ifndef _v2_VEHA_Behavior_Activity_LoopNodeBehavior_H
#define _v2_VEHA_Behavior_Activity_LoopNodeBehavior_H

#include "Tools/veha_plateform.h"
#include "VEHA/Behavior/Common/Behavior.h"
#include "VEHA/Behavior/Activity/LoopNode.h"


namespace VEHA
{
class VEHA_API LoopNodeBehavior : public Behavior
{
public :
	LoopNodeBehavior(const string& name, shared_ptr<LoopNode> loopNode, map < string, shared_ptr<InstanceSpecification> > affectations);
	virtual ~LoopNodeBehavior();

        inline shared_ptr<LoopNode> getLoopNode() { return _loopNode;}

        map < string, shared_ptr<InstanceSpecification> > _affectations;

protected :
	shared_ptr<LoopNode> _loopNode;
public :
	virtual shared_ptr<BehaviorExecution> createBehaviorExecution(shared_ptr<InstanceSpecification> host,const Parameters& p, bool sync =false);
};

}
#endif
