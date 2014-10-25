#ifndef _v2_VEHA_Behavior_BasicAction_CallBehaviorBehaviorExecution_H
#define _v2_VEHA_Behavior_BasicAction_CallBehaviorBehaviorExecution_H

#include "Tools/veha_plateform.h"
#include "VEHA/Behavior/Common/BehaviorExecution.h"
#include "VEHA/Behavior/Common/Parameters.h"
#include "VEHA/Behavior/BasicActions/CallBehaviorAction.h"

namespace VEHA
{
class InstanceSpecification;

class VEHA_API CallBehaviorBehaviorExecution : public BehaviorExecution, boost::noncopyable
{
public :
	CallBehaviorBehaviorExecution(shared_ptr<CallBehaviorAction> action, shared_ptr<InstanceSpecification> host, const Parameters& p);
	virtual ~CallBehaviorBehaviorExecution();

	virtual void stop();
	virtual void restart();
	virtual void pause();

	virtual double execute(double dt);

	shared_ptr<CallBehaviorAction> _action;
	shared_ptr<BehaviorExecution> _behaviorExecution;
private :

	bool _started;
	TimeExpression _startTimestamp;
	size_t _lastCheckedMailIndex;
	vector<string> _mustReceiveTheseMessagesToFinish;

};

}
#endif
