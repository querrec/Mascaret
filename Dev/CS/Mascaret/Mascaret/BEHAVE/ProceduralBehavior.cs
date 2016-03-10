using System;
using System.Collections.Generic;
using System.IO;

namespace Mascaret
{
    public class ProceduralBehavior : CyclicBehaviorExecution
    {
        public List<InstanceSpecification> _signals = new List<InstanceSpecification>();
 
        public bool ispause = false;
        public List<ProcedureExecution> runningProcedures = new List<ProcedureExecution>();
        public List<ActionNode> actionsToDo = new List<ActionNode>();
        public Dictionary<BehaviorExecution, ActionNode> behaviorToNode = new Dictionary<BehaviorExecution,ActionNode>();

       // StreamWriter file = MascaretApplication.Instance.logfile;


        public ProceduralBehavior(Behavior behavior, InstanceSpecification host, Dictionary<String, ValueSpecification> p)
            : base(behavior, host, p)
        {
        }

        public ProceduralBehavior()
        {
        }

        public void pushProcedureToDo(Procedure proc, OrganisationalEntity org, Role role, Dictionary<String, ValueSpecification> parameters)
        {
            Agent agt = (Agent)(this.Host);

            ProcedureExecution procInfo = new ProcedureExecution(proc, org, agt);
            procInfo.buildAffectations(parameters, this.Host);

            /* Search if the current agent have something to do in this procedure */
            List<ActivityPartition> parts = proc.Activity.Partitions;
            bool found = false;
            for (int iP = 0; iP < parts.Count; iP++)
            {
                if (parts[iP].name == role.name)
                {
                    //   file.WriteLine("[ProceduralBehavior Info] Found a role " + role.name + " in procedure " + proc.name); file.Flush();
                    found = true;
                }
            }

            runningProcedures.Add(procInfo);
            ispause = false;

            if (!found)
            {
                //inform all agents that this agent's role in this procedure has ended (used to unblock tokens in ProcedureExecution)
                sendProcedureDoneMessage(procInfo);
                runningProcedures.RemoveAt(runningProcedures.Count - 1); //remove it..
                // ispause = true;
            }
        }

        public override void action()
        {
            Agent agt = (Agent)(this.Host);

            if (ispause) return;
           // StreamWriter file = MascaretApplication.Instance.logfile;
            //file.WriteLine("NB PRocs : " + runningProcedures.Count); file.Flush();

            if (runningProcedures.Count > 0)
            {
                for (int iP = 0; iP < runningProcedures.Count; iP++)
                {
                   // file.WriteLine("ProceduralBehavior in procedure " + runningProcedures[iP].procedure.name); file.Flush();
                    ProcedureExecution procInfo = runningProcedures[iP];
                    List<ActionNode> actionNodes = new List<ActionNode>();

                    //remove those which are not agreed upon
                   /*if (agt.ControlledByHuman)
                    {
                        MascaretApplication.Instance.VRComponentFactory.Log("[ProceduralBehavior Info] Agent controlled ... ");
                        actionNodes = procInfo.getActionToExecute();
                        for (int atd = 0; atd < actionNodes.Count; ++atd)
                          MascaretApplication.Instance.VRComponentFactory.Log("ProceduralBehavior Info] -- Human has to do : " + actionNodes[atd].name);
                        actionNodes.Clear();

                        for (int atd = 0; atd < actionsToDo.Count; ++atd)
                        {
                            actionNodes.Add(actionsToDo[atd]);
                        }
                        actionsToDo.Clear();
                    }
                    else*/
                    {
                        actionNodes = procInfo.getActionToExecute();
                    }
                    if (actionNodes.Count > 0)
                    {

                        for (int i = 0; i < actionNodes.Count; i++)
                        {
                            //Dictionary<string, InstanceSpecification> affectations = procInfo.getAffectations();
                            //actionNodes[i].procInfo.procedure.Activity.Context
                            bool isCallBehavior = false;
                            if (actionNodes[i].Action.Kind == "CallBehavior") isCallBehavior = true;
                            if (!agt.ControlledByHuman)
                            {
                                actionNodes[i].start(agt, procInfo.getAffectations(), false); //TODO ///////////////////////////////////////// if not flag start, else start
                                sendActionRealisationMessage(actionNodes[i], procInfo); // TODO
                                procInfo.informActionRunning(agt.Aid, actionNodes[i]);

                                if (actionNodes[i].CurrentExecution != null)
                                {
                                    behaviorToNode.Add(actionNodes[i].CurrentExecution, actionNodes[i]);
                                    actionNodes[i].CurrentExecution.addCallbackOnBehaviorStop(onBehaviorStop);
                                }
                                else
                                {
                                    procInfo.informActionDone(agt.Aid, actionNodes[i]); //TODO
                                    sendActionDoneMessage(actionNodes[i], procInfo);
                                }
                            }
                            else
                            {
                                if (isCallBehavior)
                                {
                                    actionNodes[i].start(agt, procInfo.getAffectations(), false); //TODO ///////////////////////////////////////// if not flag start, else start
                                    sendActionRealisationMessage(actionNodes[i], procInfo); // TODO
                                    procInfo.informActionRunning(agt.Aid, actionNodes[i]);

                                    if (actionNodes[i].CurrentExecution != null)
                                    {
                                        behaviorToNode.Add(actionNodes[i].CurrentExecution, actionNodes[i]);
                                        actionNodes[i].CurrentExecution.addCallbackOnBehaviorStop(onBehaviorStop);
                                    }
                                    else
                                    {
                                        procInfo.informActionDone(agt.Aid, actionNodes[i]); //TODO
                                        sendActionDoneMessage(actionNodes[i], procInfo);
                                    }

                                }
                                else
                                {
                                    actionsToDo.Add(actionNodes[i]);
                                    MascaretApplication.Instance.VRComponentFactory.Log("ProceduralBehavior Info] -- Human has to do : " + actionNodes[i].name);
                                }
                            }
                        }
                    }
                    else if (agt.getMessageQueue().Count == 0)
                    {
                        //pause();
                        //ispause = true;
                    }

                    if (procInfo.isFinished())
                    {
                        //file.WriteLine("ProceduralBehavior Procedure : " + runningProcedures[iP].procedure.name + " finie "); file.Flush();
                        //inform all agents that this agent's role in this procedure has ended (used to unblock tokens in ProcedureExecution)
                        sendProcedureDoneMessage(procInfo);

                        runningProcedures[iP] = runningProcedures[runningProcedures.Count - 1];
                        runningProcedures.RemoveAt(runningProcedures.Count - 1);
                        //cerr << getHost()->getName()<<" : suppression de la procedure"<< endl;
                    }
                    else
                    {
                        //iP++;
                    }
                    
                }
            }
            else
            {
                ispause = true;
            }
        }

    public void stop(string procedureName)
    {
	    List<ProcedureExecution> exec = new List<ProcedureExecution>(); // Procedure still running
	    for (int iP = 0; iP < runningProcedures.Count; iP++)
	    {
		    ProcedureExecution procInfo = runningProcedures[iP];
		    if (procInfo.procedure.name == procedureName)
		    {
			    procInfo.stop();
		    }
		    else 
		    {
			exec.Add(procInfo);
		    }
	    }

	    // We put back still running procedure in the vector
	    runningProcedures.Clear();
	    for (int iP = 0; iP < exec.Count ; iP++)
		    runningProcedures.Add(exec[iP]);
    }

    public void stop(Procedure procedure)
    {
	    List<ProcedureExecution> exec = new List<ProcedureExecution>(); // Procedure still running
	    for (int iP = 0; iP < runningProcedures.Count; iP++)
	    {
		    ProcedureExecution procInfo = runningProcedures[iP];
		    if (procInfo.procedure == procedure)
		    {
			    procInfo.stop();
		    }
		    else
		    {
			    exec.Add(procInfo);
		    }
	    }

	    // We put back still running procedure in the vector
	    runningProcedures.Clear();
	    for (int iP = 0; iP < exec.Count ; iP++)
		    runningProcedures.Add(exec[iP]);
    }
        
    public void stopAll()
    {
	    for (int iP = 0; iP < runningProcedures.Count; iP++)
	    {
		    ProcedureExecution procInfo = runningProcedures[iP];
		    procInfo.stop();
	    }
	    runningProcedures.Clear();
    }
        
        public void onBehaviorStop(BehaviorExecution be)
        {
            for (int iP = 0; iP < runningProcedures.Count; iP++)
            {
                ProcedureExecution procInfo = runningProcedures[iP];
                procInfo.informActionDone(((Agent)(this.Host)).Aid, behaviorToNode[be]);
                sendActionDoneMessage(behaviorToNode[be], procInfo);
            }
            
            //restart();
            ispause = false;
        }

        public void onActionDone(AID agent, ActionNode action)
        {
	        for (int iP = 0; iP < runningProcedures.Count;iP++ )
	        {
		        ProcedureExecution procInfo = runningProcedures[iP];

		        procInfo.informActionDone(agent,action);
	        }
            ispause = false;
        }

        public void onActionDone(AID agent, string actionName)
        {
	        for (int iP = 0; iP < runningProcedures.Count;iP++ )
	        {
		        ProcedureExecution procInfo = runningProcedures[iP];
		
		        procInfo.informActionDone(agent,actionName);
	        }
        }

        public void onActionRunning(AID agent, ActionNode action)
        {
	        for (int iP = 0; iP < runningProcedures.Count;iP++ )
	        {
		        ProcedureExecution procInfo = runningProcedures[iP];
		
		        procInfo.informActionRunning(agent,action);
	        }
        }

        public void onActionRunning(AID agent, string actionName)
        {
	        for (int iP = 0; iP < runningProcedures.Count;iP++ )
	        {
		        ProcedureExecution procInfo = runningProcedures[iP];
		
		        procInfo.informActionRunning(agent,actionName);
	        }
        }

        public void sendActionRealisationMessage(ActionNode action, ProcedureExecution procInfo)
        {
            Agent agt = (Agent)(this.Host);

            ACLMessage procMsg = new ACLMessage(ACLPerformative.INFORM);

            //we inform at wich time the action start
            TimeExpression timestamp = action.CurrentExecution.Start;

            procMsg.Timestamp = timestamp;

            //set ACLMessage content
            string content = "((action ";
            content += agt.name;
            content += " ";
            content += "(" + clean(action.name) + ")";
            content += "))";
            procMsg.Content = content;

            //send message to other agents
            List<AID> agents = procInfo.getOtherAgents();

            for (int iA = 0; iA < agents.Count; iA++)
            {
                procMsg.Receivers.Add(agents[iA]);
            }
            agt.send(procMsg);
        }

        public void sendActionDoneMessage(ActionNode action, ProcedureExecution procInfo)
        {
            Agent agt = (Agent)(this.Host);

            ACLMessage procMsg = new ACLMessage(ACLPerformative.INFORM);

            //we inform at wich time the action finished
            TimeExpression timestamp = action.CurrentExecution.Finish;

            procMsg.Timestamp = timestamp;

            //set ACLMessage content
            string content = "((done (action ";
            content += agt.name;
            content += " ";
            content += "(" + clean(action.name) + ")";
            content += ")))";
            procMsg.Content = content;
            //MascaretApplication.Instance.VRComponentFactory.Log(content);

            //send message to other agents
            List<AID> agents = procInfo.getOtherAgents();

            for (int iA = 0; iA < agents.Count; iA++)
            {
                procMsg.Receivers.Add(agents[iA]);
            }
            agt.send(procMsg);
        }


        public void sendProcedureDoneMessage(ProcedureExecution procInfo)
        {
            Agent agt = (Agent)(this.Host);

            ACLMessage procMsg = new ACLMessage(ACLPerformative.INFORM);

            //we inform at wich time the procedure finish
            TimeExpression timestamp = BehaviorScheduler.Instance.getCurrentVirtualTime();
            procMsg.Timestamp = timestamp;

            string content = "((done (action ";
            content += agt.name;
            content += " ";
            content += "(" + clean(procInfo.procedure.name) + ")";
            content += ")))";
            procMsg.Content = content;

            procMsg.Receivers.Add(MascaretApplication.Instance.Agent.Aid);

            agt.send(procMsg);
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

