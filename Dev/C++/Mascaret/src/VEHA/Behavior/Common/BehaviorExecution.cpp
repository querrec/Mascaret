#include "VEHA/Behavior/Common/BehaviorExecution.h"
#include "VEHA/Behavior/Common/Behavior.h"
#include "VEHA/Behavior/Time/TimeExpression.h"

namespace VEHA
{

BehaviorExecution::BehaviorExecution(shared_ptr<Behavior> specif,shared_ptr<InstanceSpecification> host,const Parameters& p, bool sync)
:_specification(specif),_host(host),_sync(sync),_isFinished(false),_isSuspended(false),parameters(p),result()
{
}

BehaviorExecution::~BehaviorExecution()
{
}

void BehaviorExecution::stop(void)
{
	if(!_isFinished)
	{
		_isFinished=true;
		_cbOnBehaviorStop(this);
	}
}
void BehaviorExecution::pause(void)
{
	if(!_isSuspended)
	{
		_isSuspended=true;
		_cbOnBehaviorSuspended(this);
	}
}
void BehaviorExecution::restart(void)
{
	if(_isSuspended)
	{
		_isSuspended=false;
		_cbOnBehaviorRestarted(this);
	}
}
void BehaviorExecution::addCallbackOnBehaviorStart(const boost::function1<void, BehaviorExecution*> & f)
{
	_cbOnBehaviorStart.connect(f);
}

void BehaviorExecution::removeCallbackOnBehaviorStart(const boost::function1<void, BehaviorExecution*>& f)
{
	_cbOnBehaviorStart.disconnect(&f);
}

void BehaviorExecution::addCallbackOnBehaviorStop(const boost::function1<void, BehaviorExecution*> & f)
{
	_cbOnBehaviorStop.connect(f);
}

void BehaviorExecution::removeCallbackOnBehaviorStop(const boost::function1<void, BehaviorExecution*>& f)
{
	_cbOnBehaviorStop.disconnect(&f);
}
void BehaviorExecution::addCallbackOnBehaviorSuspended(const boost::function1<void, BehaviorExecution*> & f)
{
	_cbOnBehaviorSuspended.connect(f);
}

void BehaviorExecution::removeCallbackOnBehaviorSuspended(const boost::function1<void, BehaviorExecution*>& f)
{
	_cbOnBehaviorSuspended.disconnect(&f);
}
void BehaviorExecution::addCallbackOnBehaviorRestarted(const boost::function1<void, BehaviorExecution*> & f)
{
	_cbOnBehaviorRestarted.connect(f);
}
void BehaviorExecution::removeCallbackOnBehaviorRestarted(const boost::function1<void, BehaviorExecution*>& f)
{
	_cbOnBehaviorRestarted.disconnect(&f);
}

}
