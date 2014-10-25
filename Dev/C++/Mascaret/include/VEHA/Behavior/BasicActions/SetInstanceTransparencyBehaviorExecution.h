#ifndef _SetInstanceTransparencyBehaviorExecution_H
#define _SetInstanceTransparencyBehaviorExecution_H

#include "Tools/veha_plateform.h"
#include "VEHA/Behavior/Common/BehaviorExecution.h"
#include "VEHA/Behavior/Common/Parameters.h"
#include "VEHA/Behavior/BasicActions/SetInstanceTransparencyAction.h"


namespace VEHA
{
class Entity;
class VEHA_API SetInstanceTransparencyBehaviorExecution : public BehaviorExecution, boost::noncopyable
{
public :
	SetInstanceTransparencyBehaviorExecution(shared_ptr<SetInstanceTransparencyAction> action, shared_ptr<InstanceSpecification> host, const Parameters& p);
	virtual ~SetInstanceTransparencyBehaviorExecution();

	virtual double execute(double dt);

private :
	shared_ptr<SetInstanceTransparencyAction> _action;
	bool _first;
	long _firstTimeCalled;

};

}
#endif
