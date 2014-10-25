#ifndef _v2_VEHA_Behavior_BasicAction_CallOperationBehaviorExecution_H
#define _v2_VEHA_Behavior_BasicAction_CallOperationBehaviorExecution_H

#include "Tools/veha_plateform.h"
#include "VEHA/Behavior/Common/BehaviorExecution.h"
#include "VEHA/Behavior/Common/Parameters.h"
#include "VEHA/Behavior/BasicActions/CallOperationAction.h"

namespace VEHA
{
class InstanceSpecification;

class VEHA_API CallOperationBehaviorExecution : public BehaviorExecution, boost::noncopyable
{
public :
	CallOperationBehaviorExecution(shared_ptr<CallOperationAction> action, shared_ptr<InstanceSpecification> host, const Parameters& p);
	virtual ~CallOperationBehaviorExecution();

	virtual void stop();
	virtual void restart();
	virtual void pause();

	virtual double execute(double dt);

	shared_ptr<CallOperationAction> _action;
	shared_ptr<BehaviorExecution> _behaviorExecution;
private :

};

}
#endif
