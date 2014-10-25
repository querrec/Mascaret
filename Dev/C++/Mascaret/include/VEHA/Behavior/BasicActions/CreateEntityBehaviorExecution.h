#ifndef _v2_VEHA_Behavior_BasicAction_CreateEntityBehaviorExecution_H
#define _v2_VEHA_Behavior_BasicAction_CreateEntityBehaviorExecution_H

#include "VEHA/Behavior/Common/BehaviorExecution.h"
#include "VEHA/Behavior/Common/Parameters.h"
#include "VEHA/Behavior/BasicActions/CreateEntityAction.h"

namespace VEHA
{

class Entity;

class VEHA_API CreateEntityBehaviorExecution : public BehaviorExecution, boost::noncopyable
{
public :
	CreateEntityBehaviorExecution(shared_ptr<CreateEntityAction> action, shared_ptr<InstanceSpecification> host, const Parameters& p);
	virtual ~CreateEntityBehaviorExecution();

	virtual double execute(double dt);

private :
	shared_ptr<CreateEntityAction> _action;
};

}
#endif
