#include "VEHA/Behavior/Common/Signal.h"
#include "VEHA/Behavior/BasicActions/SetInstanceInRedBehaviorExecution.h"
#include "VEHA/Entity/Entity.h"
#include "VEHA/Entity/ShapeSpecification.h"
#include "VEHA/Entity/EntityClass.h"
#include "VEHA/Kernel/Environment.h"
#include <boost/bind.hpp>


namespace VEHA
{

SetInstanceInRedBehaviorExecution::SetInstanceInRedBehaviorExecution(shared_ptr<SetInstanceInRedAction> action,
		shared_ptr<InstanceSpecification> host, const Parameters& p):BehaviorExecution(action,host,p), _action(action), _first(true)
{
}

SetInstanceInRedBehaviorExecution::~SetInstanceInRedBehaviorExecution()
{
}

double SetInstanceInRedBehaviorExecution::execute(double dt)
{
	double timeToWait = 3.0; //3 seconds default
	
	Parameters::iterator it;
	for (it=parameters.begin(); it!=parameters.end(); it++)
	{
		if ((*it).first == "time")
		{
			((*it).second)->getDoubleValue(timeToWait);
		}
	}
	
	shared_ptr<Entity> entity=shared_dynamic_cast<Entity>(_action->getTarget());
	
	if(entity && entity->getActiveShape())
	{
		if (_first)
		{
			_colorMemo = entity->getActiveShape()->getColor(); //save previous color
			entity->getActiveShape()->setColor(shared_ptr<Color>(new Color(1.0, 0.0, 0.0))); //set red color
			_first=false;
			
			time_t now;
			now = time(0);
			_firstTimeCalled = now;
			
			return 0.1;
		}
		else
		{
			time_t now;
			now = time(0);
			
			long delay = now-_firstTimeCalled;
	
			if (delay < timeToWait)
			{
				return 0.1; //wait until it's time
			}
		}
		
		//restore color
		entity->getActiveShape()->restaureColor(_colorMemo); //set red color ... for some reason it needs a parameter, but does not use it...
	}
	
	return 0;
}

} // Namespace
