#ifndef _v2_VEHA_Behavior_BasicAction_AcceptEventBehaviorExecution_H
#define _v2_VEHA_Behavior_BasicAction_AcceptEventBehaviorExecution_H

#include "VEHA/Behavior/Common/BehaviorExecution.h"
#include "VEHA/Behavior/Common/Parameters.h"
#include "VEHA/Behavior/BasicActions/AcceptEventAction.h"


namespace VEHA
{
class InstanceSpecification;

class VEHA_API AcceptEventBehaviorExecution : public BehaviorExecution, boost::noncopyable
{
public :
	AcceptEventBehaviorExecution(shared_ptr<AcceptEventAction> action, shared_ptr<InstanceSpecification> host, const Parameters& p);
	virtual ~AcceptEventBehaviorExecution();

	virtual double execute(double dt);

private :
	shared_ptr<AcceptEventAction> _action;
	bool _timeSpent;
};

}
#endif
