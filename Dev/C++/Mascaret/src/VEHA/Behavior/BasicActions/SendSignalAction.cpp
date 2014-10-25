#include "VEHA/Behavior/BasicActions/SendSignalAction.h"
#include "VEHA/Behavior/BasicActions/SendSignalBehaviorExecution.h"
#include "VEHA/Behavior/Common/Signal.h"

namespace VEHA
{

SendSignalAction::SendSignalAction()
{
	_kind = "SendSignal";
}

SendSignalAction::~SendSignalAction()
{
}

shared_ptr<BehaviorExecution> SendSignalAction::createBehaviorExecution(shared_ptr<InstanceSpecification> host, const Parameters& p, bool sync)
{
	shared_ptr<SendSignalBehaviorExecution> behavior(new SendSignalBehaviorExecution(shared_dynamic_cast<SendSignalAction>(shared_from_this()),host,p));
	return behavior;
}

bool SendSignalAction::equalsTo(shared_ptr<Behavior> behavior)
{
	shared_ptr<SendSignalAction> sigAction = shared_dynamic_cast<SendSignalAction>(behavior);
	if (sigAction)
	{
		shared_ptr<Signal> signal = sigAction->getSignalClass();
		if (signal) 
		{
			cerr << " &&&&&& " << signal->getName() << " == " << getSignalClass()->getName() << endl;
			if (signal->getName() == getSignalClass()->getName())
			{
				
				string targetName = sigAction->getTarget()->getClassifier()->getName(); 
			}
		}
	}
	else
		return false;
}

}
