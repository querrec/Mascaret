using System;
using System.Collections;
using System.Collections.Generic;
using System.IO;

namespace Mascaret
{
    public class ProcedureExecution
    {

        public Procedure procedure;
        public OrganisationalEntity organisation;
        public Agent agent;

        protected List<ActionNode> actionsDone;
        protected List<KeyValuePair<string, ActionNode>> actionsRunning;
        protected Dictionary<String, List<ActionNode>> allActionsDone; // key: AID
        protected Dictionary<String, List<TimeExpression>> allActionsDoneTimestamps;
        protected Dictionary<String, ActivityPartition> agentToPartition;
        protected Dictionary<ActivityPartition, AID> partitionToAgent;
        protected Dictionary<String, ValueSpecification> activityParams;
        protected Dictionary<String, InstanceSpecification> affectations;
        protected List<ActivityExecutionToken> activeTokens;

        public ProcedureExecution(Procedure procedure, OrganisationalEntity organisation, Agent agent)
        {
            this.procedure = procedure;
            this.organisation = organisation;
            this.agent = agent;

            agentToPartition = new Dictionary<string, ActivityPartition>();
            partitionToAgent = new Dictionary<ActivityPartition, AID>();
            activeTokens = new List<ActivityExecutionToken>();
            affectations = new Dictionary<string, InstanceSpecification>();
            activityParams = new Dictionary<string, ValueSpecification>();
            actionsRunning = new List<KeyValuePair<string, ActionNode>>();
            actionsDone = new List<ActionNode>();
            allActionsDone = new Dictionary<string, List<ActionNode>>(); // key: AID
            allActionsDoneTimestamps = new Dictionary<string, List<TimeExpression>>();

            for (int i = 0; i < procedure.Activity.Partitions.Count; i++)
            {
                ActivityPartition partition = procedure.Activity.Partitions[i];
                List<RoleAssignement> assignements = organisation.RoleAssignement;
                for (int j = 0; j < assignements.Count; j++)
                {
                    if (partition.name == assignements[j].Role.name)
                    {
                        agentToPartition.Add(assignements[j].Agent.toString(), partition);
                        partitionToAgent.Add(partition, assignements[j].Agent);
                    }
                }
            }

            ActivityExecutionToken initialToken = new ActivityExecutionToken();
            initialToken.currentLocation = procedure.Activity.Initial;

            if (initialToken.currentLocation == null)
            {
                System.Console.WriteLine("ERROR: no initial node found, not allowed");
                System.Console.WriteLine("Procedure will not start.");
                return;
            }

            List<ActivityEdge> controlFlows = procedure.Activity.Initial.getOutgoingControlFlowEdges();

            if (controlFlows.Count == 1)
            {
                initialToken.outgoingEdge = controlFlows[0];
                activeTokens.Add(initialToken);

                ActionNode dummyActionVariable = null;
                tryToAdvanceToken(initialToken, dummyActionVariable,true); //first advancement
            }
            else
            {
                if (controlFlows.Count == 0)
                {
                    System.Console.WriteLine("WARNING: initial node has no outgoing edges");
                }
                else
                {
                    System.Console.WriteLine("ERROR: multiple outgoing edges for the initial node, not allowed");
                }
                System.Console.WriteLine("Procedure will not start.");
            }
        }

        protected void buildAffectationsObject(ref List<string> missing, ActivityNode node)
        {
            string pname = node.name;

            //  MascaretApplication.Instance.logfile.WriteLine("TRY TO AFFECT OBJECT : " + pname); MascaretApplication.Instance.logfile.Flush();
            RessourceAssignement ra = organisation.findRessourceAssignement(pname);
            if (ra != null && ra.Entity != null)
            {
                affectations.Add(pname, ra.Entity);
                // MascaretApplication.Instance.logfile.WriteLine("SUCCESS TO AFFECT OBJECT : " + pname); MascaretApplication.Instance.logfile.Flush();
            }
            else
            {
                //Parameters::iterator it = _activityParams.find(pname);
                if (activityParams.ContainsKey(pname))
                {
                    affectations.Add(pname, ((InstanceValue)(activityParams[pname])).SpecValue);
                }
                else
                {
                    //    MascaretApplication.Instance.logfile.WriteLine("FAIL TO AFFECT OBJECT : " + pname); MascaretApplication.Instance.logfile.Flush();
                    missing.Add(pname);
                }
            }
        }

        protected void buildAffectationsAction(ref List<string> missing, ActivityNode node)
        {
            Action action = ((ActionNode)(node)).Action;

            if (action != null)
            {
                if (action.Kind == "SendSignal")
                {
                    SendSignalAction sendAct = (SendSignalAction)(action);
                    string pname = sendAct.Target.targetName;
                    if (pname != "")
                    {
                        RessourceAssignement ra = organisation.findRessourceAssignement(pname);

                        if (ra != null && ra.Entity != null)
                        {
                            affectations.Add(pname, ra.Entity);
                            sendAct.Target.target = ra.Entity;
                        }
                        else
                        {
                            missing.Add(pname);
                        }
                    }
                }
                else if (action.Kind == "CallOperation")
                {
                    CallOperationAction callAct = (CallOperationAction)(action);
                    InstanceSpecification host = (InstanceSpecification)(agent);
                    Dictionary<string, Operation> opers = host.Classifier.Operations;
                    if (opers.ContainsKey(callAct.Operation.name))
                    {
                        callAct.Operation = opers[callAct.Operation.name];
                    }
                }
                else if (action.Kind == "CallBehavior")
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
                }
            }
        }

        protected void findRessources(List<string> missings)
        {

            // FileStream wFile;
            // StreamWriter file;

            // wFile = new FileStream("findRessource.txt", FileMode.Append);
            // file = new StreamWriter(wFile);
            // file.AutoFlush = true;
            bool affectAdded = false;

            List<string> always_missings = new List<string>(); // resources we always missing after a search pass

            for (int i = 0; i < missings.Count; i++)
            {
                bool missingAffected = false;
                //int it = 0;
                foreach (KeyValuePair<string, InstanceSpecification> entityp in affectations)
                // while (it < affectations.Count && !missingAffected)
                {
                    string missing = missings[i];

                    InstanceSpecification entity = entityp.Value;
                    // file.WriteLine(" LOOK FOR : " + missing + " in " + entity.name);
                    if (entity != null)
                    {
                        Slot slot = entity.getProperty(missing);
                        if (slot != null)
                        {
                            InstanceValue val = (InstanceValue)slot.getValue();
                            affectations.Add(missing, val.SpecValue);

                            missingAffected = true;
                            affectAdded = true;
                            //  file.WriteLine(missing + " in " + entity.name + " OK ");
                            break;

                        }
                        //else Debug.Log(missing + " in " + entity.name + " NOP");
                    }
                    else
                    {
                        //cerr << "ERROR >>> " << it->first << " NOT FOUND !!" << endl;
                    }
                    // it++;
                }
                if (!missingAffected) always_missings.Add(missings[i]);
            }

            /* If we have always missing resources but we have add some on this pass
             * we recursively search again.
             */

            //    file.Close();
            if (affectAdded) findRessources(always_missings);
        }

        protected void buildAffectationsPostbone(List<string> missing)
        {
            findRessources(missing);
        }

        public List<ActionNode> getAllActions()
        {
            List<ActivityNode> allNodes = procedure.Activity.Nodes;
            List<ActionNode> actionNodes = new List<ActionNode>();

            for (int i = 0; i < allNodes.Count; ++i)
            {
                if (allNodes[i].Kind == "LoopNode") continue;

                if (allNodes[i].Kind == "action")
                {
                    ActionNode aNode = (ActionNode)(allNodes[i]);
                    actionNodes.Add(aNode);
                    continue;
                }
            }

            return actionNodes;
        }

        public void buildAffectations(Dictionary<String, ValueSpecification> parameters, InstanceSpecification host)
        {
            List<string> missing = new List<string>();

            /* Affectation de self */

            affectations.Add("self", host);
           // MascaretApplication.Instance.logfile.WriteLine("Build Affectations"); MascaretApplication.Instance.logfile.Flush();

            /* Affectation des parametres */
            activityParams.Clear();
            List<ActivityNode> nodes = procedure.Activity.Nodes;
            for (int iNode = 0; iNode < nodes.Count; iNode++)
            {
                ActivityNode node = nodes[iNode];
                if (node.Kind == "parameter")
                {
                    Parameter p = ((ActivityParameterNode)(node)).Parameter;
                    string strVal = parameters[p.name].getStringFromValue();
                    activityParams.Add(p.name, p.Type.createValueFromString(strVal));
                }
            }

            /* Affectation des ressources */
            List<ActivityPartition> partitions = procedure.Activity.Partitions;
            for (int i = 0; i < partitions.Count; i++)
            {
                if (partitions[i].name == "this")
                {
                    affectations.Add("this", host);
                }
                else
                {
                    string pname = partitions[i].name;
                }

                nodes = partitions[i].Node;
                for (int iNode = 0; iNode < nodes.Count; iNode++)
                {
                    ActivityNode node = nodes[iNode];
                    if (node.Kind == "object")
                    {
                        buildAffectationsObject(ref missing, node);
                    }
                    else
                    {
                        if (node.Kind == "action")
                        {
                            buildAffectationsAction(ref missing, node);
                        }
                    }
                }
            }

            if (missing.Count > 0)
            {
                buildAffectationsPostbone(missing);
            }

            //RQ PARSER		procedure.Activity.setContext(buildOclContext(activityParams));
        }

        public void stop()
        {
	        List<ActionNode> actions = getAllActionsFor(agent.Aid);
	        for (int iA = 0; iA < actions.Count; iA++)
	        {
		        actions[iA].stop();
	        }
        }

        public List<ActionNode> getAllActionsFor(AID agent)
        {
	        List<ActionNode> allActionNodes = getAllActions();
	        List<ActionNode> nodesForAgent = new List<ActionNode>();
	
	        for (int i=0; i<allActionNodes.Count; ++i)
	        {
		        if(allActionNodes[i].Partitions[0].name == agentToPartition[agent.toString()].name)
		        {
			        nodesForAgent.Add(allActionNodes[i]);
		        }
	        }

	        return nodesForAgent;
        }

        public ActionNode getActionByNameFor(AID agent, string action)
        {
            //assuming only one action exists by that name !

            List<ActionNode> allActionNodes = getAllActions();

            ActionNode found = null;

            for (int i = 0; i < allActionNodes.Count; ++i)
            {
                //agentToPartition.getMap(agent.toString()).name;

                if ((allActionNodes[i].Partitions[0].name == agentToPartition[agent.toString()].name) && (action == clean(allActionNodes[i].name)))
                {
                    found = allActionNodes[i];
                    break;
                }
            }

            return found;
        }

        public List<ActionNode> getActionToExecute()
        {
            return getActionToExecuteFor(agent.Aid);
        }

        public List<ActionNode> getActionToExecuteFor(AID agent)
        {
            List<ActionNode> toExec = new List<ActionNode>();

            //	Debug.Log(" GET ACTIONS TO DO");

            List<ActionNode> allActionsToExecute = getActionToExecuteForAll();
            for (int i = 0; i < allActionsToExecute.Count; ++i)
            {
                //check author
                if (allActionsToExecute[i].Partitions[0].name == agentToPartition[agent.toString()].name)
                {
                    toExec.Add(allActionsToExecute[i]);
                }
            }


            return toExec;
        }

        public List<ActionNode> getActionToExecuteForAll()
        {
            //this function simulates token advancement to provide the list of actions that can be executed
            //an action can be executed if it makes a difference in the procedure (if it enables a token to advance)

            //prepare output vector
            List<ActionNode> toExec = new List<ActionNode>();
            // StreamWriter file = MascaretApplication.Instance.logfile;

            //build list of all reachable actions (this list is a super-set which includes the actions to execute). we will filter this list
            List<ActionNode> allPossibleActionsToExecute = getAllActions();

            //backup current activeTokens
            List<ActivityExecutionToken> activeTokensBackup = new List<ActivityExecutionToken>();
            for (int i = 0; i < activeTokens.Count; i++)
                activeTokensBackup.Add(activeTokens[i]);


            //now filter the list by creating dummy tokens and simulate their advancement
            for (int i=0; i<allPossibleActionsToExecute.Count; ++i)
            {
                //create dummy list
                activeTokens.Clear();

                for (int j=0; j<activeTokensBackup.Count; ++j)
                {
                    //copy the token into a dummy
                    ActivityExecutionToken dummyToken = new ActivityExecutionToken();
                    dummyToken.currentLocation = activeTokensBackup[j].currentLocation;
                    dummyToken.forkStack = activeTokensBackup[j].forkStack;
                    dummyToken.loopNodeStack = activeTokensBackup[j].loopNodeStack;
                    dummyToken.outgoingEdge = activeTokensBackup[j].outgoingEdge;
		
                    activeTokens.Add(dummyToken);
                }

                //test to see if this action is able to advance a token
                //file.WriteLine(" NB ACTIVE TOKENS : " + activeTokens.Count); file.Flush();

                for (int j = 0; j < activeTokens.Count; ++j)
                {
                    // file.WriteLine(activeTokens[j].currentLocation.name); file.Flush();
                    //advance token without the action (just to be sure we don't get fooled into thinking it advanced because of the action when it advances because of some other conditions that came true)
                    //if (ACTIVATE_DEBUG_CONSOLE_OUTPUT_TOKENS) cerr << "advancing with dummy action" << endl;
                    ActionNode dummyActionNode = null;
                    tryToAdvanceToken(activeTokens[j], dummyActionNode, true); //care not about the result //TODO .... check this out

                    //if (ACTIVATE_DEBUG_CONSOLE_OUTPUT_TOKENS) cerr << "advancing dummy token" << endl;

                    //see if the dummy advances with the current action
                    ActivityExecutionToken token = activeTokens[j];
                    //Debug.Log("CURRENT ACTIVE TOKEN : " + token.currentLocation.name);
                    if (tryToAdvanceToken(token, allPossibleActionsToExecute[i], true))
                    {
                        //dummy token advanced, so this action is an action to execute
                        activeTokens[j] = token;

                        toExec.Add(allPossibleActionsToExecute[i]);
                        //Debug.Log("New Current Location : " + activeTokens[j].currentLocation.name);
                        //if (ACTIVATE_GENERAL_DEBUG) cerr << "allPossibleActionsToExecute[" << i << "] = " << allPossibleActionsToExecute[i]->getName() << endl;
                        //break;
                    }
                }
            }

            //reinstate backed up activeTokens
            activeTokens.Clear();
            for (int i = 0; i < activeTokensBackup.Count; i ++ )
                activeTokens.Add(activeTokensBackup[i]);

            //remove already running nodes :: TODO : check if we should allow this or not....
            // file.WriteLine("Action Running : " + actionsRunning.Count); file.Flush();
            for (int i = 0; i < actionsRunning.Count; ++i)
            {
                for (int j = 0; j < toExec.Count; ++j)
                {
                    if (actionsRunning[i].Value == toExec[j])
                    {
                        toExec.RemoveAt(j);
                        j--;
                    }
                }
            }

            return toExec;
        }

        protected bool actionIsCurrentlyRunning(ActionNode action)
        {
            //remove already running nodes :: TODO : check if we should allow this or not....
            for (int i = 0; i < actionsRunning.Count; ++i)
            {
                if (actionsRunning[i].Value == action)
                {
                    return true;
                }
            }

            return false;
        }

        List<string> getAllActionsDoneBy(AID agent)
        {
	        // TODO: Maybe retyrb ActionNode and not name ?
	        List<string> actions = new List<string>();
	        List<ActionNode> nodes = allActionsDone[agent.toString()];
	        for(int i = 0; i < nodes.Count; i++)
	        {
		        actions.Add(clean(nodes[i].name));
	        }
	        return actions;
        }

        List<TimeExpression> getAllActionsDoneTimestampsBy(AID agent)
        {
	        return allActionsDoneTimestamps[agent.toString()];
        }

        public List<AID> getOtherAgents()
        {
            List<AID> others = new List<AID>();
            //map<shared_ptr<ActivityPartition>, shared_ptr<AID> >::iterator it=_partitionToAgent.begin();

            foreach (KeyValuePair<ActivityPartition, AID> it in partitionToAgent)
            {
                if (it.Value.toString() != agent.Aid.toString())
                {
                    others.Add(it.Value);
                }
            }

            return others;
        }

        public bool isFinished()
        {
            //  StreamWriter file = MascaretApplication.Instance.logfile;
            bool reachableNodesStillExist = false;
            for (int i = 0; i < activeTokens.Count; ++i)
            {
                if ((activeTokens[i].outgoingEdge != null) || (activeTokens[i].loopNodeStack.Count > 0))
                {
                    reachableNodesStillExist = true;
                    break;
                }
            }

            return (!reachableNodesStillExist) && actionsRunning.Count == 0;
        }

        public void informActionRunning(AID agent, string actionName)
        {
            ActionNode action = getActionByNameFor(agent, actionName);
            informActionRunning(agent, action);
        }

        public void informActionRunning(AID agent, ActionNode action)
        {

            //check if procedure will advance if this action is made
            bool canAdvance = true;

            ActionNode actionToRun = null;
            // Test if action owned by agent
            if (action.Partitions[0].name == agentToPartition[agent.toString()].name)
                actionToRun = action;
            //		else
            //			cerr << "This action " << action->getFullName() << " doesn't belong to agent " << agent->getName() << endl;
             if (actionToRun != null)
             {
                 //check if procedure can advance (action is reachable from the previous done actions)
		
                 List<ActionNode> possibleNextActions = getActionToExecuteFor(agent);
			
                 for (int i=0; i<possibleNextActions.Count; ++i)
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
                actionsRunning.Add(new KeyValuePair<string, ActionNode>(agent.toString(), action));

                //TODO: appologise because the hack is not so beautiful. the good part is that it works fine :)
                //update next reachable nodes list, if their current location is not a dead end (this caused problems with loopnodes for example...)
                //-- the problem was that loopnodes must not be restarted/skipped until the effect of its actions is understood. this is not the case with decision nodes for example
                ActionNode doneAction = action;

                if (doneAction != null)
                {
                    bool advancedToken = false;
                    for (int i = 0; i < activeTokens.Count; ++i)
                    {
                        if ((activeTokens[i].outgoingEdge == null) ||
                                (activeTokens[i].outgoingEdge.Target.getOutgoingControlFlowEdges().Count == 0))
                        {
                            continue; //skip deadends
                        }
                        //try to advance
                        //if (ACTIVATE_DEBUG_CONSOLE_OUTPUT_TOKENS) cerr << "> trying to advance token " << (i+1) << "/" << _activeTokens.size() << " from " << _activeTokens[i]->currentLocation->getName() << endl;

                        ActivityExecutionToken token = activeTokens[i];
                        if (tryToAdvanceToken(token, doneAction, true))
                        {
                            //if (ACTIVATE_DEBUG_CONSOLE_OUTPUT_TOKENS) cerr << "   >>  advanced token " << (i+1) << "/" << _activeTokens.size() << " to " << _activeTokens[i]->currentLocation->getName() << endl;
					
                            activeTokens[i] = token;
                            advancedToken = true;
                        }

                    }

                    if (advancedToken)
                    {

                        //add to "done" list
                        actionsDone.Add(doneAction);
                        if (!allActionsDone.ContainsKey(agent.toString())) 
                            allActionsDone.Add(agent.toString(), new List<ActionNode>());

                        allActionsDone[agent.toString()].Add(doneAction);

                        if (!allActionsDoneTimestamps.ContainsKey(agent.toString()))
                            allActionsDoneTimestamps.Add(agent.toString(),new List<TimeExpression>());
                        allActionsDoneTimestamps[agent.toString()].Add( doneAction.CurrentExecution.Finish);
                        
                    }
                    else
                    {
                        //if (ACTIVATE_GENERAL_DEBUG) cerr << "HUSTON, WE'VE GOT A PROBLEM... no token was advanced for " << doneAction->getName() << endl;
                    }
                }
            }
        }

        public void informActionDone(AID agent, string actionName)
        {
            ActionNode action = getActionByNameFor(agent, actionName);
            informActionDone(agent, action);
        }

        public void informActionDone(AID agent, ActionNode action)
        {
            bool foundAndRemoved = false;
            //remove from "running" list
            for (int i = 0; i < actionsRunning.Count; ++i)
            {
                if ((actionsRunning[i].Key == agent.toString()) && (actionsRunning[i].Value.getFullName() == action.getFullName()))
                {
                    actionsRunning.RemoveAt(i);
                    foundAndRemoved = true;
                    break;
                }
            }

            //Debug.Log(" IAD : " + activeTokens.Count); 
            if (foundAndRemoved)
            {
                ActionNode doneAction = action;
                if (doneAction != null)
                {
                    bool advancedToken = false;

                    for (int i = 0; i < activeTokens.Count; i++)
                    {
                        ActivityExecutionToken token = activeTokens[i];
                       	if (tryToAdvanceToken(token, doneAction, true))
				        {
					        advancedToken = true;
				        }
                    }

                    if (advancedToken)
                    {
                        
					    //add to "done" list
					    actionsDone.Add(doneAction);
						allActionsDone[agent.toString()].Add(doneAction);
						allActionsDoneTimestamps[agent.toString()].Add(doneAction.CurrentExecution.Finish);
                    }
                    else
                    {
                        //if (ACTIVATE_GENERAL_DEBUG) cerr << "HUSTON, WE'VE GOT A PROBLEM... no token was advanced for " << doneAction->getName() << endl;
                    }
                }
            }
        }



        protected bool tryToAdvanceToken(ActivityExecutionToken token, ActionNode doneAction, bool gotTicketToUseAction)
        {
            //TODO : current implementation will fail if the loop node has branches which are not joined together at the end

            //TODO : setup part of loop nodes not implemented
            //TODO : careful if the loop node contains only an initial node :: ENDLESS LOOP

            //		Debug.Log(" current token : " + token.currentLocation.name);

            if (token.currentLocation.Kind == "action")
            {
                ActionNode aNode = (ActionNode)(token.currentLocation);
                if (actionIsCurrentlyRunning(aNode))
                {
                    System.Console.WriteLine(" ACTION is RUNNING : " + aNode.Action.name);
                    //token busy...
                    return false;
                }
            }


            if (token.outgoingEdge == null)
            {
                //if (ACTIVATE_DEBUG_CONSOLE_OUTPUT_TOKENS) cerr << "dbg Part1: edge null" << endl;
                //Debug.Log(" NO OUTGOING EDGES....");
                //my work here is done *flies away*
                return false;
            }

            //evaluate guard(s). Normal behavior for fork, but I treat decision nodes here, if there are more paths going out, I will take the first one.
            if (token.currentLocation.Kind == "fork")
            {
                //if (ACTIVATE_DEBUG_CONSOLE_OUTPUT_TOKENS) cerr << "dbg Part1: fork" << endl;

                //normal behavior for fork (should not have guard, but hell... it doesn't hurt to check)
                if (token.outgoingEdge.Guard != null && (!(token.outgoingEdge.Guard.eval(procedure.Activity.Context))))
                {
                    return false; //eval failed
                }
            }
            else
            {
                //if (ACTIVATE_DEBUG_CONSOLE_OUTPUT_TOKENS) cerr << "dbg Part1: no fork" << endl;

                List<ActivityEdge> outgoingNormal = token.currentLocation.getOutgoingControlFlowEdges();
                bool foundWayOut = false;
                for (int i = 0; i < outgoingNormal.Count; ++i)
                {
                    //if <no guard> or <guard but evaluates ok>
                    if ((outgoingNormal[i].Guard == null) || (outgoingNormal[i].Guard != null && (outgoingNormal[i].Guard.eval(procedure.Activity.Context))))
                    {
                        foundWayOut = true;
                        token.outgoingEdge = outgoingNormal[i]; //set new direction, we take the first possible one, remember ?
                    }
                }

                if (!foundWayOut)
                {
                    //if (ACTIVATE_DEBUG_CONSOLE_OUTPUT_TOKENS) cerr << "dbg Part1: didn't find way out" << endl;

                    return false; //no way out... leave for later
                }
            }

            ActivityNode nextLocation = token.outgoingEdge.Target;

            //test allowed action
            if (nextLocation.Kind == "action")
            {
                if ((nextLocation != doneAction) || (!gotTicketToUseAction))
                {
                    //not allowed to advance to this action
			
                    //if (ACTIVATE_DEBUG_CONSOLE_OUTPUT_TOKENS) cerr << "dbg Part1: action not allowed" << endl;
			
                    return false;
                }
		
                gotTicketToUseAction = false;
            }


            //Debug.Log("Next Location : " + nextLocation);

            //if (ACTIVATE_DEBUG_CONSOLE_OUTPUT_TOKENS) cerr << "adv tok from '" << token->currentLocation->getName() << "' to '" << nextLocation->getName() << "'" << endl;

            //Advance
            ActivityEdge dummyActivityEdge = null;
            token.currentLocation = nextLocation;
            token.outgoingEdge = dummyActivityEdge;

            List<ActivityEdge> outgoingAfterAdvance = token.currentLocation.getOutgoingControlFlowEdges();

            List<ActivityExecutionToken> nextTokens = new List<ActivityExecutionToken>();

            if (token.currentLocation.Kind == "fork")
            {
                //time to split token into pieces, in order to follow all fork branches

                token.forkStack.Add(token.currentLocation); //push this fork to the stack

                //remove this token from the active list
                for (int i = 0; i < activeTokens.Count; ++i)
                {
                    if (activeTokens[i].ToString() == token.ToString())
                    {
                        activeTokens.RemoveAt(i);
                        break;
                    }
                }

                for (int i = 0; i < outgoingAfterAdvance.Count; ++i)
                {
                    ActivityExecutionToken tok = new ActivityExecutionToken();
                    tok.currentLocation = token.currentLocation;
                    tok.forkStack = token.forkStack;
                    tok.loopNodeStack = token.loopNodeStack;
                    tok.outgoingEdge = outgoingAfterAdvance[i]; //set outgoing, to know which branch to follow

                    //push to active list
                    activeTokens.Add(tok);

                    //save for further use
                    nextTokens.Add(tok);
                }
            }
            else if (token.currentLocation.Kind == "join")
	        {
		        //we can only pass by a join, if all other branched tokens have arrived in this join
		        //the point is that the number of tokens which have the same last fork as this one, should be the same as the nr of incoming links to this join
		        //i hope i understand this next time i read it
	
		        List<ActivityEdge> incoming = token.currentLocation.Incoming; //so get the incoming
	
		        //see which is the last fork of this token
		        ActivityNode lastFork = null;
		        if (token.forkStack.Count > 0)
		        {
			        lastFork = token.forkStack[token.forkStack.Count-1];
		        }
        
            	if (lastFork != null)
		        {
			        //find all tokens that have this fork and see if they all finished their branch // TODO: remember that actually, the forkStacks should be identical for paired tokens... if they're not, we've got a problem....
                    List<ActivityExecutionToken> finishedTokens = new List<ActivityExecutionToken>() ;
			        for (int i=0; i<activeTokens.Count; ++i)
			        {
				        //if found a token with this last fork
				        if ((activeTokens[i].forkStack.Count > 0) && (activeTokens[i].forkStack[activeTokens[i].forkStack.Count-1] == lastFork)) 
				        {
					        //check to see if it's finished (arrived at the same join)
					        if (activeTokens[i].currentLocation == token.currentLocation)
					        {
						        //add to finished tokens list
						        finishedTokens.Add(activeTokens[i]);
					        }
				        }
			        }
		
			        //now check to see if the number of found tokens equals the number of incoming links to this join
			        if (finishedTokens.Count == incoming.Count)
			        {				
				        //then all should be great.... we can merge all tokens into a single one
				        for (int i=0; i<finishedTokens.Count; ++i)
				        {
					        //skip the current token, this one will remain
					        if (finishedTokens[i].ToString() == token.ToString()) continue;
				
					        //seek and destroy
					        for (int j=0; j<activeTokens.Count; ++j)
					        {
						        if (finishedTokens[i].ToString() == activeTokens[j].ToString())
						        {
							        //destroy
							        activeTokens.RemoveAt( j);
						        }
					        }
				        }
				
				        if (outgoingAfterAdvance.Count == 1)
				        {
					        token.outgoingEdge = outgoingAfterAdvance[0];
                            token.forkStack.RemoveAt(token.forkStack.Count - 1);//pop last fork
					        nextTokens.Add(token);
				        }
				        else if (outgoingAfterAdvance.Count > 1)
				        {
				        }
                    }
                }
                else
                {
                }
            }
        	else if (token.currentLocation.Kind == "loop")
	        {
		        LoopNode loopNode = (LoopNode)(token.currentLocation);
		
		        bool skipThisLoopNode = false;
		
		        if (loopNode.TestedFirst)
		        {
			        if (loopNode.eval()) //thou shall pass
			        {
				        skipThisLoopNode = true;
			        }
		        }
		
		        ActivityNode initLoop = loopNode.Initial;
		        List<ActivityEdge> outgoingFromInitLoop = null;
		
		        if (initLoop != null)
		        {
			        outgoingFromInitLoop = initLoop.getOutgoingControlFlowEdges();
		        }
		
		        if ( skipThisLoopNode || (initLoop == null) || (outgoingFromInitLoop.Count < 1) )
		        {
			        //I don't care which path to take, so I will just take the first one. In the advance part (see above) if it's a decision node, it will take the first possible path, if any
			        if (outgoingAfterAdvance.Count >= 1)
			        {
				        token.outgoingEdge = outgoingAfterAdvance[0];
				        nextTokens.Add(token);
			        }
			
		        }
		        else
		        {
			        //enter the loop node, we'll get the edge which exits the loopnode's initial node
			
			        token.currentLocation = initLoop;
			        token.outgoingEdge = outgoingFromInitLoop[0];
			        token.loopNodeStack.Add(loopNode);
		        }
	        }
            else
            {
                //I don't care which path to take, so I will just take the first one. In the advance part (see above) if it's a decision node, it will take the first possible path, if any
                if (outgoingAfterAdvance.Count >= 1)
                {
                    token.outgoingEdge = outgoingAfterAdvance[0];
                    nextTokens.Add(token);
                }
                else if (token.loopNodeStack.Count != 0) //check if we're in a loop node
                {
                    //if (ACTIVATE_DEBUG_CONSOLE_OUTPUT_TOKENS) cerr << "dbg Part1: edge null, but in loop node" << endl;

                    while (((token.outgoingEdge == null)) && (token.loopNodeStack.Count != 0)) //while we are still in a loopnode and it is finished
                    {
                        LoopNode lastLoop = token.loopNodeStack[token.loopNodeStack.Count - 1];
                        if (lastLoop.eval()) //test exit condition
                        {
                            //exiting loop node

                            List<ActivityEdge> outgoingFromLoop = lastLoop.getOutgoingControlFlowEdges();

                            if (outgoingFromLoop.Count != 0)
                            {
                                token.currentLocation = lastLoop;
                                token.outgoingEdge = outgoingFromLoop[0];
                            }

                            token.loopNodeStack.RemoveAt(token.loopNodeStack.Count -1); //remove last loop from stack (exit)
                        }
                        else
                        {
                            //restarting loop node

                            ActivityNode initLoop = lastLoop.Initial;
                            List<ActivityEdge> outgoingFromInitLoop = initLoop.getOutgoingControlFlowEdges();

                            token.currentLocation = initLoop;
                            token.outgoingEdge = outgoingFromInitLoop[0]; //it should exist, given the fact that we've entered this loop node already
                        }
                    }

                    if (token.outgoingEdge != null)
                    {
                        nextTokens.Add(token);
                    }
                }
            }
	
	
	        //continue to advance
	        for (int i=0; i<nextTokens.Count; ++i)
	        {
		        tryToAdvanceToken(nextTokens[i], doneAction, gotTicketToUseAction);
	        }
	
	        return true;
        }


   

        string clean(string toClean)
        {
            string returned = toClean;
            returned.Replace("/", "");
            returned.Replace("(", "");
            returned.Replace(")", "");
            returned.Replace("^", "");

            return returned;
        }

        public Dictionary<string, InstanceSpecification> getAffectations() 
        {
            return affectations;
        }

    }
}
