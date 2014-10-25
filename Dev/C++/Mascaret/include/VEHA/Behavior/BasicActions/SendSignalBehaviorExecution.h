#ifndef _v2_VEHA_Behavior_BasicAction_SendSignalBehaviorExecution_H
#define _v2_VEHA_Behavior_BasicAction_SendSignalBehaviorExecution_H

#include "VEHA/Behavior/Common/BehaviorExecution.h"
#include "VEHA/Behavior/Common/Parameters.h"
#include "VEHA/Behavior/BasicActions/SendSignalAction.h"


namespace VEHA
{
class InstanceSpecification;

class VEHA_API SendSignalBehaviorExecution : public BehaviorExecution, boost::noncopyable
{
public :
	SendSignalBehaviorExecution(shared_ptr<SendSignalAction> action, shared_ptr<InstanceSpecification> host, const Parameters& p);
	virtual ~SendSignalBehaviorExecution();

	virtual double execute(double dt);

private :
	shared_ptr<SendSignalAction> _action;
};

}
#endif
