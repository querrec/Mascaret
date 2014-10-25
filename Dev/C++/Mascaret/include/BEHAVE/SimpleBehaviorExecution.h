#ifndef BEHAVE_SIMPLEBEHAVIOREXECUTION_H
#define BEHAVE_SIMPLEBEHAVIOREXECUTION_H


#include "Tools/veha_plateform.h"
#include "VEHA/Behavior/Common/Behavior.h"
#include "VEHA/Behavior/Common/BehaviorExecution.h"
#include "BEHAVE/AgentBehaviorExecution.h"

namespace BEHAVE
{
using VEHA::InstanceSpecification;
using VEHA::Parameters;
class VEHA_API SimpleBehaviorExecution : public AgentBehaviorExecution
{
public :
	SimpleBehaviorExecution(shared_ptr<Behavior> specif,shared_ptr<InstanceSpecification> host, const Parameters& p);
	virtual ~SimpleBehaviorExecution();

public :
	virtual double execute(double dt);
	virtual void setInterval(double dt);
	virtual double getInterval();
protected:
	double _interval;
};
inline void SimpleBehaviorExecution::setInterval(double dt)
{
	_interval=dt;
}
inline double SimpleBehaviorExecution::getInterval()
{
	return _interval;
}
}

#endif

