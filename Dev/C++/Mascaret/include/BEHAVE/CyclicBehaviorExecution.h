#ifndef BEHAVE_CYCLICBEHAVIOREXECUTION_H
#define BEHAVE_CYCLICBEHAVIOREXECUTION_H

#include "Tools/veha_plateform.h"
#include "BEHAVE/SimpleBehaviorExecution.h"

namespace BEHAVE
{
class VEHA_API CyclicBehaviorExecution : public SimpleBehaviorExecution
{
public :
	CyclicBehaviorExecution(shared_ptr<Behavior> specif,shared_ptr<InstanceSpecification> host, const Parameters& p);
	virtual ~CyclicBehaviorExecution();

public :
	virtual bool done(void);

};
}

#endif

