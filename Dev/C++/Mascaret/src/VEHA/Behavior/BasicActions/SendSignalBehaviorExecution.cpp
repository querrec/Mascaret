#include "VEHA/Behavior/Common/Signal.h"
#include "VEHA/Behavior/BasicActions/SendSignalBehaviorExecution.h"
#include "VEHA/Behavior/StateMachine/StateMachineBehaviorExecution.h"
#include "VEHA/Kernel/InstanceSpecification.h"
#include "VEHA/Kernel/Environment.h"
#include "MascaretApplication.h"
#include <boost/bind.hpp>


namespace VEHA
{
SendSignalBehaviorExecution::SendSignalBehaviorExecution(shared_ptr<SendSignalAction> action,
		shared_ptr<InstanceSpecification> host, const Parameters& p):BehaviorExecution(action,host,p), _action(action)
{
}

SendSignalBehaviorExecution::~SendSignalBehaviorExecution()
{
}

double SendSignalBehaviorExecution::execute(double dt)
{
	shared_ptr<InstanceSpecification> instance=_action->getTarget();
	if(instance)
	{
		cerr << " Send Signal Action" << endl;
		vector<shared_ptr<StateMachineBehaviorExecution> > stmExec = instance->getStateMachineExecutions();
		for (unsigned int i = 0; i < stmExec.size(); i++)
		{
			shared_ptr<InstanceSpecification> sig = _action->getSignal();
			if (sig)
			{
				//shared_ptr<Signal> signal( new Signal(sig->getName()) );
				(shared_dynamic_cast<StateMachineBehaviorExecution>(stmExec[i]))->addSignal(sig);
			}
			else
			{
                                shared_ptr<Signal> signalC = _action->getSignalClass();
                                if (signalC)
                                {
                                   shared_ptr<InstanceSpecification> sig (new InstanceSpecification(signalC->getName(),signalC));
				   (shared_dynamic_cast<StateMachineBehaviorExecution>(stmExec[i]))->addSignal(sig);
                                }
                                else 
				   cerr << " PAS DE SIGNAL " << endl;
			}
		}
	}
	else
	{
		if(getHost())
			cerr << getHost()->getName() << " Broadcasting signal: ";
		else
			cerr << "() Broadcasting signal: ";
		if(_action->getSignal())
		{
			cerr << _action->getSignal()->getName() << endl;
		}
		else if(_action->getSignalClass())
		{
			cerr << "(Class) " << _action->getSignalClass()->getName() << endl;
		}
		else
		{
			cerr << "Gnii... ça va planter !" << endl;
		}

		map<string,shared_ptr<InstanceSpecification> > instances=MascaretApplication::getInstance()->getEnvironment()->getInstances();
		for (map<string,shared_ptr<InstanceSpecification> >::iterator itE = instances.begin(); itE!=instances.end(); itE++)
		{
			shared_ptr<InstanceSpecification> instance = itE->second;
			vector<shared_ptr<StateMachineBehaviorExecution> > stmExec = instance->getStateMachineExecutions();
			for (unsigned int i = 0; i < stmExec.size(); i++)
			{
				shared_ptr<InstanceSpecification> sig = _action->getSignal();
				if (sig)
				{
					(shared_dynamic_cast<StateMachineBehaviorExecution>(stmExec[i]))->addSignal(sig);
				}
				else
				{
					shared_ptr<Signal> signalC = _action->getSignalClass();
					if (signalC)
					{
						shared_ptr<InstanceSpecification> sig (new InstanceSpecification(signalC->getName(),signalC));
						(shared_dynamic_cast<StateMachineBehaviorExecution>(stmExec[i]))->addSignal(sig);
					}
					else 
						cerr << " PAS DE SIGNAL " << endl;
				}
			}
		}
	}

	return 0;
}

} // Namespace
