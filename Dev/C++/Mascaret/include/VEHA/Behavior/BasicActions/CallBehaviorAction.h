#ifndef _v2_VEHA_Behavior_BasicActions_CallBehaviorAction_H
#define _v2_VEHA_Behavior_BasicActions_CallBehaviorAction_H

#include "Tools/veha_plateform.h"
#include "VEHA/Behavior/Common/Parameters.h"
#include "VEHA/Behavior/BasicActions/Action.h"

namespace VEHA
{

class Operation;
class InstanceSpecification;

class VEHA_API CallBehaviorAction : public Action
{
public :
	CallBehaviorAction();
	virtual ~CallBehaviorAction();

protected :
	shared_ptr<InstanceSpecification> _target;
	string _arguments;
	shared_ptr<Behavior> _behavior;
public :
	inline shared_ptr<InstanceSpecification> getTarget(void) { return _target;}
	inline void setTarget(shared_ptr<InstanceSpecification> target) { _target = target;}

	inline shared_ptr<Behavior> getBehavior(){return _behavior;}
	inline void setBehavior(shared_ptr <Behavior> behavior){_behavior = behavior;}

	virtual shared_ptr<BehaviorExecution> createBehaviorExecution(shared_ptr<InstanceSpecification>, const Parameters&, bool sync = false);

};

}
#endif
