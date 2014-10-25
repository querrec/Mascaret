#ifndef _RestoreEnvironmentBehaviorExecution_H
#define _RestoreEnvironmentBehaviorExecution_H

#include "VEHA/Behavior/Common/BehaviorExecution.h"
#include "VEHA/Behavior/Common/Parameters.h"
#include "VEHA/Behavior/BasicActions/RestoreEnvironmentAction.h"


namespace VEHA
{
class Entity;

class VEHA_API RestoreEnvironmentBehaviorExecution : public BehaviorExecution, boost::noncopyable
{
public :
	RestoreEnvironmentBehaviorExecution(shared_ptr<RestoreEnvironmentAction> action, shared_ptr<InstanceSpecification> host, const Parameters& p);
	virtual ~RestoreEnvironmentBehaviorExecution();

	virtual double execute(double dt);

private :
	shared_ptr<RestoreEnvironmentAction> _action;
};

}
#endif
