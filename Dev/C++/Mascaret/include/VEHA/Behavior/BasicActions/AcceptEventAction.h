#ifndef _v2_VEHA_Behavior_BasicActions_AcceptEventAction_H
#define _v2_VEHA_Behavior_BasicActions_AcceptEventAction_H

#include "Tools/veha_plateform.h"
#include "VEHA/Behavior/BasicActions/Action.h"

namespace VEHA
{
class Trigger;

class VEHA_API AcceptEventAction : public Action
{
public :
	AcceptEventAction();
	virtual ~AcceptEventAction();
protected :
	shared_ptr<Trigger>    _trigger;

public :
	inline shared_ptr<Trigger> getTrigger(void) { return _trigger;}
	inline void setTrigger(shared_ptr<Trigger> trigger) { _trigger = trigger;}
	virtual shared_ptr<BehaviorExecution> createBehaviorExecution(shared_ptr<InstanceSpecification>, const Parameters&, bool sync = false);
};

}
#endif
