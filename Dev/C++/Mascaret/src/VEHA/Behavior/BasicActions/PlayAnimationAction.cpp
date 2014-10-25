#include "VEHA/Behavior/BasicActions/PlayAnimationAction.h"
#include "VEHA/Behavior/BasicActions/PlayAnimationBehaviorExecution.h"

namespace VEHA
{

PlayAnimationAction::PlayAnimationAction()
{
	_kind = "PlayAnimation";
}

PlayAnimationAction::~PlayAnimationAction()
{
}

shared_ptr<BehaviorExecution> PlayAnimationAction::createBehaviorExecution(shared_ptr<InstanceSpecification> host, const Parameters& p, bool sync)
{
	shared_ptr<PlayAnimationBehaviorExecution> behavior(new PlayAnimationBehaviorExecution(shared_dynamic_cast<PlayAnimationAction>(shared_from_this()),host,p));
	return behavior;
}

}
