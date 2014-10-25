#include "VEHA/Behavior/Activity/ActionNode.h"
#include "VEHA/Kernel/Expression.h"
#include "VEHA/Behavior/Common/Signal.h"
#include "VEHA/Behavior/Common/Trigger.h"
#include "VEHA/Behavior/BasicActions/AcceptEventBehaviorExecution.h"
#include "VEHA/Behavior/StateMachine/StateMachineBehaviorExecution.h"
#include "VEHA/Behavior/BasicActions/AcceptEventAction.h"
#include "VEHA/Behavior/StateMachine/ChangeEvent.h"
#include "VEHA/Behavior/StateMachine/TimeEvent.h"
#include "VEHA/Kernel/InstanceSpecification.h"
#include "VEHA/Kernel/Environment.h"
#include "VEHA/Kernel/Expression.h"
#include "MascaretApplication.h"
#include <boost/bind.hpp>


namespace VEHA
{
AcceptEventBehaviorExecution::AcceptEventBehaviorExecution(shared_ptr<AcceptEventAction> action,
		shared_ptr<InstanceSpecification> host, const Parameters& p):BehaviorExecution(action,host,p), _action(action),_timeSpent(false)
{
}

AcceptEventBehaviorExecution::~AcceptEventBehaviorExecution()
{
}

double AcceptEventBehaviorExecution::execute(double dt)
{
	OclParser::Context context = _action->getContext();

	bool result = false;
	double time = 0.2;


	if(_action->getTrigger()->getEvent()->getType() == "ChangeEvent")
	{
		shared_ptr<ChangeEvent> changeEvent = shared_dynamic_cast<ChangeEvent>(_action->getTrigger()->getEvent());
		shared_ptr<Expression> expr = shared_dynamic_cast<Expression>(changeEvent->getChangeExpression());
		result =expr->evaluateExpression(context);
		cerr<<"Checking context of AcceptEventAction "<<changeEvent->getType()<<endl;
	}
	else if(_action->getTrigger()->getEvent()->getType() == "TimeEvent")
	{
		if(!_timeSpent)
		{
			shared_ptr<TimeEvent> timeEvent = shared_dynamic_cast<TimeEvent>(_action->getTrigger()->getEvent());
			timeEvent->getWhen()->getDoubleValue(time);
			if(time!=0)
				_timeSpent= true;
			cerr<<"Checking context of AcceptEventAction "<<timeEvent->getType()<<" time to spend : "<<time<<endl;
		}
		else
		{
			time = 0;
			cerr<<"TimeEvent is now checked."<<endl;
		}
	}
	else
		cerr<<"ERROR: This trigger event is not an AcceptEventAction event. This event should be a ChangeEvent. (Is type is " << _action->getTrigger()->getEvent()->getType() << ")" << endl;

	if (result)
		return 0;
	else
		return time;
}

} // Namespace
