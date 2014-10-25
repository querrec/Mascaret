#ifndef BEHAVE_AGENTBEHAVIORCONTROLLER_H
#define BEHAVE_AGENTBEHAVIORCONTROLLER_H

#include "Tools/veha_plateform.h"
#include "VEHA/Behavior/Common/BehaviorExecution.h"
using namespace VEHA;

namespace BEHAVE
{

class VEHA_API AgentBehaviorController
{
public :
	AgentBehaviorController();
	virtual ~AgentBehaviorController();

	// Behavior Executions
public :
	inline void addBehaviorExecution(shared_ptr<BehaviorExecution> execution) {_executions.push_back(execution);}
	inline vector<shared_ptr<BehaviorExecution> > getExecutions(void) {return _executions;}

private :
	vector< shared_ptr<BehaviorExecution> > _executions;

};
}

#endif

