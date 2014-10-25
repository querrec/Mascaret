#ifndef BEHAVE_AGENTBEHAVIOREXECUTION_H
#define BEHAVE_AGENTBEHAVIOREXECUTION_H

#include "Tools/veha_plateform.h"
#include "VEHA/Behavior/Common/Behavior.h"
#include "VEHA/Behavior/Common/BehaviorExecution.h"


namespace BEHAVE
{
using VEHA::Behavior;
using VEHA::InstanceSpecification;
using VEHA::Parameters;
class VEHA_API AgentBehaviorExecution : public VEHA::BehaviorExecution
{
public :
	AgentBehaviorExecution(shared_ptr<Behavior> specif,shared_ptr<InstanceSpecification> host, const Parameters& p);
	virtual ~AgentBehaviorExecution();

public :
	//         virtual double execute();
	virtual void action(void) = 0;
	virtual bool done(void) = 0;

};
}

#endif

