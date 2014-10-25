#include "VEHA/Behavior/Common/behaviorScheduler.h"
#include "VEHA/Behavior/Common/BehaviorExecution.h"
#include "VEHA/Behavior/Common/Behavior.h"

#if defined(_WIN32) && !defined(__MINGW32__)
#include "Tools/utils.h"
#elif defined(__MINGW32__)
#include <unistd.h>
#include <sys/time.h>
#else
#include <unistd.h>
#include <sys/select.h>
#include <sys/time.h>
#endif
#include <boost/thread.hpp>
#include "VEHA/Kernel/InstanceSpecification.h"


#define max(x,y) ((x>y)?x:y)
#define min(x,y) ((x<y)?x:y)

namespace VEHA
{
shared_ptr<BehaviorScheduler> BehaviorScheduler::_instance = shared_ptr<BehaviorScheduler>();

BehaviorScheduler::BehaviorScheduler():
	_suspended(false),
	_virtualTimeSpeedFactor(1.0),
	_totalSuspendedRealTime(0),
	_cycle(0)
{
	TimeExpression _currentRealTime = TimeExpression::createTimeOfDay();

	_startRealTime = _currentRealTime;
	_lastRealTimeCheck = _currentRealTime;
	_startVirtualTime = _currentRealTime;
	_currentVirtualTime = _currentRealTime;
}

void BehaviorScheduler::destroyInstance()
{
	if (_instance)
		_instance=shared_ptr<BehaviorScheduler>();
}

BehaviorScheduler::~BehaviorScheduler()
{
	//cerr << __PRETTY_FUNCTION__ << endl;
	//	std::set<shared_ptr<BehaviorExecution> >::iterator it;
	//	for (it=_allBehaviors.begin();it!=_allBehaviors.end();it++)
		//		*it;
}

void BehaviorScheduler::step()
{
	if(!_suspended)
		_activate();
}

shared_ptr<BehaviorExecution> BehaviorScheduler::executeBehavior(shared_ptr<Behavior> behavior, shared_ptr<InstanceSpecification> host,const Parameters& p, bool sync)
{
	_updateCurrentVirtualTime();
	shared_ptr<BehaviorExecution> newBe=behavior->createBehaviorExecution(host,p,sync);
	if (newBe)
	{
		host->addExecution(newBe);
		newBe->setStart(_currentVirtualTime);
		newBe->setLastCallTime(_currentVirtualTime);
		if(sync)
		{
			newBe->execute(0.00);
		}
		else
		{
			//cerr << " BE Add : " << newBe << " : " << behavior->getName() << endl;
			_allBehaviors.insert(newBe);
			_insert(newBe,0.00);
		}
	}
	return newBe;
}

void BehaviorScheduler::suspendExecutionBehavior(shared_ptr<BehaviorExecution> be)
{
  if(!be) return;

	//_allBehaviors.erase(be);
	_suspendedBehaviors.push_back(be);
	be->addCallbackOnBehaviorRestarted(bind(&BehaviorScheduler::_onBehaviorRestarted,this,_1));
	list<SchedInfo>::iterator it=_behaviors.begin();
	for (;it!=_behaviors.end();it++)
	{
		if (it->be==be)
		{
			_behaviors.erase(it);
			return;
		}
	}
}

void BehaviorScheduler::deleteExecutionBehavior(shared_ptr<BehaviorExecution> be)
{
  if(!be) return;

	_allBehaviors.erase(be);
	list<SchedInfo>::iterator it=_behaviors.begin();
	for (;it!=_behaviors.end();it++)
	{
		if (it->be==be)
		{
			_behaviors.erase(it);
			return;
		}
	}
}

void BehaviorScheduler::reActivate(shared_ptr<BehaviorExecution> be)
{
  if(!be) return;

	deleteExecutionBehavior(be);
	//be->execute();
	_insert(be,0);
	_allBehaviors.insert(be);
}

void BehaviorScheduler::_insert(shared_ptr<BehaviorExecution> be,double dt)
{
  if(!be) return;

	_updateCurrentVirtualTime();
	TimeExpression nextTime = _currentVirtualTime + dt;

	list<SchedInfo>::iterator it=_behaviors.begin();
	for(;it!=_behaviors.end();it++)
	{
		if(nextTime < it->nextTime)
		{
			_behaviors.insert(it,SchedInfo(be,nextTime));
			return;
		}
	}
	_behaviors.push_back(SchedInfo(be,nextTime));
}

void BehaviorScheduler::setSuspended(bool suspend)
{
	TimeExpression _currentRealTime = TimeExpression::createTimeOfDay();
	if(_suspended && !suspend)
	{
		_updateCurrentVirtualTime();
		_totalSuspendedRealTime += _currentRealTime - _startSuspendTime;
		_suspended = false;
	}
	else if(!_suspended && suspend)
	{
		_updateCurrentVirtualTime();
		_startSuspendTime = TimeExpression::createTimeOfDay();
		_suspended = true;
	}
}

void BehaviorScheduler::setAccelerationFactor(double factor)
{
	_virtualTimeSpeedFactor = factor;
}

void BehaviorScheduler::forceCurrentVirtualTime(TimeExpression time)
{
	double dt = _currentVirtualTime - _startVirtualTime;

    std::list<SchedInfo> new_behaviors;
    std::list<SchedInfo>::iterator it = _behaviors.begin();

    while(it != _behaviors.end())
    {
        double dt = (*it).nextTime - _currentVirtualTime;
        TimeExpression nextTime = time;
        nextTime += dt;
        new_behaviors.push_back(SchedInfo((*it).be, nextTime));
        it++;
    }

    _behaviors = new_behaviors;

    _currentVirtualTime = time;
    time -= dt;
    _startVirtualTime = time;
}

double BehaviorScheduler::getCurrentTime()
{
	return getCurrentVirtualTime().getValue();
}

TimeExpression BehaviorScheduler::getCurrentVirtualTime()
{
	_updateCurrentVirtualTime();
	return _currentVirtualTime;
}

void BehaviorScheduler::_updateCurrentVirtualTime()
{
	TimeExpression _currentRealTime = TimeExpression::createTimeOfDay();

	if(!_suspended)
	{
		_currentVirtualTime += (_currentRealTime - _lastRealTimeCheck) * _virtualTimeSpeedFactor;
	}

	_lastRealTimeCheck = _currentRealTime;
}

void BehaviorScheduler::_jumpToNext()
{
	_updateCurrentVirtualTime();
	double next_dt=0.02;
	if(!_behaviors.empty())
	{
		next_dt = max(0,min((_behaviors.begin()->nextTime - _currentVirtualTime) / _virtualTimeSpeedFactor, 0.02));
	}

#if defined(_WIN32) && !defined(__MINGW32__)
	Sleep((int)(1000*next_dt));
#else
	usleep(1000000*next_dt);
#endif
}

void BehaviorScheduler::_activate()
{
	//_checkReactivateExecutions();
	_jumpToNext();
	_updateCurrentVirtualTime();

	while(!_behaviors.empty())
	{
		TimeExpression virtualTime = getCurrentVirtualTime();

		//cerr << "Schedul : " << _cycle << "Size: " << _behaviors.size() << endl;
		SchedInfo si  = _behaviors.front();
		//cerr << "Behavior: "<< si.be->getHost()->getName() << "-->" << si.be->getSpecification()->getName()<< " nextTime: " << (si.nextTime - virtualTime) << endl;

		if (si.nextTime > virtualTime)
		{
			break;
		}
		else
		{
			if(si.be->isSuspended())
			{
			//	cerr << "Behavior suspended: "<< si.be->getHost()->getName() << "-->" << si.be->getSpecification()->getName()<<endl;
				suspendExecutionBehavior(si.be);
				continue;
			}
			if(si.be->isFinished())
			{
				//cerr << "Behavior finished: "<< si.be->getHost()->getName() << "-->" << si.be->getSpecification()->getName()<<endl;
				si.be->setFinish(virtualTime);
				
				deleteExecutionBehavior(si.be);
				continue;
			} 
			//cerr << "executeBehavior "<<si.be->getSpecification()->getName()<<" on "<<si.be->getHost()->getName()<<endl;

			double dt = virtualTime - si.be->getLastCallTime();
			double next_dt = si.be->execute(dt);
			si.be->setLastCallTime(virtualTime);

			if (next_dt > 0)
			{
				// BehaviorExecution ask to be execute in next_dt seconds, so we reinsert it in the list
				_insert(si.be,next_dt);
			}
			else
			{
				// BehaviorExecution have finish
				//cout << "Delete behavior " << si.be->getSpecification()->getName() << " " <<  si.be.use_count() << " " << si.be.get() << endl;
				si.be->stop();
				si.be->setFinish(virtualTime);
				_allBehaviors.erase(si.be);
				//cout << "Delete behavior " << si.be->getSpecification()->getName() << " " <<  si.be.use_count() << " " << si.be.get() << endl;
			}
			//cout << "befor pop_front behavior " << si.be->getSpecification()->getName() << " " <<  si.be.use_count() << " " << si.be.get() << endl;
			_behaviors.pop_front();
		}
	}
	_cycle++;
}

void BehaviorScheduler::_onBehaviorRestarted(BehaviorExecution* be)
{
  if(!be) return;

	for(size_t i=_suspendedBehaviors.size();i--;)
	{
		if(_suspendedBehaviors[i].get() == be)
		{
			_insert(_suspendedBehaviors[i],0);
			_suspendedBehaviors[i]=_suspendedBehaviors.back();
			_suspendedBehaviors.pop_back();
		}
	}
}

}
