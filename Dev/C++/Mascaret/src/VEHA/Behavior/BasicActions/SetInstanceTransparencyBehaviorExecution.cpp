#include "VEHA/Behavior/Common/Signal.h"
#include "VEHA/Behavior/BasicActions/SetInstanceTransparencyBehaviorExecution.h"
#include "VEHA/Entity/Entity.h"
#include "VEHA/Entity/EntityClass.h"
#include "VEHA/Entity/ShapeSpecification.h"
#include "VEHA/Kernel/Environment.h"
#include <boost/bind.hpp>

#include "MascaretApplication.h"

namespace VEHA
{

SetInstanceTransparencyBehaviorExecution::SetInstanceTransparencyBehaviorExecution(shared_ptr<SetInstanceTransparencyAction> action,
		shared_ptr<InstanceSpecification> host, const Parameters& p):BehaviorExecution(action,host,p), _action(action), _first(true)
{
}

SetInstanceTransparencyBehaviorExecution::~SetInstanceTransparencyBehaviorExecution()
{
}

double SetInstanceTransparencyBehaviorExecution::execute(double dt)
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
			map<string,shared_ptr<InstanceSpecification> > instances=MascaretApplication::getInstance()->getEnvironment()->getInstances();
    		map<string,shared_ptr<InstanceSpecification> >::iterator it=instances.begin();
    		while(it != instances.end())
    		{
    			shared_ptr<Entity> entity2=shared_dynamic_cast<Entity>(it->second);
    			if(entity2 && entity != entity2 && entity2->getActiveShape())
    			{
    				entity2->getActiveShape()->setTransparent(shared_ptr<Color>(new Color(0.5f)));
    			}
    			it++;
    		}
			
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
		
		//restore
		map<string,shared_ptr<InstanceSpecification> > instances=MascaretApplication::getInstance()->getEnvironment()->getInstances();
		map<string,shared_ptr<InstanceSpecification> >::iterator it=instances.begin();
		while(it != instances.end())
		{
			shared_ptr<Entity> entity2=shared_dynamic_cast<Entity>(it->second);
			if(entity2 && entity != entity2 && entity2->getActiveShape())
			{
				entity2->getActiveShape()->setTransparent(shared_ptr<Color>(new Color(0.0f))); //BAD TODO
			}
			it++;
		}
	}
	
	return 0;

		/*shared_ptr<Entity> entity=shared_dynamic_cast<Entity>(_action->getTarget());
		        	if(!_first)
		        	{
		        		Interface3D::get()->restoreAll();
		        		return 0;
		        	}
		        	if(entity && entity->getActiveShape())
		        	{
		        		map<string,shared_ptr<InstanceSpecification> > instances=MascaretApplication::getInstance()->world->getInstances();
		        		map<string,shared_ptr<InstanceSpecification> >::iterator it=instances.begin();
		        		while(it != instances.end())
		        		{
		        			shared_ptr<Entity> entity2=shared_dynamic_cast<Entity>(it->second);
		        			if(entity2 && entity != entity2 && entity2->getActiveShape())
		        			{
		        				Interface3D::get()->setTransparency(entity2->getActiveShape()->getGID(),0.7f);
		        			}
		        			it++;
		        		}
		        	}
		        	else*/
		return 0;
		_first=false;
		return 5;
}

} // Namespace
