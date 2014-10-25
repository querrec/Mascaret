#include "BEHAVE/Agent.h"
#include "BEHAVE/ProcedureExecution.h"
#include "BEHAVE/Procedure.h"
#include "BEHAVE/OrganisationalEntity.h"
#include "BEHAVE/RessourceAssignement.h"
#include "BEHAVE/RoleAssignement.h"
#include "BEHAVE/Role.h"

#include "VEHA/Kernel/Operation.h"
#include "VEHA/Kernel/Parameter.h"

#include "VEHA/Behavior/Activity/ActivityNode.h"
#include "VEHA/Behavior/Activity/ActionNode.h"
#include "VEHA/Behavior/Activity/ObjectNode.h"
#include "VEHA/Behavior/Activity/ActivityEdge.h"
#include "VEHA/Behavior/Activity/ActivityPartition.h"
#include "VEHA/Behavior/Activity/Activity.h"
#include "VEHA/Behavior/Activity/ActivityParameterNode.h"

#include "VEHA/Behavior/BasicActions/Action.h"
#include "VEHA/Behavior/BasicActions/CallOperationAction.h"
#include "VEHA/Behavior/BasicActions/SendSignalAction.h"

#define ACTIVATE_DEBUG_CONSOLE_OUTPUT_TOKENS false
#define ACTIVATE_GENERAL_DEBUG false

using namespace VEHA;

namespace BEHAVE
{

// ------------------------------------------------------------------------------------------- //
// ----- additional helper functions --------------------------------------------------------- //

string clean(const string& toClean)
{
	string returned="";
	for(size_t i=0;i<toClean.size();i++)
	{
		if(toClean[i] != '/' && toClean[i] != '(' && toClean[i] != ')' && toClean[i] != '^')
		{
			returned += toClean[i];
		}
	}
	
	return returned;
}

template <typename T> inline
void StlVectorFastErase(vector<T> & vect, unsigned int index)
{
	vect[index]=vect.back();
	vect.pop_back();
}

// ------------------------------------------------------------------------------------------- //

ProcedureExecution::ProcedureExecution(const shared_ptr<Procedure> procedure, shared_ptr<OrganisationalEntity> organisation, shared_ptr<Agent> agent) :
_procedure(procedure), _organisation(organisation), _agent(agent)
{
	for(size_t i=0;i<procedure->getActivity()->getPartition().size();i++)
	{
		shared_ptr<ActivityPartition> partition=procedure->getActivity()->getPartition()[i];
		vector<shared_ptr<RoleAssignement> > assignements=organisation->getRoleAssignements();
		for(size_t j=0;j<assignements.size();j++)
		{
			cerr << " Partition : " << partition->getName() << endl;
			if(partition->getName() == assignements[j]->getRole()->getName())
			{
				cerr << "    >> " << assignements[j]->getAgent()->toString() << " -> " << partition->getName() << endl;
				_agentToPartition[assignements[j]->getAgent()->toString()]=partition;
				_partitionToAgent[partition]=assignements[j]->getAgent();
				cerr << assignements[j]->getAgent()->getName()<<" --> "<<partition->getName()<<endl;
			}
		}
	}
	
	shared_ptr<ActivityExecutionToken> initialToken = make_shared<ActivityExecutionToken>();
	initialToken->currentLocation = procedure->getActivity()->getInitialNode();

	if(!initialToken->currentLocation)
	{
		cerr << "ERROR: no initial node found, not allowed" << endl;
		cerr << "Procedure will not start." << endl;
		return;
	}
	
	vector<shared_ptr<ActivityEdge> > controlFlows = procedure->getActivity()->getInitialNode()->getOutgoingControlFlowEdges();
	
	if (controlFlows.size() == 1)
	{
		initialToken->outgoingEdge = controlFlows[0];
		_activeTokens.push_back(initialToken);
		
		shared_ptr<ActionNode> dummyActionVariable;
		_tryToAdvanceToken(initialToken, dummyActionVariable); //first advancement
	}
	else
	{
		if (controlFlows.size() == 0)
		{
			cerr << "WARNING: initial node has no outgoing edges" << endl;
		}
		else
		{
			cerr << "ERROR: multiple outgoing edges for the initial node, not allowed" << endl;
		}
		cerr << "Procedure will not start." << endl;
	}
}

ProcedureExecution::~ProcedureExecution()
{
	cerr << "ProcedureExecution::~ProcedureExecution() for " << _agent->getAID()->toString() << endl;
}

void ProcedureExecution::_buildAffectationsObject(vector<string> & missing, shared_ptr<ActivityNode> node)
{
	string pname = node->getName();

	cerr << " Essaie affectation : " << pname << "(" << node->getFullName() << ")" << endl;

	shared_ptr<RessourceAssignement> ra=_organisation->findRessourceAssignement(pname);
	if(ra && ra->getEntity())
	{
		_affectations[pname]=ra->getEntity();
	}
	else
	{
		cerr << " Parameters size : " << _activityParams.size() << endl;
		Parameters::iterator it = _activityParams.find(pname);
		if (it != _activityParams.end())
		{
			_affectations[pname] = (shared_dynamic_cast<InstanceValue>(it->second))->getValue();
		}
		else
		{
			cerr << "Pas Affecte...." << endl;
			missing.push_back(pname);
		}
	}
}

void ProcedureExecution::_buildAffectationsAction(vector<string> & missing, shared_ptr<ActivityNode> node)
{
	shared_ptr<Action> action = (shared_dynamic_cast<ActionNode>(node))->getAction();

	if (action)
	{
		if (action->getKind() == "SendSignal")
		{
			cerr << "SendSignalAction found: " << node->getName()<<endl;
			shared_ptr<SendSignalAction> sendAct = (shared_dynamic_cast<SendSignalAction>(action));
			string pname = sendAct->getTargetName();
			cerr <<  " Signal target : " << pname << endl;

			if(pname != "")
			{
				shared_ptr<RessourceAssignement> ra=_organisation->findRessourceAssignement(pname);

				if(ra && ra->getEntity())
				{
					_affectations[pname]=ra->getEntity();
					sendAct->setTarget(ra->getEntity());
					cerr << "pname affectation: " << ra->getEntity()->getName() <<endl;
				}
				else
				{
					missing.push_back(pname);
					cerr << "[ProceduralBehavior.cpp] Affectation SendAction de " << pname << " impossible..." << endl;
				}
			}
		}
		else if (action->getKind() == "CallOperation")
		{
			shared_ptr<CallOperationAction> callAct = shared_dynamic_cast<CallOperationAction>(action);
			shared_ptr<InstanceSpecification> host = shared_dynamic_cast<InstanceSpecification>(_agent);
			map<string, shared_ptr<Operation> > opers = host->getClassifier()->getOperations();
			map<string, shared_ptr<Operation> >::iterator it = opers.find(callAct->getOperation()->getName());
			if (it != opers.end())
			{
				callAct->setOperation(it->second);
			}
		}
		else if (action->getKind() == "CallBehavior")
		{
			/*
			shared_ptr<CallBehaviorAction> callBeh = shared_dynamic_cast<CallBehaviorAction>(action);
			map<string, shared_ptr<Operation> > opers = getHost()->getClassifier()->getOperations();
			map<string, shared_ptr<Operation> >::iterator it = opers.find(callBeh->getOperation()->getName());
			if (it != opers.end())
			{
				callBeh->setBehavior(it->second);
			}
			*/
		}
		else
		{
			cerr << action->getKind() << endl;
		}
	}
}

void ProcedureExecution::_findRessources(vector<string> missings)
{
	/* We have unaffected resource, we while see if they exist in currently
	 * affected resources.
	 */
	bool affectAdded = false;

	vector<string> always_missings; // resources we always missing after a search pass

	for (size_t i = 0; i < missings.size(); i++)
	{
		bool missingAffected = false;
		map<string, shared_ptr<InstanceSpecification> >::iterator it = _affectations.begin();
		while (it != _affectations.end() && !missingAffected)
		{
			string missing = missings[i];
			shared_ptr<InstanceSpecification> entity = it->second;
			if (entity)
			{
				cerr << "Recherche : " << missing << " dans " << entity->getName() << " (affecté) " << endl;
				shared_ptr<Slot> slot = entity->getProperty(missing);
				if (slot)
				{
					cerr << " ... OK " << endl;
					shared_ptr<InstanceSpecification> value = *slot->getValue();
					_affectations[missing] = value;
					missingAffected = true;
					affectAdded = true;
				}
				else cerr << " ... pas trouve" << endl;
			}
			else
			{
				//cerr << "ERROR >>> " << it->first << " NOT FOUND !!" << endl;
			}
			it++;
		}
		if (!missingAffected) always_missings.push_back(missings[i]);
	}

	/* If we have always missing resources but we have add some on this pass
	 * we recursively search again.
	 */
	if (affectAdded) _findRessources(always_missings);
}

void ProcedureExecution::_buildAffectationsPostbone(vector<string> & missing)
{
	cerr << "[ProceduralBehavior Warning] Il y a des ressources non affectées ..." << endl;
	_findRessources(missing);
}

vector<shared_ptr<ActionNode> > ProcedureExecution::getAllActions()
{
	vector<shared_ptr<ActivityNode> > allNodes = _procedure->getActivity()->getNode();
	vector<shared_ptr<ActionNode> > actionNodes;

	for (size_t i=0; i<allNodes.size(); ++i)
	{
		shared_ptr<LoopNode> lNode = (shared_dynamic_cast<LoopNode>(allNodes[i]));
		if (lNode) //if cast succeeded
		{
			continue;
		}
		
		shared_ptr<ActionNode> aNode = (shared_dynamic_cast<ActionNode>(allNodes[i]));
		if (aNode) //if cast succeeded
		{
			actionNodes.push_back(aNode);
			continue;
		}
	}
	
	return actionNodes;
}

OclParser::Context ProcedureExecution::_buildOclContext(Parameters params) 
{ 
        /* Construction du contexte des constraintes */ 
        OclParser::Context context; 
 	 
        map<string, shared_ptr<InstanceSpecification> >::iterator itAffect; 
        for(itAffect=_affectations.begin();itAffect!=_affectations.end();itAffect++) 
        { 
                context[itAffect->first]=itAffect->second; 
        } 

        Parameters::iterator itParams = params.begin(); 
        for(itParams=params.begin();itParams!=params.end();itParams++) 
        { 
               context[itParams->first]=AnyValue(itParams->second->getStringFromValue()); 
        } 

       return context; 
} 

void ProcedureExecution::buildAffectations(Parameters params, shared_ptr<InstanceSpecification> host)
{
	vector<string> missing;

	cerr << "[ProcedureExecution Info] Affectations." << endl;

	/* Affectation de self */
	_affectations["self"] = host;

	/* Affectation des parametres */
	_activityParams.clear();
	vector< shared_ptr<ActivityNode> > nodes = _procedure->getActivity()->getNode();
	for(size_t iNode = 0; iNode < nodes.size(); iNode ++)
	{
		shared_ptr<ActivityNode> node = nodes[iNode];
		if (node->getKind() == "parameter")
		{
			shared_ptr<Parameter> p = (shared_dynamic_cast<ActivityParameterNode>(node))->getParameter();
			string strVal = params[p->getName()]->getStringFromValue();
			_activityParams[p->getName()] = p->getType()->createValueFromString(strVal);
		}
	}

	/* Affectation des ressources */
	vector<shared_ptr<ActivityPartition> > partitions = _procedure->getActivity()->getPartition();
	for (size_t i = 0; i < partitions.size(); i++)
	{
		if (partitions[i]->getName() == "this")
		{
			_affectations["this"] = host;
		}
		else
		{
			string pname = partitions[i]->getName();
		}

		vector< shared_ptr<ActivityNode> > nodes = partitions[i]->getNode();
		cerr << " SIZE : " << nodes.size() << endl;
		for (size_t iNode = 0; iNode<nodes.size(); iNode++)
		{
			shared_ptr<ActivityNode> node = nodes[iNode];
			if (node->getKind() == "object")
			{
				_buildAffectationsObject(missing, node);
			}
			else
			{
				if (node->getKind() == "action")
				{
					_buildAffectationsAction(missing, node);
				}
			}
		}
	}

	if (missing.size())
	{
		_buildAffectationsPostbone(missing);
	}

	cerr << "[ProcedureExecution Info] Fin affectations." << endl;

	_procedure->getActivity()->setContext(_buildOclContext(_activityParams));
}

void ProcedureExecution::stop()
{
	vector<shared_ptr<ActionNode> > actions = getAllActionsFor(_agent->getAID());
	for (unsigned int iA = 0; iA < actions.size(); iA++)
	{
		actions[iA]->stop();
	}
}

vector<shared_ptr<ActionNode> > ProcedureExecution::getAllActionsFor(shared_ptr<AID> agent)
{
	vector<shared_ptr<ActionNode> > allActionNodes = getAllActions();
	vector<shared_ptr<ActionNode> > nodesForAgent;
	
	for (size_t i=0; i<allActionNodes.size(); ++i)
	{
		if(allActionNodes[i]->getPartition_at(0)->getName() == _agentToPartition[agent->toString()]->getName())
		{
			nodesForAgent.push_back(allActionNodes[i]);
		}
	}

	return nodesForAgent;
}

shared_ptr<ActionNode> ProcedureExecution::getActionByNameFor(shared_ptr<AID> agent, string action)
{
	//assuming only one action exists by that name !
	
	vector<shared_ptr<ActionNode> > allActionNodes = getAllActions();
	
	shared_ptr<ActionNode> found;
	
	for (size_t i=0; i<allActionNodes.size(); ++i)
	{
		_agentToPartition[agent->toString()]->getName();
		
		if ((allActionNodes[i]->getPartition_at(0)->getName() == _agentToPartition[agent->toString()]->getName()) && (action == clean(allActionNodes[i]->getName())))
		{
			found = allActionNodes[i];
			break;
		}
	}

	return found;
}

vector<shared_ptr<ActionNode> > ProcedureExecution::getActionToExecute()
{
	return getActionToExecuteFor(_agent->getAID());
}

vector<shared_ptr<ActionNode> > ProcedureExecution::getActionToExecuteFor(shared_ptr<AID> agent)
{
	vector< shared_ptr<ActionNode> > toExec;
	
	vector<shared_ptr<ActionNode> > allActionsToExecute = getActionToExecuteForAll();
	
	for (size_t i=0; i<allActionsToExecute.size(); ++i)
	{
		//check author
		if(allActionsToExecute[i]->getPartition_at(0)->getName() == _agentToPartition[agent->toString()]->getName())
		{
			toExec.push_back(allActionsToExecute[i]);
		}
	}
	
	return toExec;
}

vector<shared_ptr<ActionNode> > ProcedureExecution::getActionToExecuteForAll()
{
	//this function simulates token advancement to provide the list of actions that can be executed
	//an action can be executed if it makes a difference in the procedure (if it enables a token to advance)
	
	//prepare output vector
	vector< shared_ptr<ActionNode> > toExec;
	
	//build list of all reachable actions (this list is a super-set which includes the actions to execute). we will filter this list
	vector< shared_ptr<ActionNode> > allPossibleActionsToExecute = getAllActions();
	
	//backup current activeTokens
	vector<shared_ptr<ActivityExecutionToken> > activeTokensBackup = _activeTokens;
	
	//now filter the list by creating dummy tokens and simulate their advancement
	for (size_t i=0; i<allPossibleActionsToExecute.size(); ++i)
	{
		//create dummy list
		_activeTokens.clear();
		for (size_t j=0; j<activeTokensBackup.size(); ++j)
		{
			//copy the token into a dummy
			shared_ptr<ActivityExecutionToken> dummyToken = make_shared<ActivityExecutionToken>();
			dummyToken->currentLocation = activeTokensBackup[j]->currentLocation;
			dummyToken->forkStack = activeTokensBackup[j]->forkStack;
			dummyToken->loopNodeStack = activeTokensBackup[j]->loopNodeStack;
			dummyToken->outgoingEdge = activeTokensBackup[j]->outgoingEdge;
		
			_activeTokens.push_back(dummyToken);
		}
		
		//test to see if this action is able to advance a token
		for (size_t j=0; j<_activeTokens.size(); ++j)
		{
			//advance token without the action (just to be sure we don't get fooled into thinking it advanced because of the action when it advances because of some other conditions that came true)
			if (ACTIVATE_DEBUG_CONSOLE_OUTPUT_TOKENS) cerr << "advancing with dummy action" << endl;
			shared_ptr<ActionNode> dummyActionNode;
			_tryToAdvanceToken(_activeTokens[j], dummyActionNode); //care not about the result //TODO .... check this out
			
			if (ACTIVATE_DEBUG_CONSOLE_OUTPUT_TOKENS) cerr << "advancing dummy token" << endl;
			
			//see if the dummy advances with the current action
			if (_tryToAdvanceToken(_activeTokens[j], allPossibleActionsToExecute[i]))
			{
				//dummy token advanced, so this action is an action to execute
				toExec.push_back(allPossibleActionsToExecute[i]);
				if (ACTIVATE_GENERAL_DEBUG) cerr << "allPossibleActionsToExecute[" << i << "] = " << allPossibleActionsToExecute[i]->getName() << endl;
				break;
			}
		}
	}
	
	//reinstate backed up activeTokens
	_activeTokens.clear();
	_activeTokens = activeTokensBackup;
	
	//remove already running nodes :: TODO : check if we should allow this or not....
	for (size_t i=0; i<_actionsRunning.size(); ++i)
	{
		for (size_t j=0; j<toExec.size(); ++j)
		{
			if (_actionsRunning[i].second == toExec[j])
			{
				toExec.erase(toExec.begin()+j);
				j--;
			}
		}
	}
	
	return toExec;
}

bool ProcedureExecution::_actionIsCurrentlyRunning(shared_ptr<ActionNode> action)
{
	//remove already running nodes :: TODO : check if we should allow this or not....
	for (size_t i=0; i<_actionsRunning.size(); ++i)
	{
		if (_actionsRunning[i].second == action)
		{
			return true;
		}
	}
	
	return false;
}

vector<string> ProcedureExecution::getAllActionsDoneBy(shared_ptr<AID> agent)
{
	// TODO: Maybe retyrb ActionNode and not name ?
	vector<string> actions;
	vector< shared_ptr<ActionNode> > & nodes = _allActionsDone[agent->toString()];
	for(size_t i = 0; i < nodes.size(); i++)
	{
		actions.push_back(clean(nodes[i]->getName()));
	}
	return actions;
}

vector<TimeExpression> ProcedureExecution::getAllActionsDoneTimestampsBy(shared_ptr<AID> agent)
{
	return _allActionsDoneTimestamps[agent->toString()];
}

vector<shared_ptr<AID> > ProcedureExecution::getOtherAgents()
{
	vector<shared_ptr<AID> > others;
	map<shared_ptr<ActivityPartition>, shared_ptr<AID> >::iterator it=_partitionToAgent.begin();
	
	while(it != _partitionToAgent.end())
	{
		if(it->second->toString() != _agent->getAID()->toString())
		{
			others.push_back(it->second);
		}
		
		it++;
	}
	
	return others;
}

void ProcedureExecution::printState()
{
	/*
	cerr << _agent->getName() << endl;
	cerr << "Noeud a executer:"<<endl;
	
	vector<shared_ptr<ActionNode> >::iterator it=toExecuteNodes.begin();
	
	while(it != toExecuteNodes.end())
	{
		cerr << "--> "<<(*it)->getName()<< " par "<< partitionToAgent[(*it)->getPartition_at(0)]->getName()<<endl;
		it++;
	}
	
	cerr << "Noeud en cours d'execution:"<<endl;
	it=runningNodes.begin();
	
	while(it != runningNodes.end())
	{
		cerr << "--> "<<(*it)->getName()<< " par "<< partitionToAgent[(*it)->getPartition_at(0)]->getName()<<endl;
		it++;
	}
	
	cerr << "Evenement running recu:"<<endl;
	vector<pair<string,string> >::iterator it2=actionsRunning.begin();
	
	while(it2 != actionsRunning.end())
	{
		cerr << "--> "<<it2->first << " running "<<it2->second << endl;
		it2++;
	}
	
	cerr << "Evenement done recu:"<<endl;
	it2=actionsDone.begin();
	
	while(it2 != actionsDone.end())
	{
		cerr << "--> "<<it2->first << " running "<<it2->second << endl;
		it2++;
	}
	*/
}

bool ProcedureExecution::isFinished()
{
	bool reachableNodesStillExist = false;
	for (size_t i=0; i<_activeTokens.size(); ++i)
	{
		if ((_activeTokens[i]->outgoingEdge) || (_activeTokens[i]->loopNodeStack.size()))
		{
			reachableNodesStillExist = true;
			break;
		}
	}
	
	return (!reachableNodesStillExist) && _actionsRunning.size() == 0;
}

void ProcedureExecution::informActionRunning(shared_ptr<AID> agent,const string& actionName)
{
        cerr << "ActionName : " << actionName << endl;
	shared_ptr<ActionNode> action = getActionByNameFor(agent, actionName);
	informActionRunning(agent, action);
}

void ProcedureExecution::informActionRunning(shared_ptr<AID> agent,const shared_ptr<ActionNode> action)
{
	if (ACTIVATE_GENERAL_DEBUG) cerr << "ProcedureExecution::informActionRunning ( " << agent->toString() << " , " << action->getName() << " ) on " << _agent->getAID()->toString() << endl;
	
	//check if procedure will advance if this action is made
	bool canAdvance = false;
	
	shared_ptr<ActionNode> actionToRun;
	// Test if action owned by agent
	cout << "Acion : " << action << endl;
	if(action->getPartition_at(0)->getName() == _agentToPartition[agent->toString()]->getName())
		actionToRun = action;
	else
		cerr << "This action " << action->getFullName() << " doesn't belong to agent " << agent->getName() << endl;
	
	if (actionToRun)
	{
		//check if procedure can advance (action is reachable from the previous done actions)
		
		vector< shared_ptr<ActionNode> > possibleNextActions = getActionToExecuteFor(agent);
		
		for (size_t i=0; i<possibleNextActions.size(); ++i)
		{
			if (actionToRun == possibleNextActions[i])
			{
				canAdvance = true;
				break;
			}
		}
	}
	
	if (canAdvance)
	{
		//save the running action in the list
		_actionsRunning.push_back(make_pair< string,shared_ptr<ActionNode> >(agent->toString(),action));
		
		//TODO: appologise because the hack is not so beautiful. the good part is that it works fine :)
		//update next reachable nodes list, if their current location is not a dead end (this caused problems with loopnodes for example...)
		//-- the problem was that loopnodes must not be restarted/skipped until the effect of its actions is understood. this is not the case with decision nodes for example
		shared_ptr<ActionNode> doneAction = action;
		
		if (doneAction)
		{
			bool advancedToken = false;
			for (size_t i=0; i<_activeTokens.size(); ++i)
			{
				if ( !(_activeTokens[i]->outgoingEdge) ||
						(_activeTokens[i]->outgoingEdge->getTarget()->getOutgoingControlFlowEdges().size() == 0))
				{
					continue; //skip deadends
				}
				
				//try to advance
				if (ACTIVATE_DEBUG_CONSOLE_OUTPUT_TOKENS) cerr << "> trying to advance token " << (i+1) << "/" << _activeTokens.size() << " from " << _activeTokens[i]->currentLocation->getName() << endl;
				
				if (_tryToAdvanceToken(_activeTokens[i], doneAction))
				{
					if (ACTIVATE_DEBUG_CONSOLE_OUTPUT_TOKENS) cerr << "   >>  advanced token " << (i+1) << "/" << _activeTokens.size() << " to " << _activeTokens[i]->currentLocation->getName() << endl;
					
					advancedToken = true;
				}
			}
			
			if (advancedToken)
			{
				//add to "done" list
				_actionsDone.push_back(doneAction);
				
				_allActionsDone[agent->toString()].push_back(action); //should have been in the IF, but i'm afraid right now
				_allActionsDoneTimestamps[agent->toString()].push_back(doneAction->getCurrentExecution()->getFinish());
			}
			else
			{
				if (ACTIVATE_GENERAL_DEBUG) cerr << "HUSTON, WE'VE GOT A PROBLEM... no token was advanced for " << doneAction->getName() << endl;
			}
		}
	}
}

void ProcedureExecution::informActionDone(shared_ptr<AID> agent,const string& actionName)
{
	shared_ptr<ActionNode> action = getActionByNameFor(agent, actionName);
	informActionDone(agent, action);
}

void ProcedureExecution::informActionDone(shared_ptr<AID> agent,const shared_ptr<ActionNode> action)
{
	if (ACTIVATE_GENERAL_DEBUG) cerr << "ProcedureExecution::informActionDone ( " << agent->toString() << " , " << action->getName() << " ) on " << _agent->getAID()->toString() << endl;
	
	bool foundAndRemoved = false;
	
	//remove from "running" list
	for (size_t i=0; i<_actionsRunning.size(); ++i)
	{
		if ((_actionsRunning[i].first == agent->toString()) && (_actionsRunning[i].second == action))
		{
			StlVectorFastErase(_actionsRunning, i);
			foundAndRemoved = true;
			break;
		}
	}
	
	if (foundAndRemoved)
	{
		//TODO: appologise because the hack is not so beautiful. the good part is that it works fine :)
		//re-update next reachable nodes list, just in case we had a loopnode case... damn these loopnodes...
		//-- the problem was that loopnodes must not be restarted/skipped until the effect of its actions is understood. this is not the case with decision nodes for example
		shared_ptr<ActionNode> doneAction = action;
		
		if (doneAction)
		{
			bool advancedToken = false;
			for (size_t i=0; i<_activeTokens.size(); ++i)
			{
				//try to advance
				if (ACTIVATE_DEBUG_CONSOLE_OUTPUT_TOKENS) cerr << "> trying to advance token " << (i+1) << "/" << _activeTokens.size() << " from " << _activeTokens[i]->currentLocation->getName() << endl;
				
				if (_tryToAdvanceToken(_activeTokens[i], doneAction))
				{
					if (ACTIVATE_DEBUG_CONSOLE_OUTPUT_TOKENS) cerr << "   >>  advanced token " << (i+1) << "/" << _activeTokens.size() << " to " << _activeTokens[i]->currentLocation->getName() << endl;
					
					advancedToken = true;
				}
			}
			
			if (advancedToken)
			{
				//add to "done" list
				_actionsDone.push_back(doneAction);
				
				_allActionsDone[agent->toString()].push_back(doneAction); //should have been in the IF, but i'm afraid right now
				_allActionsDoneTimestamps[agent->toString()].push_back(doneAction->getCurrentExecution()->getFinish());
			}
			else
			{
				if (ACTIVATE_GENERAL_DEBUG) cerr << "HUSTON, WE'VE GOT A PROBLEM... no token was advanced for " << doneAction->getName() << endl;
			}
		}
	}
}

bool ProcedureExecution::_tryToAdvanceToken(shared_ptr<ActivityExecutionToken> token, shared_ptr<ActionNode> doneAction, bool gotTicketToUseAction)
{
	//TODO : current implementation will fail if the loop node has branches which are not joined together at the end
	
	//TODO : setup part of loop nodes not implemented
	//TODO : careful if the loop node contains only an initial node :: ENDLESS LOOP
	
	if (token->currentLocation->getKind() == "action")
	{
		shared_ptr<ActionNode> aNode = shared_dynamic_cast<ActionNode>(token->currentLocation);
		if (_actionIsCurrentlyRunning(aNode))
		{
			//token busy...
			return false;
		}
	}
	

	if (!(token->outgoingEdge))
	{
		if (ACTIVATE_DEBUG_CONSOLE_OUTPUT_TOKENS) cerr << "dbg Part1: edge null" << endl;
		
		//my work here is done *flies away*
		return false;
	}
	
	//evaluate guard(s). Normal behavior for fork, but I treat decision nodes here, if there are more paths going out, I will take the first one.
	if (token->currentLocation->getKind() == "fork")
	{
		if (ACTIVATE_DEBUG_CONSOLE_OUTPUT_TOKENS) cerr << "dbg Part1: fork" << endl;
		
		//normal behavior for fork (should not have guard, but hell... it doesn't hurt to check)
		if (token->outgoingEdge->getGuard() && (!(token->outgoingEdge->getGuard()->eval(_procedure->getActivity()->getContext()))))
		{
			return false; //eval failed
		}
	}
	else
	{
		if (ACTIVATE_DEBUG_CONSOLE_OUTPUT_TOKENS) cerr << "dbg Part1: no fork" << endl;
		
		vector<shared_ptr<ActivityEdge> > outgoingNormal = token->currentLocation->getOutgoingControlFlowEdges();
		bool foundWayOut = false;
		for (size_t i=0; i<outgoingNormal.size(); ++i)
		{
			//if <no guard> or <guard but evaluates ok>
			if ((!outgoingNormal[i]->getGuard()) || (outgoingNormal[i]->getGuard() && (outgoingNormal[i]->getGuard()->eval(_procedure->getActivity()->getContext()))))
			{
				foundWayOut = true;
				token->outgoingEdge = outgoingNormal[i]; //set new direction, we take the first possible one, remember ?
			}
		}
		
		if (!foundWayOut)
		{
			if (ACTIVATE_DEBUG_CONSOLE_OUTPUT_TOKENS) cerr << "dbg Part1: didn't find way out" << endl;
			
			return false; //no way out... leave for later
		}
	}
	
	shared_ptr<ActivityNode> nextLocation = token->outgoingEdge->getTarget();
	
	//test allowed action
	if (nextLocation->getKind() == "action")
	{
		if ((nextLocation != doneAction) || (!gotTicketToUseAction))
		{
			//not allowed to advance to this action
			
			if (ACTIVATE_DEBUG_CONSOLE_OUTPUT_TOKENS) cerr << "dbg Part1: action not allowed" << endl;
			
			return false;
		}
		
		gotTicketToUseAction = false;
	}
	
	if (ACTIVATE_DEBUG_CONSOLE_OUTPUT_TOKENS) cerr << "adv tok from '" << token->currentLocation->getName() << "' to '" << nextLocation->getName() << "'" << endl;
	
	//Advance
	shared_ptr<ActivityEdge> dummyActivityEdge;
	token->currentLocation = nextLocation;
	token->outgoingEdge = dummyActivityEdge; //clear outgoing... will decide in the following code
	
	//Decide next move (set outgoingEdge)
	
	vector<shared_ptr<ActivityEdge> > outgoingAfterAdvance = token->currentLocation->getOutgoingControlFlowEdges();
	
	vector<shared_ptr<ActivityExecutionToken> > nextTokens;
	
	if (token->currentLocation->getKind() == "fork")
	{
		if (ACTIVATE_DEBUG_CONSOLE_OUTPUT_TOKENS) cerr << "dbg Part2: fork" << endl;
		
		//time to split token into pieces, in order to follow all fork branches
		
		token->forkStack.push_back(token->currentLocation); //push this fork to the stack
		
		//remove this token from the active list
		for (size_t i=0; i<_activeTokens.size(); ++i)
		{
			if (_activeTokens[i] == token)
			{
				StlVectorFastErase(_activeTokens, i);
				break;
			}
		}
		
		if (ACTIVATE_DEBUG_CONSOLE_OUTPUT_TOKENS) cerr << "  spawning " << outgoingAfterAdvance.size() << " tokens" << endl;
		
		for (size_t i=0; i<outgoingAfterAdvance.size(); ++i)
		{
			shared_ptr<ActivityExecutionToken> tok = make_shared<ActivityExecutionToken>();
			tok->currentLocation = token->currentLocation;
			tok->forkStack = token->forkStack;
			tok->loopNodeStack = token->loopNodeStack;
			tok->outgoingEdge = outgoingAfterAdvance[i]; //set outgoing, to know which branch to follow
			
			//push to active list
			_activeTokens.push_back(tok);
			
			//save for further use
			nextTokens.push_back(tok);
		}
	}
	else if (token->currentLocation->getKind() == "join")
	{
		//we can only pass by a join, if all other branched tokens have arrived in this join
		//the point is that the number of tokens which have the same last fork as this one, should be the same as the nr of incoming links to this join
		//i hope i understand this next time i read it
	
		vector< shared_ptr<ActivityEdge> > incoming = token->currentLocation->getIncoming(); //so get the incoming
	
		//see which is the last fork of this token
		shared_ptr<ActivityNode> lastFork;
		if (token->forkStack.size())
		{
			lastFork = token->forkStack[token->forkStack.size()-1];
		}
	
		if (lastFork)
		{
			//find all tokens that have this fork and see if they all finished their branch // TODO: remember that actually, the forkStacks should be identical for paired tokens... if they're not, we've got a problem....
			vector<shared_ptr<ActivityExecutionToken> > finishedTokens;
			for (size_t i=0; i<_activeTokens.size(); ++i)
			{
				//if found a token with this last fork
				if ((_activeTokens[i]->forkStack.size()) && (_activeTokens[i]->forkStack[_activeTokens[i]->forkStack.size()-1] == lastFork)) 
				{
					//check to see if it's finished (arrived at the same join)
					if (_activeTokens[i]->currentLocation == token->currentLocation)
					{
						//add to finished tokens list
						finishedTokens.push_back(_activeTokens[i]);
					}
				}
			}
		
			//now check to see if the number of found tokens equals the number of incoming links to this join
			if (finishedTokens.size() == incoming.size())
			{
				if (ACTIVATE_DEBUG_CONSOLE_OUTPUT_TOKENS) cerr << "  joining " << finishedTokens.size() << " tokens into one" << endl;
				
				//then all should be great.... we can merge all tokens into a single one
				for (size_t i=0; i<finishedTokens.size(); ++i)
				{
					//skip the current token, this one will remain
					if (finishedTokens[i] == token) continue;
				
					//seek and destroy
					for (size_t j=0; j<_activeTokens.size(); ++j)
					{
						if (finishedTokens[i] == _activeTokens[j])
						{
							//destroy
							StlVectorFastErase(_activeTokens, j);
						}
					}
				}
				
				if (outgoingAfterAdvance.size() == 1)
				{
					token->outgoingEdge = outgoingAfterAdvance[0];
					token->forkStack.pop_back(); //pop last fork
					nextTokens.push_back(token);
				}
				else if (outgoingAfterAdvance.size() > 1)
				{
					if (ACTIVATE_GENERAL_DEBUG) cerr << "ERROR: multiple outgoing edges for join node, not allowed" << endl;
				}
			}
		}
		else
		{
			if (ACTIVATE_GENERAL_DEBUG) cerr << "HUSTON, WE'VE GOT A PROBLEM.... there's a join without a fork" << endl;
		}
	}
	else if (token->currentLocation->getKind() == "loop")
	{
		shared_ptr<LoopNode> loopNode = shared_dynamic_cast<LoopNode>(token->currentLocation);
		
		bool skipThisLoopNode = false;
		
		if (loopNode->getTestedFirst())
		{
			if (loopNode->eval()) //thou shall pass
			{
				skipThisLoopNode = true;
			}
		}
		
		shared_ptr<ActivityNode> initLoop = loopNode->getInitialNode();
		vector<shared_ptr<ActivityEdge> > outgoingFromInitLoop;
		
		if (initLoop)
		{
			outgoingFromInitLoop = initLoop->getOutgoingControlFlowEdges();
		}
		
		if ( skipThisLoopNode || (!initLoop) || (outgoingFromInitLoop.size() < 1) )
		{
			//I don't care which path to take, so I will just take the first one. In the advance part (see above) if it's a decision node, it will take the first possible path, if any
			if (outgoingAfterAdvance.size() >= 1)
			{
				token->outgoingEdge = outgoingAfterAdvance[0];
				nextTokens.push_back(token);
			}
			
			if (ACTIVATE_GENERAL_DEBUG) if (!initLoop) cerr << "HUSTON, WE'VE GOT A PROBLEM.... there's a loop node without an initial node inside it" << endl;
		}
		else
		{
			//enter the loop node, we'll get the edge which exits the loopnode's initial node
			
			shared_ptr<ActivityEdge> dummyActivityEdge;
			token->currentLocation = initLoop;
			token->outgoingEdge = outgoingFromInitLoop[0];
			token->loopNodeStack.push_back(loopNode);
		}
	}
	else
	{
		//I don't care which path to take, so I will just take the first one. In the advance part (see above) if it's a decision node, it will take the first possible path, if any
		if (outgoingAfterAdvance.size() >= 1)
		{
			token->outgoingEdge = outgoingAfterAdvance[0];
			nextTokens.push_back(token);
		}
		else if (token->loopNodeStack.size()) //check if we're in a loop node
		{
			//if (ACTIVATE_DEBUG_CONSOLE_OUTPUT_TOKENS) cerr << "dbg Part1: edge null, but in loop node" << endl;
			
			while ((!(token->outgoingEdge)) && (token->loopNodeStack.size())) //while we are still in a loopnode and it is finished
			{
				shared_ptr<LoopNode> lastLoop = token->loopNodeStack[token->loopNodeStack.size()-1];
				if (lastLoop->eval()) //test exit condition
				{
					//exiting loop node
					
					vector<shared_ptr<ActivityEdge> > outgoingFromLoop = lastLoop->getOutgoingControlFlowEdges();
					
					if (outgoingFromLoop.size())
					{
						token->currentLocation = lastLoop;
						token->outgoingEdge = outgoingFromLoop[0];
					}
					
					token->loopNodeStack.pop_back(); //remove last loop from stack (exit)
				}
				else
				{
					//restarting loop node
					
					shared_ptr<ActivityNode> initLoop = lastLoop->getInitialNode();
					vector<shared_ptr<ActivityEdge> > outgoingFromInitLoop = initLoop->getOutgoingControlFlowEdges();
					
					token->currentLocation = initLoop;
					token->outgoingEdge = outgoingFromInitLoop[0]; //it should exist, given the fact that we've entered this loop node already
				}
			}
			
			if (token->outgoingEdge)
			{
				nextTokens.push_back(token);
			}
		}
	}
	
	//continue to advance
	for (size_t i=0; i<nextTokens.size(); ++i)
	{
		if (ACTIVATE_DEBUG_CONSOLE_OUTPUT_TOKENS) cerr << " recurse: advancing token " << (i+1) << "/" << nextTokens.size() << endl;
		
		_tryToAdvanceToken(nextTokens[i], doneAction, gotTicketToUseAction);
	}
	
	return true;
}

}
