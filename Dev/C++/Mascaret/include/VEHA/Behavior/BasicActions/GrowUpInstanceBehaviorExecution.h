#ifndef _SetInstanceInRedBehaviorExecution_H
#define _SetInstanceInRedBehaviorExecution_H
#include "Tools/veha_plateform.h"
#include "VEHA/Behavior/Common/BehaviorExecution.h"
#include "VEHA/Behavior/Common/Parameters.h"
#include "VEHA/Behavior/BasicActions/GrowUpInstanceAction.h"


namespace VEHA
{
class Entity;
class VEHA_API GrowUpInstanceBehaviorExecution : public BehaviorExecution, boost::noncopyable
{
public :
	GrowUpInstanceBehaviorExecution(shared_ptr<GrowUpInstanceAction> action, shared_ptr<InstanceSpecification> host, const Parameters& p);
	virtual ~GrowUpInstanceBehaviorExecution();

	virtual double execute(double dt);

private :
	shared_ptr<GrowUpInstanceAction> _action;
	bool _first;
};
}
#endif
