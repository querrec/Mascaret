#include "VEHA/Behavior/BasicActions/Action.h"
#include "VEHA/Behavior/BasicActions/SendSignalAction.h"
#include "VEHA/Behavior/Activity/Activity.h"
#include "VEHA/Behavior/Activity/ActivityPartition.h"
#include "VEHA/Behavior/Activity/ActivityNode.h"
#include "VEHA/Behavior/Activity/ActivityEdge.h"
#include "VEHA/Behavior/Activity/ActionNode.h"
#include "VEHA/Behavior/Activity/LoopNode.h"
#include "VEHA/Kernel/Expression.h"
#include "VEHA/Behavior/Activity/ActivityBehaviorExecution.h"
#include "VEHA/Kernel/InstanceSpecification.h"
#include <boost/bind.hpp>


namespace VEHA
{
ActivityBehaviorExecution::ActivityBehaviorExecution(
		shared_ptr<Activity> activity, shared_ptr<InstanceSpecification> host, const Parameters& p, bool sync):
		BehaviorExecution(activity,host,p,sync),
		_activity(activity),
		_stateActivity(0)
{
	buildAffectations(p, host, activity);

	shared_ptr<ActivityExecutionToken> initialToken = make_shared<ActivityExecutionToken>();
	initialToken->currentLocation = activity->getInitialNode();

	if(!initialToken->currentLocation)
	{
		cerr << "[ActivityBehaviorExecution Error] No initial node found in activity ";
		cerr << activity->getName() << ". Activity will not start." << endl;
		return;
	}

	vector<shared_ptr<ActivityEdge> > controlFlows = activity->getInitialNode()->getOutgoingControlFlowEdges();

	if (controlFlows.size() == 1)
	{
			initialToken->outgoingEdge = controlFlows[0];
			_activeTokens.push_back(initialToken);
			_tryToAdvanceToken(initialToken, shared_ptr<ActionNode>()); //first advancement
	}
	else
	{
		if (controlFlows.size() == 0)
		{
			cerr << "[ActivityBehaviorExecution Warning] Initial node has no outgoing edges" << endl;
		}
		else
		{
			cerr << "[ActivityBehaviorExecution Error] Multiple outgoing edges for the initial node, not allowed" << endl;
		}
		cerr << "Activity " << activity->getName() << " will not start." << endl;
	}
}

void ActivityBehaviorExecution::buildAffectations(Parameters params, shared_ptr<InstanceSpecification> host, shared_ptr<Activity> activity)
{
	vector<string> missing;

	cerr << "[ActivityBehaviorExecution Info] Affectations " << activity->getName() << endl;

	/* Affectation de self */
	_affectations["self"] = getHost();

	/* Affectation des parametres */
	//TODO: ?

	/* Affectation des ressources */
	vector<shared_ptr<ActivityPartition> > partitions = activity->getPartition();
	for (size_t i = 0; i < partitions.size(); i++)
	{
//cerr << partitions[i]->getName() << endl;
		if (partitions[i]->getName() == "this")
		{
			_affectations["this"] = getHost();
		}
		else
		{
			string pname = partitions[i]->getName();
			if (params.find(pname) != params.end())
			{
				_affectations[pname] = (shared_dynamic_cast<InstanceValue>(params.find(pname)->second))->getValue();
			}
			else
			{
				map<string,shared_ptr<Slot> > slots = host->getSlots();
				map<string,shared_ptr<Slot> >::iterator it = slots.find(pname);
				if (it != slots.end())
					_affectations[pname] = *host->getProperty(pname)->getValue();
				else
				{
					cerr << "[ActivityBehaviorExecution Warning] Affectation Partition de " << pname << " impossible ..." << endl;
					missing.push_back(pname);
				}
			}
		}
		vector< shared_ptr<ActivityNode> > nodes = partitions[i]->getNode();
		cerr << " SIZE : " << nodes.size() << endl;
		for (size_t iNode = 0; iNode<nodes.size(); iNode++)
		{
			shared_ptr<ActivityNode> node = nodes[iNode];
//cerr << " NODES : " << nodes[iNode]->getName() << endl;
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

	cerr << "[ActivityBehaviorExecution Info] Fin affectations." << endl;

	activity->setContext(_buildOclContext(params));
}

void ActivityBehaviorExecution::_buildAffectationsObject(vector<string> & missing, shared_ptr<ActivityNode> node)
{
	string pname = node->getName();

	cerr << " Parameters size : " << parameters.size() << endl;
	Parameters::const_iterator it = parameters.find(pname);
	if (it != parameters.end())
	{
		_affectations[pname] = (shared_dynamic_cast<InstanceValue>(it->second))->getValue();
	}
	else
	{
		map<string,shared_ptr<Slot> > slots = getHost()->getSlots();
		map<string,shared_ptr<Slot> >::iterator it = slots.find(pname);
		if (it != slots.end())
		{
			// FIXME: Workaround, doesn't support multiple relation, get the last one (the first one in XML file)
			try
			{
				_affectations[pname] = *(getHost()->getProperty(pname)->getValue());
			}
			catch(std::range_error& e)
			{
				cerr << "Multiple relation found, not supported, get the last one" << endl;
				map< string, shared_ptr<ValueSpecification> > values = getHost()->getProperty(pname)->getValues();
				for(map< string, shared_ptr<ValueSpecification> >::iterator it = values.begin(); it != values.end(); ++it)
				{
					cerr << "key: " << it->first << " values: " << it->second << endl;
					_affectations[pname] = *it->second;
				}
				//_affectations[pname] = *(values.end()->second);
			}
		}
		else
		{
			cerr << "[ActivityBehaviorExecution Warning] Affectation Object de " << pname << " impossible ..." << endl;
			missing.push_back(pname);
		}
	}
}

void ActivityBehaviorExecution::_findRessources(vector<string> missings)
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

void ActivityBehaviorExecution::_buildAffectationsPostbone(vector<string> & missing)
{
	cerr << "[ActivityBehaviorExecution Warning] Il y a des ressources non affectées ..." << endl;
	_findRessources(missing);
}

void ActivityBehaviorExecution::_buildAffectationsAction(vector<string> & missing, shared_ptr<ActivityNode> node)
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
				if (parameters.find(pname) != parameters.end())
				{
					shared_ptr<InstanceSpecification> is = (shared_dynamic_cast<InstanceValue>(parameters.find(pname)->second))->getValue();
					_affectations[pname] = is;
					sendAct->setTarget(is);
				}
				else
				{
					map<string,shared_ptr<Slot> > slots = getHost()->getSlots();
					map<string,shared_ptr<Slot> >::iterator it = slots.find(pname);
					if (it != slots.end())
					{
						_affectations[pname] = *(getHost()->getProperty(pname)->getValue());
						sendAct->setTarget(*(getHost()->getProperty(pname)->getValue()));
					}
					else
					{
						cerr << "[ActivityBehaviorExecution.cpp] Affectation SendAction de " << pname << " impossible..." << endl;
					}
				}
			}
		}
		else
		{
			// TODO: Affectation for other kind of action
		}
	}
}

ActivityBehaviorExecution::~ActivityBehaviorExecution()
{
}

void ActivityBehaviorExecution::fire(shared_ptr<ActivityNode> /*activityNode*/)
{
}

void ActivityBehaviorExecution::stop()
{
	BehaviorExecution::stop();
	for (size_t i = 0; i < _execs.size(); i++)
		_execs[i]->stop();
}

#if 0
double ActivityBehaviorExecution::execute(double dt)
{
	if (getSync())
	{
		shared_ptr<Activity> activity = getActivity();
		shared_ptr<ActivityNode> initial = activity->getInitialNode();
		vector < shared_ptr<ActionNode> > actions ;

		if (initial)
		{
			actions = initial->getOutgoingActionNode();
		}
		else return 0;

		while (actions.size())
		{
			for (size_t iA = 0; iA < actions.size(); iA++)
			{
				if (actions[iA]->getKind() == "action")
				{
					//cerr << "Running : " << actions[iA]->getName() << endl;
					string roleName = actions[iA]->getPartition()[0]->getName();
					(shared_dynamic_cast<ActionNode>(actions[iA]))->start(_affectations[roleName], _affectations, getSync());
				}
			}
			vector< shared_ptr<ActionNode> > next;
			for (size_t i = 0; i<_actions.size(); i++)
			{
				vector<shared_ptr<ActionNode> > tmp = _actions[i]->getOutgoingActionNode();
				for (size_t j = 0; j < tmp.size() ; j++)
					next.push_back(tmp[j]);
			}
			actions = next;
		}
	}
	else
	{
		if (_stateActivity == 0)
		{
			shared_ptr<Activity> activity = getActivity();
			shared_ptr<ActivityNode> initial = activity->getInitialNode();

			if (initial)
			{
				vector < shared_ptr<ActionNode> > actions = initial->getOutgoingActionNode();
				for (size_t iA = 0; iA < actions.size(); iA++)
				{
					if (actions[iA]->getKind() == "action")
					{
						cerr << "Running : " << actions[iA]->getName() << endl;
						cerr << "Partitions:" << actions[iA]->getPartition().size() << endl;
						shared_ptr<ActivityPartition> partition = actions[iA]->getPartition()[0];
						string roleName = partition->getName();
						(shared_dynamic_cast<ActionNode>(actions[iA]))->start(_affectations[roleName], _affectations);
					}
					_execs.push_back(actions[iA]);
					_actions.push_back(actions[iA]);
				}
				_stateActivity ++;
			}
		}

		if (_stateActivity == 1)
		{
			if (_execs.size())
			{
				size_t nbAction = _execs.size();
				for (size_t i = 0; i < _execs.size(); i++)
				{
					if (!_execs[i]->isRunning()) nbAction--;
				}
				if (!nbAction) _execs.clear();
			}
			else
			{
				if(_stateActivity == 1)
				{
					vector< shared_ptr<ActionNode> > next;
					for (size_t i = 0; i<_actions.size(); i++)
					{
						vector<shared_ptr<ActionNode> > tmp = _actions[i]->getOutgoingActionNode();
						for (size_t j = 0; j < tmp.size() ; j++)
						{
							//cerr << tmp[i]->getName() << endl;
							next.push_back(tmp[j]);
						}
					}
					_actions.clear();
					if (next.size())
					{
						for( size_t iAction = 0; iAction < next.size(); iAction++)
						{
							if (next[iAction]->getKind() == "action")
							{
								cerr << "Running : " << next[iAction]->getName() << endl;
								string roleName = next[iAction]->getPartition()[0]->getName();
								(shared_dynamic_cast<ActionNode>(next[iAction]))->start(_affectations[roleName], _affectations);
							}
							else
							{
								cerr << "Not action Running : " << next[iAction]->getName() << endl;
							}
							//next[iAction]->start(_affects[next[iAction]->getPartition()->getName()],_affects);
							_execs.push_back(next[iAction]);
							_actions.push_back(next[iAction]);
						}
					}
					else
					{
						_stateActivity ++;
					}
				}
			}
		}
		else
		{
			//_affects.clear();
			_execs.clear();
			_actions.clear();
			return 0;
		}
	}
	return 0.01;
}
#else
double ActivityBehaviorExecution::execute(double dt)
{
	vector<shared_ptr<ActionNode> > actionNodes = getActionToExecute();

	if (actionNodes.size())
	{
		for(size_t i=0;i<actionNodes.size();i++)
		{
			cerr << "{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{ Actions: " << actionNodes[i]->getName() << endl;
			shared_ptr<ActivityPartition> partition = actionNodes[i]->getPartition()[0];
			string roleName = partition->getName();
			actionNodes[i]->start(_affectations[roleName], _affectations);
			informActionRunning(actionNodes[i]);

			if(actionNodes[i]->getCurrentExecution())
			{
				cerr << "actionNodes[" << i << "] " << actionNodes[i]->getName() << " (" << actionNodes[i] << "), bind stop callback." << endl;
				_behaviorToNode[actionNodes[i]->getCurrentExecution().get()]=actionNodes[i];
				actionNodes[i]->getCurrentExecution()->addCallbackOnBehaviorStop(bind(&ActivityBehaviorExecution::_onBehaviorStop,this,_1));
			}
			else
			{
				informActionDone(actionNodes[i]);
			}
		}
	}
	else
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

		if(!reachableNodesStillExist)
			return 0;
	}

	return 0.01;
}
#endif

#include "VEHA/Behavior/Activity/LoopNode.h"

#define ACTIVATE_DEBUG_CONSOLE_OUTPUT_TOKENS false
#define ACTIVATE_GENERAL_DEBUG false

template <typename T> inline
void StlVectorFastErase(vector<T> & vect, unsigned int index)
{
	vect[index]=vect.back();
	vect.pop_back();
}

bool ActivityBehaviorExecution::_tryToAdvanceToken(shared_ptr<ActivityExecutionToken> token, shared_ptr<ActionNode> doneAction, bool gotTicketToUseAction)
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
		if (token->outgoingEdge->getGuard() && (!(token->outgoingEdge->getGuard()->eval(_activity->getContext()))))
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
			if ((!outgoingNormal[i]->getGuard()) || (outgoingNormal[i]->getGuard() && (outgoingNormal[i]->getGuard()->eval(_activity->getContext()))))
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

bool ActivityBehaviorExecution::_actionIsCurrentlyRunning(shared_ptr<ActionNode> action)
{
	//remove already running nodes :: TODO : check if we should allow this or not....
	for (size_t i=0; i<_actionsRunning.size(); ++i)
	{
		if (_actionsRunning[i] == action)
		{
			return true;
		}
	}

	return false;
}

vector<shared_ptr<ActionNode> > ActivityBehaviorExecution::getAllActions()
{
	vector<shared_ptr<ActivityNode> > allNodes = _activity->getNode();
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

vector<shared_ptr<ActionNode> > ActivityBehaviorExecution::getActionToExecute()
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
			if (_actionsRunning[i] == toExec[j])
			{
				if (ACTIVATE_GENERAL_DEBUG) cerr << "Removing already running action " << _actionsRunning[i]->getName() << endl;
				toExec.erase(toExec.begin()+j);
				j--;
			}
		}
	}

	return toExec;
}

void ActivityBehaviorExecution::informActionRunning(const shared_ptr<ActionNode> actionToRun)
{
	//check if procedure will advance if this action is made
	bool canAdvance = false;

	if (actionToRun)
	{
		//check if procedure can advance (action is reachable from the previous done actions)

		vector< shared_ptr<ActionNode> > possibleNextActions = getActionToExecute();

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
		_actionsRunning.push_back(actionToRun);

		//TODO: appologise because the hack is not so beautiful. the good part is that it works fine :)
		//update next reachable nodes list, if their current location is not a dead end (this caused problems with loopnodes for example...)
		//-- the problem was that loopnodes must not be restarted/skipped until the effect of its actions is understood. this is not the case with decision nodes for example
		shared_ptr<ActionNode> doneAction = actionToRun;

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
			}
			else
			{
				if (ACTIVATE_GENERAL_DEBUG) cerr << "HUSTON, WE'VE GOT A PROBLEM... no token was advanced for " << doneAction->getName() << endl;
			}
		}
	}
}

void ActivityBehaviorExecution::informActionDone(const shared_ptr<ActionNode> action)
{
//	cerr << __PRETTY_FUNCTION__ << endl;
	bool foundAndRemoved = false;

	//remove from "running" list
	cerr << "Runnings actions:" << _actionsRunning.size() << endl;
	for (size_t i=0; i<_actionsRunning.size(); ++i)
	{
		if(_actionsRunning[i])
			cerr << " --- " << _actionsRunning[i] << endl;
		else
			cerr << " --- " << i << ": " << _actionsRunning[i] << endl;
		if(_actionsRunning[i] == action)
		{
			cerr << " !!!! Is done action " << endl;
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
			}
			else
			{
				if (ACTIVATE_GENERAL_DEBUG) cerr << "HUSTON, WE'VE GOT A PROBLEM... no token was advanced for " << doneAction->getName() << endl;
			}
		}
	}
}

void ActivityBehaviorExecution::_onBehaviorStop(BehaviorExecution* be)
{
	informActionDone(_behaviorToNode[be]);
	restart();
}

} // Namespace
