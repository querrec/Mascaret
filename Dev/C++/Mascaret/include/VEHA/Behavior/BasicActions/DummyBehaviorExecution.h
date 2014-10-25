#ifndef _v2_VEHA_Behavior_BasicAction_DummyBehaviorExecution_H
#define _v2_VEHA_Behavior_BasicAction_DummyBehaviorExecution_H

#include "VEHA/Behavior/Common/BehaviorExecution.h"
#include "VEHA/Behavior/Common/Parameters.h"
#include "VEHA/Behavior/BasicActions/DummyAction.h"


namespace VEHA
{
class InstanceSpecification;

class VEHA_API DummyBehaviorExecution : public BehaviorExecution, boost::noncopyable
{
public :
	DummyBehaviorExecution(shared_ptr<DummyAction> action, shared_ptr<InstanceSpecification> host, const Parameters& p);
	virtual ~DummyBehaviorExecution();

	virtual double execute(double dt);

private :
};

}
#endif
