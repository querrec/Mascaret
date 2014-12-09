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

        protected Dictionary<String, ActivityPartition> agentToPartition;
        protected Dictionary<ActivityPartition, AID> partitionToAgent;
        protected List<ActivityExecutionToken> activeTokens;
        protected Dictionary<String, InstanceSpecification> affectations;
        protected Dictionary<String, ValueSpecification> activityParams;
        protected List<KeyValuePair<string, ActionNode>> actionsRunning;
        protected List<ActionNode> actionsDone;
        protected Dictionary<String, ActionNode> allActionsDone; // key: AID
        protected Dictionary<String, TimeExpression> allActionsDoneTimestamps;

        StreamWriter file = MascaretApplication.Instance.logfile;


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
            allActionsDone = new Dictionary<string, ActionNode>(); // key: AID
            allActionsDoneTimestamps = new Dictionary<string, TimeExpression>();

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
            initialToken.loopNodeStack = new List<LoopNode>();
            initialToken.forkStack = new List<ActivityNode>();
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

                //ActionNode dummyActionVariable = null;
                //tryToAdvanceToken(initialToken, dummyActionVariable,true); //first advancement
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

        public void buildAffectations(Dictionary<String, ValueSpecification> parameters, InstanceSpecification host)
        {
            List<string> missing = new List<string>();

            /* Affectation de self */

            affectations.Add("self", host);
            MascaretApplication.Instance.logfile.WriteLine("Build Affectations"); MascaretApplication.Instance.logfile.Flush();

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
                /*else
                {
                    string pname = partitions[i].name;
                }*/

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
                            buildAffectationsAction(missing, node);
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

        protected bool tryToAdvanceToken(ref ActivityExecutionToken token, ActionNode doneAction, bool gotTicketToUseAction)
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
            /*if (nextLocation.Kind == "action")
            {
                if ((nextLocation != doneAction) || (!gotTicketToUseAction))
                {
                    //not allowed to advance to this action
			
                    //if (ACTIVATE_DEBUG_CONSOLE_OUTPUT_TOKENS) cerr << "dbg Part1: action not allowed" << endl;
			
                    return false;
                }
		
                gotTicketToUseAction = false;
            }*/


            //Debug.Log("Next Location : " + nextLocation);

            //if (ACTIVATE_DEBUG_CONSOLE_OUTPUT_TOKENS) cerr << "adv tok from '" << token->currentLocation->getName() << "' to '" << nextLocation->getName() << "'" << endl;

            //Advance
            ActivityEdge dummyActivityEdge = null;
            token.currentLocation = nextLocation;
            token.outgoingEdge = dummyActivityEdge;

            if (token.currentLocation.Kind != "action") return false;

            return true;
        }

        protected void buildAffectationsObject(ref List<string> missing, ActivityNode node)
        {
            string pname = node.name;

            MascaretApplication.Instance.logfile.WriteLine("TRY TO AFFECT OBJECT : " + pname); MascaretApplication.Instance.logfile.Flush();
            RessourceAssignement ra = organisation.findRessourceAssignement(pname);
            if (ra != null && ra.Entity != null)
            {
                affectations.Add(pname, ra.Entity);
                MascaretApplication.Instance.logfile.WriteLine("SUCCESS TO AFFECT OBJECT : " + pname); MascaretApplication.Instance.logfile.Flush();
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
                    MascaretApplication.Instance.logfile.WriteLine("FAIL TO AFFECT OBJECT : " + pname); MascaretApplication.Instance.logfile.Flush();
                    missing.Add(pname);
                }
            }
        }

        protected void buildAffectationsAction(List<string> missing, ActivityNode node)
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

        protected void buildAffectationsPostbone(List<string> missing)
        {
            findRessources(missing);
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
            StreamWriter file = MascaretApplication.Instance.logfile;

            //build list of all reachable actions (this list is a super-set which includes the actions to execute). we will filter this list
            //List<ActionNode> allPossibleActionsToExecute = getAllActions();

            //backup current activeTokens
            /*List<ActivityExecutionToken> activeTokensBackup = new List<ActivityExecutionToken>();
            foreach(ActivityExecutionToken token in activeTokens)
            {
                activeTokensBackup.Add(token);
            }*/

            //now filter the list by creating dummy tokens and simulate their advancement
            //for (int i=0; i<allPossibleActionsToExecute.Count; ++i)
            {
                //create dummy list
                /*activeTokens.Clear();
                for (int j=0; j<activeTokensBackup.Count; ++j)
                {
                    //copy the token into a dummy
                    ActivityExecutionToken dummyToken = new ActivityExecutionToken();
                    dummyToken.forkStack = new List<ActivityNode>();
                    dummyToken.loopNodeStack = new List<LoopNode>();
                    dummyToken.currentLocation = activeTokensBackup[j].currentLocation;
                    dummyToken.forkStack = activeTokensBackup[j].forkStack;
                    dummyToken.loopNodeStack = activeTokensBackup[j].loopNodeStack;
                    dummyToken.outgoingEdge = activeTokensBackup[j].outgoingEdge;
		
                    activeTokens.Add(dummyToken);
                }*/

                //test to see if this action is able to advance a token
                //file.WriteLine(" NB ACTIVE TOKENS : " + activeTokens.Count); file.Flush();

                for (int j = 0; j < activeTokens.Count; ++j)
                {
                   // file.WriteLine(activeTokens[j].currentLocation.name); file.Flush();
                    //advance token without the action (just to be sure we don't get fooled into thinking it advanced because of the action when it advances because of some other conditions that came true)
                    //if (ACTIVATE_DEBUG_CONSOLE_OUTPUT_TOKENS) cerr << "advancing with dummy action" << endl;
                    //RQ ActionNode dummyActionNode = null;
                    //RQ tryToAdvanceToken(activeTokens[j], dummyActionNode,true); //care not about the result //TODO .... check this out

                    //if (ACTIVATE_DEBUG_CONSOLE_OUTPUT_TOKENS) cerr << "advancing dummy token" << endl;

                    //see if the dummy advances with the current action
                    ActivityExecutionToken token = activeTokens[j];
                    //Debug.Log("CURRENT ACTIVE TOKEN : " + token.currentLocation.name);
                    if (tryToAdvanceToken(ref token, null, true))
                    {
                        //dummy token advanced, so this action is an action to execute
                        toExec.Add((ActionNode)(token.currentLocation));
                        activeTokens[j] = token;
                        //Debug.Log("New Current Location : " + activeTokens[j].currentLocation.name);
                        //if (ACTIVATE_GENERAL_DEBUG) cerr << "allPossibleActionsToExecute[" << i << "] = " << allPossibleActionsToExecute[i]->getName() << endl;
                        //break;
                    }
                }
            }

            //reinstate backed up activeTokens
            /*activeTokens.Clear();
            foreach(ActivityExecutionToken token in activeTokensBackup)
            {
                activeTokens.Add(token);
            }*/

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

        public Dictionary<String, InstanceSpecification> getAffectations()
        {
            return this.affectations;
        }

        public void informActionRunning(AID agent, string actionName)
        {
            ActionNode action = getActionByNameFor(agent, actionName);
            informActionRunning(agent, action);
        }

        public void informActionRunning(AID agent, ActionNode action)
        {
            //Debug.Log("ProcedureExecution::informActionRunning ( " + agent.toString()+ " , " + action.name + " ) on " + agent.name);

            //check if procedure will advance if this action is made
            bool canAdvance = true;

            ActionNode actionToRun = null;
            // Test if action owned by agent
            if (action.Partitions[0].name == agentToPartition[agent.toString()].name)
                actionToRun = action;
            //		else
            //			cerr << "This action " << action->getFullName() << " doesn't belong to agent " << agent->getName() << endl;
            /*
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
                }*/

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

                        /*ActivityExecutionToken token = activeTokens[i];
                        if (tryToAdvanceToken(ref token, doneAction, true))
                        {
                            //if (ACTIVATE_DEBUG_CONSOLE_OUTPUT_TOKENS) cerr << "   >>  advanced token " << (i+1) << "/" << _activeTokens.size() << " to " << _activeTokens[i]->currentLocation->getName() << endl;
					
                            activeTokens[i] = token;
                            advancedToken = true;
                        }
                        */
                    }

                    if (advancedToken)
                    {
                        //add to "done" list
                        actionsDone.Add(doneAction);

                        if (allActionsDone.ContainsKey(agent.toString()))
                            allActionsDone.Add(agent.toString(), doneAction);
                        else
                        {
                            //allActionsDone.Add(agent.toString(),new List<ActionNode>());
                            allActionsDone.Add(agent.toString(), doneAction);

                        }

                        if (allActionsDoneTimestamps.ContainsKey(agent.toString()))
                            allActionsDoneTimestamps.Add(agent.toString(), doneAction.CurrentExecution.Finish);
                        else
                        {
                            //allActionsDoneTimestamps.Add(agent.toString(), new List<TimeExpression>());
                            allActionsDoneTimestamps.Add(agent.toString(), doneAction.CurrentExecution.Finish);
                        }
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
		    for (int i=0; i<actionsRunning.Count; ++i)
		    {
			    //Debug.Log(actionsRunning[i].Key + " == " + agent.toString() + " / " + actionsRunning[i].Value + " == " + action);
			    if ((actionsRunning[i].Key == agent.toString()) && (actionsRunning[i].Value == action))
			    {
				    actionsRunning.RemoveAt(i);
				    foundAndRemoved = true;
				    break;
			    }
		    }
		
		    //Debug.Log(" IAD : " + activeTokens.Count); 
		
		    for (int i=0; i<activeTokens.Count; i++)
		    {
			    ActivityEdge dummyActivityEdge = null;
			    ActivityExecutionToken token = activeTokens[i];
			    List<ActivityEdge> outGoingEdge = token.currentLocation.getOutgoingControlFlowEdges();
			    if (outGoingEdge.Count != 0)
				    token.outgoingEdge = token.currentLocation.getOutgoingControlFlowEdges()[0];
			    else
				    token.outgoingEdge = dummyActivityEdge;
			    activeTokens[i] = token;
		    }
			
		    // RQ Add :
		    if (foundAndRemoved)
		    {
			    //TODO: appologise because the hack is not so beautiful. the good part is that it works fine :)
			    //re-update next reachable nodes list, just in case we had a loopnode case... damn these loopnodes...
			    //-- the problem was that loopnodes must not be restarted/skipped until the effect of its actions is understood. this is not the case with decision nodes for example
			    ActionNode doneAction = action;
		
			    if (doneAction != null)
			    {
				    bool advancedToken = false;
				    
				
				    if (advancedToken)
				    {
                        /*
					    //add to "done" list
					    actionsDone.Add(doneAction);
					
					    if (allActionsDone.ContainsKey(agent.toString()))
						    allActionsDone[agent.toString()]=doneAction;
					    else 
					    {
						    allActionsDone[agent.toString ()] = new List<Mascaret.ActionNode>();
						    allActionsDone[agent.toString()] = doneAction;

					    }
					
					    if (allActionsDoneTimestamps.ContainsKey(agent.toString()))
						    allActionsDoneTimestamps[agent.toString()]=doneAction.CurrentExecution.Finish;
					    else
					    {
					    	allActionsDoneTimestamps.Add(agent.toString(), new List<TimeExpression>());
						    allActionsDoneTimestamps.getMap(agent.toString()).Add(doneAction.CurrentExecution.Finish);
					    }
                         */
				    }
				    else
				    {
				    	//if (ACTIVATE_GENERAL_DEBUG) cerr << "HUSTON, WE'VE GOT A PROBLEM... no token was advanced for " << doneAction->getName() << endl;
				    }
			    }
		    }
        }

        public bool isFinished()
        {
            StreamWriter file = MascaretApplication.Instance.logfile;
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
        public bool isFinished2()
        {
            StreamWriter file = MascaretApplication.Instance.logfile;
            bool reachableNodesStillExist = false;
         //   file.WriteLine("Active token : " + activeTokens.Count); file.Flush();
            for (int i = 0; i < activeTokens.Count; ++i)
            {
              //  file.WriteLine(activeTokens[i].currentLocation.getFullName()); file.Flush();
                if ((activeTokens[i].currentLocation.getOutgoingActionNode().Count > 0) )
                {
                   // file.WriteLine("Still reacheable"); file.Flush();
                    reachableNodesStillExist = true;
                    break;
                }
            }

            return !reachableNodesStillExist ;
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


        public List<ActionNode> getAllActions()
        {
            List<ActivityNode> allNodes = procedure.Activity.Nodes;
            List<ActionNode> actionNodes = new List<ActionNode>();

            for (int i = 0; i < allNodes.Count; ++i)
            {
                if (allNodes[i].Kind == "action")
                {
                    ActionNode aNode = (ActionNode)(allNodes[i]);
                    actionNodes.Add(aNode);
                }
            }

            return actionNodes;
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

        string clean(string toClean)
        {
            string returned = toClean;
            returned.Replace("/", "");
            returned.Replace("(", "");
            returned.Replace(")", "");
            returned.Replace("^", "");

            return returned;
        }

    }
}
