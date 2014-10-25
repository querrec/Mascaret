#ifndef _v2_VEHA_Behavior_BasicActions_DummyAction_H
#define _v2_VEHA_Behavior_BasicActions_DummyAction_H

#include "Tools/veha_plateform.h"
#include "VEHA/Behavior/BasicActions/Action.h"

namespace VEHA
{
class Trigger;

class VEHA_API DummyAction : public Action
{
public :
	DummyAction();
	virtual ~DummyAction();

public :
	virtual shared_ptr<BehaviorExecution> createBehaviorExecution(shared_ptr<InstanceSpecification>, const Parameters&, bool sync = false);
};

}
#endif
