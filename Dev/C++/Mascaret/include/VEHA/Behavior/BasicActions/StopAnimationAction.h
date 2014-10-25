#ifndef _v2_VEHA_Behavior_BasicActions_StopAnimationAction_H
#define _v2_VEHA_Behavior_BasicActions_StopAnimationAction_H

#include "Tools/veha_plateform.h"
#include "VEHA/Behavior/BasicActions/Action.h"

namespace VEHA
{
class InstanceSpecification;
class Signal;

class VEHA_API StopAnimationAction : public Action
{
public :
	StopAnimationAction();
	virtual ~StopAnimationAction();

protected :
	string _animationName;
public :
	inline string getAnimationName(void) { return _animationName;}
	inline void setAnimationName(string animationName) { _animationName = animationName;}

	virtual shared_ptr<BehaviorExecution> createBehaviorExecution(shared_ptr<InstanceSpecification>, const Parameters&, bool sync = false);

};

}
#endif
