#ifndef _SetInstanceInRedBehaviorExecution_H
#define _SetInstanceInRedBehaviorExecution_H

#include "Tools/veha_plateform.h"
#include "VEHA/Behavior/Common/BehaviorExecution.h"
#include "VEHA/Behavior/Common/Parameters.h"
#include "VEHA/Behavior/BasicActions/SetInstanceInRedAction.h"

#include "VEHA/Entity/Color.h"

namespace VEHA
{
class Entity;

class VEHA_API SetInstanceInRedBehaviorExecution : public BehaviorExecution, boost::noncopyable
{
public :
	SetInstanceInRedBehaviorExecution(shared_ptr<SetInstanceInRedAction> action, shared_ptr<InstanceSpecification> host, const Parameters& p);
	virtual ~SetInstanceInRedBehaviorExecution();

	virtual double execute(double dt);

private :
	shared_ptr<SetInstanceInRedAction> _action;
	bool _first;
	shared_ptr<Color> _colorMemo;
	long _firstTimeCalled;
};

}
#endif
