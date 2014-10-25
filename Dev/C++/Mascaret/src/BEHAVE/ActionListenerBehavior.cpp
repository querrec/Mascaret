#include <boost/bind.hpp>
#include <iostream>
#include <vector>

#include "VEHA/Kernel/Environment.h"
#include "VEHA/Kernel/InstanceSpecification.h"
#include "VEHA/Kernel/Classifier.h"
#include "VEHA/Kernel/Operation.h"
#include "VEHA/Kernel/Parameter.h"
#include "VEHA/Kernel/Slot.h"
#include "VEHA/Kernel/Property.h"
#include "VEHA/Kernel/LiteralReal.h"
#include "VEHA/Kernel/LiteralInteger.h"
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
#include "BEHAVE/ActionListenerBehavior.h"
#include "BEHAVE/ProcedureExecution.h"
#include "BEHAVE/HttpCommunicationManager.h"
#include "HAVE/VirtualHuman.h"
#include "MascaretApplication.h"

#include "VEHA/Behavior/BasicActions/Action.h"
#include "VEHA/Behavior/BasicActions/SendSignalAction.h"
#include "VEHA/Behavior/Common/Signal.h"
#include "VEHA/Behavior/BasicActions/CallOperationAction.h"
#include "VEHA/Behavior/Activity/ActivityEdge.h"
#include "VEHA/Behavior/Activity/ObjectNode.h"

#include "IEHA/Interaction/Interaction.h"
#include "IEHA/VRApplication.h"

using namespace VEHA;
namespace BEHAVE
{
string clean(const string& toClean);
ActionListenerBehavior::ActionListenerBehavior(shared_ptr<Behavior> specif,shared_ptr<InstanceSpecification> host,const Parameters& p)
: CyclicBehaviorExecution(specif, host, p)
{
}

void ActionListenerBehavior::onActionDone(shared_ptr<Action> action)
{

	if(shared_dynamic_cast<SendSignalAction>(action))
	{
		shared_ptr<SendSignalAction> ssa=shared_dynamic_cast<SendSignalAction>(action);
		cerr << ssa->getTarget()->getName()<<" received signal "<<ssa->getSignal()->getName()<<endl;
		vector<string> toRemove;
		map<string,shared_ptr<ActivityExecution> >::iterator it=_actionsPossibles.begin();
		cerr << _actionsPossibles.size() << endl;
		while(it != _actionsPossibles.end())
		{
			bool found=false;
			/*cerr << it->first << " state:"<<endl;
			it->second->printState();
			cerr << it->first << " end state"<<endl<<endl;*/
			vector<shared_ptr<ActionNode> > actions=it->second->getNextActions("this");
			for(size_t i=0;i<actions.size();i++)
			{
				cerr << "TEST : " << actions[i]->getName()  << " == " << ssa->getSignal()->getName() << endl;
				//if(actions[i]->getName()==string("^"+ssa->getSignal()->getName()))
				if(actions[i]->getName()==ssa->getSignal()->getName())
				{
					if(_actionsPossiblesParameter.find(it->first) != _actionsPossiblesParameter.end() &&
							_actionsPossiblesParameter[it->first] == ssa->getTarget())
					{
						//cerr << "the action can be "<< it->first<<endl;
						it->second->informActionRunning("this",ssa->getSignal()->getName());
						it->second->informActionDone("this",ssa->getSignal()->getName());
						it->second->updateNodes();
						found=true;
					}
				}
			}
			if(!found)
			{
				//cerr << "the action can't be "<< it->first<<endl;
				toRemove.push_back(it->first);
			}
			it++;
		}
		if(toRemove.size() < _actionsPossibles.size())
			for(size_t i=0;i<toRemove.size();i++)
				_actionsPossibles.erase(toRemove[i]);
		restart();
	}
}

void ActionListenerBehavior::pushProcedureToDo(shared_ptr<Procedure> proc, shared_ptr<OrganisationalEntity> org, shared_ptr<Role> role, Parameters params_)
{
	shared_ptr<Agent> agt = (shared_dynamic_cast<Agent>(getHost()));

	cerr << "ActionListenerBehavior starting procedure "<< proc->getName() <<endl;

	shared_ptr<ActionListenerBehavior> thisRef=shared_dynamic_cast<ActionListenerBehavior>(shared_from_this());
	//IEHA::VRApplication::getInstance()->getInteraction()->addCallbackActionDone(thisRef,&ActionListenerBehavior::onActionDone);
	//FIXME pas top
	IEHA::VRApplication::getInstance()->getWindows()[0]->addCallbackActionDone(thisRef,&ActionListenerBehavior::onActionDone);

	shared_ptr<AgentPlateform> agtP = agt->getAgentPlateform();
	shared_ptr<ExternalCommunicationManager> commManager = agtP->getCommunicationManager();
	shared_ptr<HttpCommunicationManager> httpManager = shared_dynamic_cast<HttpCommunicationManager>(commManager);
	// RQ       httpManager->sigServlet->addCallbackActionDone(thisRef,&ActionListenerBehavior::onActionDone);

	//Interface3D::get()->addCallbackOnSignalSent(thisRef,&ActionListenerBehavior::onSignalSent);
	shared_ptr<ProcedureExecution> procInfo = make_shared<ProcedureExecution>(proc, org, agt);
	//procInfo.currentNodeState = 2; //DONE

	procInfo->buildAffectations(params_, agt);
	_buildActionsPossibles(procInfo);

	/* Search if the current agent have something to do in this procedure */
	vector<shared_ptr<ActivityPartition> > parts = proc->getActivity()->getPartition();
	bool found = false;
	for (size_t iP = 0; iP < parts.size(); iP++)
	{
		if (parts[iP]->getName() == role->getName())
		{
			cerr << "[ActionListenerBehavior Info] Found a role " << role->getName() << " in procedure " << proc->getName() << endl;
			found = true;
		}
	}

	if (found)
	{
		_runningProcedures.push_back(procInfo);
	}
}

void ActionListenerBehavior::action ()
{
	shared_ptr<Agent> agt = (shared_dynamic_cast<Agent>(getHost()));

	if (_runningProcedures.size())
	{
		for (size_t iP = 0; iP < _runningProcedures.size(); )
		{
			shared_ptr<ProcedureExecution> procInfo = _runningProcedures[iP];
			vector<shared_ptr<ActionNode> > actionNodes=procInfo->getActionToExecute();

			if(_actionsPossibles.size() == 1)
			{
				if(_actionsPossibles.begin()->second != _lastActionPossible)
				{
					cerr << " ############################################### " << endl;
					cerr << " ##     " << agt->getName() << endl;
					cerr << " ############################################### " << endl;
					cerr << "the action is "<< _actionsPossibles.begin()->first <<endl;
					_sendActionRealisationMessage(_actionsPossibles.begin()->first,procInfo);
					_lastActionPossible=_actionsPossibles.begin()->second;
				}
				if(_actionsPossibles.begin()->second->isFinished())
				{
					_sendActionDoneMessage(_actionsPossibles.begin()->first,procInfo);
					vector< pair<string , shared_ptr<AID> > >  agts = MascaretApplication::getInstance()->getAgentPlateform()->getAgentToInform();
					shared_ptr<AID> agentAID=MascaretApplication::getInstance()->agent->getAID();
					for (size_t i = 0; i < agts.size() ; i++)
					{
						shared_ptr<ACLMessage> procMsg = shared_ptr<ACLMessage>(new ACLMessage(INFORM));
						procMsg->setSender(agentAID);
						string content ="((action ";
						content += agentAID->getName();
						content += " ";
						content += "(" + _actionsPossibles.begin()->first +")";
						content += "))";
						procMsg->setContent(content);
						procMsg->setConversationID(agts[i].first);
						procMsg->addReceiver(agts[i].second);
						MascaretApplication::getInstance()->getAgentPlateform()->sendMessage(procMsg);
					}

					cerr << _actionsPossibles.begin()->first << " fini"<<endl;
					_lastActionPossible=shared_ptr<ActivityExecution>();
					_buildActionsPossibles(procInfo);
				}
			}

			if(procInfo->isFinished())
			{
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
	pause();
	/*else
	{
		pause();
	}*/
}

void ActionListenerBehavior::_buildActionsPossibles(shared_ptr<ProcedureExecution> procInfo)
{
	_actionsPossibles.clear(); // FIXME: On supprime les actions possibles ? Comment ça ce passe quand plusieurs procédure sont en cours ?!?
	vector< shared_ptr<ActivityNode> > nodes = procInfo->getProcedure()->getActivity()->getNode();
	map<string, shared_ptr<InstanceSpecification> > affectations = procInfo->getAffectations();

	cerr << " BuildActions Possible : " << nodes.size() << endl;
	for(size_t i=0;i<nodes.size();i++)
	{
		shared_ptr<ActivityNode> node = nodes[i];
		cerr << "    -----> " << node << endl;
		if(nodes[i]->getKind() == "action")
		{
			shared_ptr<ActionNode> actionNode=shared_dynamic_cast<ActionNode>(node);
			shared_ptr<CallOperationAction> callOp=shared_dynamic_cast<CallOperationAction>(actionNode->getAction());
			if(callOp)
			{
				cerr << " CallOperation : " << callOp->getName() << endl;
				string opName=callOp->getOperation()->getName();
				shared_ptr<Activity> act=shared_dynamic_cast<Activity>(callOp->getOperation()->getMethod());
				if(act)
				{
					cerr << " CallOp + Method : " << callOp->getName() << endl;
					_actionsPossibles[opName]=shared_ptr<ActivityExecution>(new ActivityExecution(act));
					_actionsPossiblesParameter[opName]= affectations[actionNode->getOutgoingObjectNode()[0]->getName()];
				}
			}
		}
	}
}

void ActionListenerBehavior::_sendActionRealisationMessage(const string& action, shared_ptr<ProcedureExecution> procInfo)
{
	shared_ptr<Agent> agt = (shared_dynamic_cast<Agent>(getHost()));
	shared_ptr<ACLMessage> procMsg = make_shared<ACLMessage>(INFORM);

	string content ="((action ";
	content += agt->getName();
	content += " ";
	content += "(" + clean(action) +")";
	content += "))";
	procMsg->setContent(content);

	vector<shared_ptr<AID> > agents = procInfo->getOtherAgents();

	for (size_t iA = 0; iA < agents.size(); iA++)
	{
		procMsg->addReceiver(agents[iA]);
	}
	agt->send(procMsg);
}

void ActionListenerBehavior::_sendActionDoneMessage(const string& action, shared_ptr<ProcedureExecution> procInfo)
{
	shared_ptr<Agent> agt = (shared_dynamic_cast<Agent>(getHost()));

	shared_ptr<ACLMessage> procMsg = make_shared<ACLMessage>(INFORM);

	//set ACLMessage content
	string content ="((done (action ";
	content += agt->getName();
	content += " ";
	content += "(" + clean(action) +")";
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
}
