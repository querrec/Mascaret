#include "BEHAVE/Agent.h"
#include "BEHAVE/AgentClass.h"
#include "BEHAVE/AgentBehavior.h"
#include "BEHAVE/AgentBehaviorExecution.h"
#include "BEHAVE/SimpleBehaviorExecution.h"
#include "BEHAVE/SimpleCommunicationBehavior.h"
#include "BEHAVE/ProceduralBehavior.h"
#include "BEHAVE/ProcedureExecution.h"
#include "BEHAVE/OneShotBehaviorExecution.h"
#include "BEHAVE/AgentPlateform.h"
#include "BEHAVE/ACLMessage.h"
#include "VEHA/Behavior/Common/behaviorScheduler.h"
#include "VEHA/Behavior/Common/Signal.h"
#include "VEHA/Behavior/BasicActions/Action.h"
#include "VEHA/Behavior/Activity/Activity.h"
#include "VEHA/Behavior/Activity/ActivityNode.h"
#include "VEHA/Behavior/Activity/ActionNode.h"
#include "VEHA/Behavior/BasicActions/SendSignalAction.h"
#include "VEHA/Kernel/Operation.h"

namespace BEHAVE
{
using VEHA::BehaviorScheduler;
Agent::Agent(shared_ptr<AgentPlateform> ap, const string & name, shared_ptr<AgentClass> agentclass, string KBName) : InstanceSpecification(name, agentclass), _aid(shared_ptr<AID>(new AID(name,ap->getName(), ap->getPort()))), _plateform(ap),_mailbox(shared_ptr<Mailbox>(new Mailbox())), _knowledgeBase(shared_ptr<KnowledgeBase>(new KnowledgeBase(KBName)))
{
}

Agent::~Agent()
{
}

void Agent::addBehavior(const string& behaviorName,double interval, bool start)
{
	if(getBehaviorByName(behaviorName))
		return;

	shared_ptr<AgentBehavior> behavior(new AgentBehavior(behaviorName));
	Parameters param;
	// TODO: le comportement est créer 2 fois. Il doit y avoir mieux a faire...
	shared_ptr<BehaviorExecution> be=behavior->createBehaviorExecution(shared_dynamic_cast<InstanceSpecification>(shared_from_this()),param);
	if(!shared_dynamic_cast<OneShotBehaviorExecution>(be) && start)
	{
		be = BehaviorScheduler::getInstance()->executeBehavior(shared_dynamic_cast<Behavior>(behavior),shared_dynamic_cast<InstanceSpecification>(shared_from_this()),param);
		if(interval > 0)
		{
			shared_ptr<SimpleBehaviorExecution> sbe=shared_dynamic_cast<SimpleBehaviorExecution>(be);
			if(sbe)
				sbe->setInterval(interval);
		}
		_behaviorsExecution.push_back(shared_dynamic_cast<AgentBehaviorExecution>(be));
	}
	_behaviors.push_back(behavior);
}

shared_ptr<AgentBehavior> Agent::getBehaviorByName(const string& behaviorName)
{
	shared_ptr<AgentBehavior> behavior;

	for (size_t i = 0; i < _behaviors.size(); i++)
	{
		if (_behaviors[i]->getName() == behaviorName)
			behavior = _behaviors[i];
	}

	return behavior;
}
shared_ptr<AgentBehaviorExecution> Agent::executeOneShotBehavior(const string& behaviorName)
{
	shared_ptr<AgentBehavior> behavior=getBehaviorByName(behaviorName);
	Parameters param;
	// TODO: le comportement est créer 2 fois. Il doit y avoir mieux a faire...
	shared_ptr<BehaviorExecution> be=behavior->createBehaviorExecution(shared_dynamic_cast<InstanceSpecification>(shared_from_this()),param);
	if(shared_dynamic_cast<OneShotBehaviorExecution>(be))
	{
		be = BehaviorScheduler::getInstance()->executeBehavior(shared_dynamic_cast<Behavior>(behavior),shared_dynamic_cast<InstanceSpecification>(shared_from_this()),param);
		return shared_dynamic_cast<AgentBehaviorExecution>(be);
	}
	return shared_ptr<AgentBehaviorExecution>();
}
shared_ptr<AgentBehaviorExecution> Agent::getBehaviorExecutionByName(const string& behaviorName)
{
	shared_ptr<AgentBehaviorExecution> behavior;

	for (size_t i = 0; i < _behaviorsExecution.size(); i++)
	{
		if (_behaviorsExecution[i]->getSpecification()->getName() == behaviorName)
			behavior = _behaviorsExecution[i];
	}

	return behavior;
}


void Agent::postMessage(shared_ptr<ACLMessage> message)
{
	_mailbox->postMessage(message);
}

shared_ptr<ACLMessage> Agent::receive()
{
	return _mailbox->check();
}

void Agent::send(shared_ptr<ACLMessage> message)
{
	message->setSender(getAID());
	getAgentPlateform()->sendMessage(message);
	_mailbox->send(message);
}

void Agent::addExecution(shared_ptr<BehaviorExecution> be)
{
	InstanceSpecification::addExecution(be);

	shared_ptr<Behavior> behaviorDone = be->getSpecification();

	shared_ptr<Class> classe = getClassifier();
	if (!classe) return;

	map<string, shared_ptr<Operation> > operations = classe->getOperations();

	bool found = false;
	shared_ptr<Operation> operationDone;
	map<string,  shared_ptr<Operation> >::iterator it = operations.begin();
	while( it != operations.end() && !found)
	{
		shared_ptr<Behavior> operationBehavior = it->second->getMethod();
		if (operationBehavior)
		{
			shared_ptr<Activity> act = shared_dynamic_cast<Activity>(operationBehavior);
			if (act)
			{
				shared_ptr<ActivityNode> init = act->getInitialNode();

				vector< shared_ptr<ActionNode> > ans = init->getOutgoingActionNode();
				if (ans.size() > 0)
				{
					shared_ptr<ActionNode> actionNode = ans[0];
					shared_ptr<VEHA::Action> action = actionNode->getAction();
					shared_ptr<SendSignalAction> sendSigAction = shared_dynamic_cast<SendSignalAction>(action);	
					if (sendSigAction)
					{
						shared_ptr<SendSignalAction> sendSigDone = shared_dynamic_cast<SendSignalAction>(behaviorDone);
						if (sendSigDone)
						{
							if (sendSigDone->getSignalClass()->getName() == sendSigAction->getSignalClass()->getName())
							{
								string targetClassNameDone = sendSigDone->getTarget()->getClassifier()->getName();
								vector< shared_ptr<ObjectNode> > objs = actionNode->getOutgoingObjectNode();
								if (objs.size() > 0)
								{
									if (objs[0]->getResourceType()->getName() == targetClassNameDone)
									{
										found = true;
										operationDone = it->second;
									} 
								}
							}
						}
					}
				}
			}
		}
		it ++;
	}

	if (found)
	{
		shared_ptr<AgentBehaviorExecution> abe = getBehaviorExecutionByName("SimpleCommunicationBehavior");
                shared_ptr<SimpleCommunicationBehavior> cbe  = shared_dynamic_cast<SimpleCommunicationBehavior> (abe);
		vector<shared_ptr<ActionNode> > possibleTranslation = cbe->_translateOperationToActions(operationDone);		

		shared_ptr<AgentBehaviorExecution> pbe = getBehaviorExecutionByName("ProceduralBehavior"); 
                shared_ptr<ProceduralBehavior> pb  = shared_dynamic_cast<ProceduralBehavior> (pbe);
		if (possibleTranslation.size() >0 && pb)
		{
			shared_ptr<ActionNode> an = possibleTranslation[0];
		        cerr << " # Action : " << an->getName() << endl;
			an->_currentExecution = be;
			vector<shared_ptr<ProcedureExecution> > procs = pb->accessRunningProcedures();
			shared_ptr<ProcedureExecution> procInfo = procs[0];
			pb->_sendActionRealisationMessage(an, procInfo); // TODO
                        procInfo->informActionRunning(getAID(),an->getName());

			procInfo->informActionDone(getAID(),an->getName()); 
                        pb->_sendActionDoneMessage(an, procInfo);
		}

	}
	
}

void Agent::wakeup()
{
	for (size_t i = 0; i < _behaviorsExecution.size(); i++)
	{
		if (_behaviorsExecution[i] && _behaviorsExecution[i]->isSuspended())
		{
			_behaviorsExecution[i]->restart();
		}
	}
}

}
