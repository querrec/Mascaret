#ifndef _v2_VEHA_Behavior_BasicAction_StopAnimationBehaviorExecution_H
#define _v2_VEHA_Behavior_BasicAction_StopAnimationBehaviorExecution_H

#include "VEHA/Behavior/Common/BehaviorExecution.h"
#include "VEHA/Behavior/Common/Parameters.h"
#include "VEHA/Behavior/BasicActions/StopAnimationAction.h"


namespace VEHA
{
class InstanceSpecification;

class VEHA_API StopAnimationBehaviorExecution : public BehaviorExecution, boost::noncopyable
{
public :
	StopAnimationBehaviorExecution(shared_ptr<StopAnimationAction> action, shared_ptr<InstanceSpecification> host, const Parameters& p);
	virtual ~StopAnimationBehaviorExecution();

	virtual double execute(double dt);

private :
	shared_ptr<StopAnimationAction> _action;
};

}
#endif
