#ifndef _v2_VEHA_Behavior_BasicAction_PlayAnimationBehaviorExecution_H
#define _v2_VEHA_Behavior_BasicAction_PlayAnimationBehaviorExecution_H

#include "VEHA/Behavior/Common/BehaviorExecution.h"
#include "VEHA/Behavior/Common/Parameters.h"
#include "VEHA/Behavior/BasicActions/PlayAnimationAction.h"


namespace VEHA
{
class InstanceSpecification;

class VEHA_API PlayAnimationBehaviorExecution : public BehaviorExecution, boost::noncopyable
{
public :
	PlayAnimationBehaviorExecution(shared_ptr<PlayAnimationAction> action, shared_ptr<InstanceSpecification> host, const Parameters& p);
	virtual ~PlayAnimationBehaviorExecution();

	virtual double execute(double dt);

private :
	shared_ptr<PlayAnimationAction> _action;
};

}
#endif
