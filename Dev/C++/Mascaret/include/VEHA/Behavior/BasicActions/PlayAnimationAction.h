#ifndef _v2_VEHA_Behavior_BasicActions_PlayAnimationAction_H
#define _v2_VEHA_Behavior_BasicActions_PlayAnimationAction_H

#include "Tools/veha_plateform.h"
#include "VEHA/Behavior/BasicActions/Action.h"

namespace VEHA
{
class InstanceSpecification;
class Signal;

class VEHA_API PlayAnimationAction : public Action
{
public :
	PlayAnimationAction();
	virtual ~PlayAnimationAction();

protected :
	string _animationName;
public :
	inline string getAnimationName(void) { return _animationName;}
	inline void setAnimationName(string animationName) { _animationName = animationName;}

	virtual shared_ptr<BehaviorExecution> createBehaviorExecution(shared_ptr<InstanceSpecification>, const Parameters&, bool sync = false);

};

}
#endif
