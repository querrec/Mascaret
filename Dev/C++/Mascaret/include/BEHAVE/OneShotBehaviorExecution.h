#ifndef BEHAVE_ONESHOTBEHAVIOREXECUTION_H
#define BEHAVE_ONESHOTBEHAVIOREXECUTION_H

#include "Tools/veha_plateform.h"
#include "BEHAVE/SimpleBehaviorExecution.h"

namespace BEHAVE
{
class VEHA_API OneShotBehaviorExecution : public SimpleBehaviorExecution
{
public :
	OneShotBehaviorExecution(shared_ptr<Behavior> specif,shared_ptr<InstanceSpecification> host, const Parameters& p);
	virtual ~OneShotBehaviorExecution();

public :
	//virtual void action(void) = 0;
	virtual bool done(void);

};
}

#endif

