#include "VEHA/Kernel/Environment.h"
#include "VEHA/Kernel/InstanceSpecification.h"
#include "VEHA/Kernel/Classifier.h"
#include "VEHA/Kernel/Operation.h"
#include "VEHA/Kernel/Parameter.h"
#include "VEHA/Kernel/Slot.h"
#include "VEHA/Kernel/Property.h"
#include "VEHA/Kernel/LiteralReal.h"
#include "VEHA/Kernel/LiteralInteger.h"
#include "VEHA/Behavior/Common/behaviorScheduler.h"
#include "VEHA/Behavior/Common/BehaviorExecution.h"
#include "VEHA/Behavior/Common/Behavior.h"
#include "VEHA/Entity/Vector3.h"
#include "VEHA/Entity/Entity.h"
#include "BEHAVE/OneShotBehaviorExecution.h"
#include "BEHAVE/CyclicBehaviorExecution.h"
#include "BEHAVE/OrganisationalEntity.h"
#include "BEHAVE/Agent.h"
#include "BEHAVE/AgentPlateform.h"
#include "BEHAVE/RoleAssignement.h"
#include "BEHAVE/RessourceAssignement.h"
#include "BEHAVE/AID.h"
#include "BEHAVE/ACLMessage.h"
#include "BEHAVE/fipaSLParser.h"
#include "BEHAVE/ProceduralBehavior.h"
#include "BEHAVE/ProcedureExecution.h"
#include "HAVE/VirtualHuman.h"
#include <boost/bind.hpp>
#include <iostream>
#include <vector>

#include "VEHA/Behavior/BasicActions/Action.h"
#include "VEHA/Behavior/BasicActions/SendSignalAction.h"
#include "VEHA/Behavior/BasicActions/CallOperationAction.h"
#include "VEHA/Behavior/BasicActions/DummyAction.h"
#include "VEHA/Behavior/Activity/ActivityEdge.h"
#include "VEHA/Behavior/Time/TimeExpression.h"

using namespace VEHA;
namespace BEHAVE
{
string clean(const string& toClean);
ProceduralBehavior::ProceduralBehavior(shared_ptr<Behavior> specif,shared_ptr<InstanceSpecification> host,const Parameters& p)
: CyclicBehaviorExecution(specif, host, p)
{
}

ProceduralBehavior::~ProceduralBehavior()
{
	// shared_ptr<Agent> agt = (shared_dynamic_cast<Agent>(getHost()));
	// cerr << "ProceduralBehavior::~ProceduralBehavior() for " << agt->getAID()->toString() << endl;
}

void ProceduralBehavior::pushProcedureToDo(shared_ptr<Procedure> proc, shared_ptr<OrganisationalEntity> org_, shared_ptr<Role> role, Parameters params_)
{
	shared_ptr<Agent> agt = (shared_dynamic_cast<Agent>(getHost()));

	shared_ptr<ProcedureExecution> procInfo = make_shared<ProcedureExecution>(proc, org_, agt);
	procInfo->buildAffectations(params_, getHost());

	/* Search if the current agent have something to do in this procedure */
	vector<shared_ptr<ActivityPartition> > parts = proc->getActivity()->getPartition();
	bool found = false;
	for (size_t iP = 0; iP < parts.size(); iP++)
	{
		if (parts[iP]->getName() == role->getName())
		{
			cerr << "[ProceduralBehavior Info] Found a role " << role->getName() << " in procedure " << proc->getName() << endl;
			found = true;
		}
	}

	_runningProcedures.push_back(procInfo);

	if (!found)
	{
		//inform all agents that this agent's role in this procedure has ended (used to unblock tokens in ProcedureExecution)
		_sendProcedureDoneMessage(procInfo);
		_runningProcedures.pop_back(); //remove it..
	}
}

void ProceduralBehavior::action ()
{
	shared_ptr<Agent> agt = (shared_dynamic_cast<Agent>(getHost()));

		if (_runningProcedures.size())
		{
			for (size_t iP = 0; iP < _runningProcedures.size(); )
			{
				shared_ptr<ProcedureExecution> procInfo = _runningProcedures[iP];
				vector<shared_ptr<ActionNode> > actionNodes;
				
				//remove those which are not agreed upon
				if (agt->getControlledByHuman())
				{
					cerr << "[ProceduralBehavior Info] Agent controlled ... " << endl;
					actionNodes = procInfo->getActionToExecute();
					for (size_t atd=0; atd<actionNodes.size(); ++atd)
						cerr << "ProceduralBehavior Info] -- Human has to do : " << actionNodes[atd]->getName() << endl;
					actionNodes.clear();

					for (size_t atd=0; atd<_actionsToDo.size(); ++atd)
					{
						actionNodes.push_back(_actionsToDo[atd]);
					}
					_actionsToDo.clear();
				}
				else
				{
					//cerr << "ProceduralBehavior getActionToExecute() ... " << endl;
					actionNodes = procInfo->getActionToExecute();
				}
				
				if (actionNodes.size())
				{
					cerr << " ############################################### " << endl;
					cerr << " ##     " << agt->getName() << endl;
					cerr << " ############################################### " << endl;

					for(size_t i=0;i<actionNodes.size();i++)
					{
						actionNodes[i]->start(agt,procInfo->getAffectations()); //TODO ///////////////////////////////////////// if not flag start, else start
						_sendActionRealisationMessage(actionNodes[i], procInfo); // TODO
						
						procInfo->informActionRunning(agt->getAID(),actionNodes[i]);
						
						if(actionNodes[i]->getCurrentExecution())
						{
							//cerr << "execution ok" << endl;
							_behaviorToNode[actionNodes[i]->getCurrentExecution().get()]=actionNodes[i];
                            
							actionNodes[i]->getCurrentExecution()->addCallbackOnBehaviorStop(bind(&ProceduralBehavior::_onBehaviorStop,this,_1));
						}
						else
						{
							//cerr << "execution ko --> ignore (= says realization ok)" << endl;
							procInfo->informActionDone(agt->getAID(),actionNodes[i]); //TODO
							_sendActionDoneMessage(actionNodes[i], procInfo);
						}
					}
				}
				else if (agt->getMessageQueue().size() == 0)
				{
					pause();
				}
				
				if (procInfo->isFinished())
				{
					//inform all agents that this agent's role in this procedure has ended (used to unblock tokens in ProcedureExecution)
					_sendProcedureDoneMessage(procInfo);
					
					_runningProcedures[iP]=_runningProcedures.back();
					_runningProcedures.pop_back();
					cerr << getHost()->getName()<<" : suppression de la procedure"<< endl;
				}
				else
				{
					iP ++;
				}
			}
		}
		else
		{
			pause();
		}
}

void ProceduralBehavior::stop(string procedureName)
{
	vector<shared_ptr<ProcedureExecution> > exec; // Procedure still running
	for (size_t iP = 0; iP < _runningProcedures.size(); iP++)
	{
		shared_ptr<ProcedureExecution> procInfo = _runningProcedures[iP];
		if (procInfo->getProcedure()->getName() == procedureName)
		{
			procInfo->stop();
		}
		else 
		{
			exec.push_back(procInfo);
		}
	}

	// We put back still running procedure in the vector
	_runningProcedures.clear();
	for (size_t iP = 0; iP < exec.size() ; iP++)
		_runningProcedures.push_back(exec[iP]);
}

void ProceduralBehavior::stop(shared_ptr<Procedure> procedure)
{
	vector<shared_ptr<ProcedureExecution> > exec; // Procedure still running
	for (size_t iP = 0; iP < _runningProcedures.size(); iP++)
	{
		shared_ptr<ProcedureExecution> procInfo = _runningProcedures[iP];
		if (procInfo->getProcedure() == procedure)
		{
			procInfo->stop();
		}
		else
		{
			exec.push_back(procInfo);
		}
	}

	// We put back still running procedure in the vector
	_runningProcedures.clear();
	for (size_t iP = 0; iP < exec.size() ; iP++)
		_runningProcedures.push_back(exec[iP]);
}

void ProceduralBehavior::stopAll()
{
	vector<shared_ptr<ProcedureExecution> > exec;
	for (size_t iP = 0; iP < _runningProcedures.size(); iP++)
	{
		shared_ptr<ProcedureExecution> procInfo = _runningProcedures[iP];
		procInfo->stop();
	}
	_runningProcedures.clear();
}

void ProceduralBehavior::_onBehaviorStop(BehaviorExecution* be)
{
	//cerr << _behaviorToNode[be]->getName() <<" ended"<< endl;
	for (size_t iP = 0; iP < _runningProcedures.size(); iP++)
	{

		shared_ptr<ProcedureExecution> procInfo = _runningProcedures[iP];
		procInfo->informActionDone(shared_dynamic_cast<Agent>(getHost())->getAID(),_behaviorToNode[be]);
		_sendActionDoneMessage(_behaviorToNode[be], procInfo);

	}
	restart();
}

void ProceduralBehavior::onActionDone(shared_ptr<AID> agent, const shared_ptr<ActionNode> action)
{
	for (size_t iP = 0; iP < _runningProcedures.size();iP++ )
	{
		shared_ptr<ProcedureExecution> procInfo = _runningProcedures[iP];

		procInfo->informActionDone(agent,action);
	}
}

void ProceduralBehavior::onActionDone(shared_ptr<AID> agent, const string& actionName)
{
	for (size_t iP = 0; iP < _runningProcedures.size();iP++ )
	{
		shared_ptr<ProcedureExecution> procInfo = _runningProcedures[iP];
		
		procInfo->informActionDone(agent,actionName);
	}
}

void ProceduralBehavior::onActionRunning(shared_ptr<AID> agent, const shared_ptr<ActionNode> action)
{
	for (size_t iP = 0; iP < _runningProcedures.size();iP++ )
	{
		shared_ptr<ProcedureExecution> procInfo = _runningProcedures[iP];
		
		procInfo->informActionRunning(agent,action);
	}
}

void ProceduralBehavior::onActionRunning(shared_ptr<AID> agent, const string& actionName)
{
	for (size_t iP = 0; iP < _runningProcedures.size();iP++ )
	{
		shared_ptr<ProcedureExecution> procInfo = _runningProcedures[iP];
		
		procInfo->informActionRunning(agent,actionName);
	}
}

void ProceduralBehavior::_sendActionRealisationMessage(shared_ptr<ActionNode> action, shared_ptr<ProcedureExecution> procInfo)
{
	shared_ptr<Agent> agt = (shared_dynamic_cast<Agent>(getHost()));

	shared_ptr<ACLMessage> procMsg = make_shared<ACLMessage>(INFORM);
	
	//we inform at wich time the action start
	TimeExpression timestamp = action->getCurrentExecution()->getStart();
	
	procMsg->setTimestamp(timestamp);
	
	//set ACLMessage content
	string content ="((action ";
	content += agt->getName();
	content += " ";
	content += "(" + clean(action->getName()) +")";
	content += "))";
	procMsg->setContent(content);

	//send message to other agents
	vector<shared_ptr<AID> > agents = procInfo->getOtherAgents();

	for (size_t iA = 0; iA < agents.size(); iA++)
	{
		procMsg->addReceiver(agents[iA]);
	}
	agt->send(procMsg);
}
void ProceduralBehavior::_sendActionDoneMessage(shared_ptr<ActionNode> action, shared_ptr<ProcedureExecution> procInfo)
{
	shared_ptr<Agent> agt = (shared_dynamic_cast<Agent>(getHost()));

	shared_ptr<ACLMessage> procMsg = make_shared<ACLMessage>(INFORM);
	
	//we inform at wich time the action finished
	TimeExpression timestamp = action->getCurrentExecution()->getFinish();
	
	procMsg->setTimestamp(timestamp);

	//set ACLMessage content
	string content ="((done (action ";
	content += agt->getName();
	content += " ";
	content += "(" + clean(action->getName()) +")";
	content += ")))";
	procMsg->setContent(content);

	//send message to other agents
	vector<shared_ptr<AID> > agents = procInfo->getOtherAgents();

	for (size_t iA = 0; iA < agents.size(); iA++)
	{
		procMsg->addReceiver(agents[iA]);
	}
	agt->send(procMsg);
}

void ProceduralBehavior::_sendProcedureDoneMessage(shared_ptr<ProcedureExecution> procInfo)
{
	shared_ptr<Agent> agt = (shared_dynamic_cast<Agent>(getHost()));
	
	shared_ptr<ACLMessage> procMsg = make_shared<ACLMessage>(INFORM);
	
	//we inform at wich time the procedure finish
	TimeExpression timestamp = BehaviorScheduler::getInstance()->getCurrentVirtualTime();
	procMsg->setTimestamp(timestamp);

	string content ="((done (action ";
	content += agt->getName();
	content += " ";
	content += "(" + clean(procInfo->getProcedure()->getName()) +")";
	content += ")))";
	procMsg->setContent(content);
	
	/*
	//broadcast to all agents which participate in a procedure // TODO: review this
	vector<shared_ptr<AID> > agents;
	for (size_t iP = 0; iP < _runningProcedures.size(); ++iP)
	{
		shared_ptr<ProcedureExecution> pinf = _runningProcedures[iP];
		
		vector<shared_ptr<AID> > pAgts = pinf->getOtherAgents();
		for (size_t iA = 0; iA < pAgts.size(); ++iA)
		{
			bool found = false;
			//check if already added agent (we don't like duplicates)
			for (size_t jA = 0; jA < agents.size(); ++jA)
			{
				if (pAgts[iA] == agents[jA])
				{
					found = true;
					break;
				}
			}
			
			if (!found)
			{
				agents.push_back(pAgts[iA]);
			}
		}
	}

	for (size_t iA = 0; iA < agents.size(); iA++)
	{
		procMsg->addReceiver(agents[iA]);
	}
	*/
	
	procMsg->addReceiver(MascaretApplication::getInstance()->agent->getAID());
	
	agt->send(procMsg);
}
}
