using System;
using System.Collections.Generic;
using System.IO;

namespace Mascaret
{
    public class ProceduralBehavior : CyclicBehaviorExecution
    {

        public bool ispause = false;
        StreamWriter file = MascaretApplication.Instance.logfile;


        public ProceduralBehavior(Behavior behavior, InstanceSpecification host, Dictionary<String, ValueSpecification> p)
            : base(behavior, host, p)
        {
         
            runningProcedures = new List<ProcedureExecution>();
            behaviorToNode = new Dictionary<BehaviorExecution, ActionNode>();
        }

        public override void action()
        {
            Agent agt = (Agent)(this.Host);

            if (ispause) return;
            StreamWriter file = MascaretApplication.Instance.logfile;
            //file.WriteLine("NB PRocs : " + runningProcedures.Count); file.Flush();


            if (runningProcedures.Count > 0)
            {
                for (int iP = 0; iP < runningProcedures.Count; iP++)
                {
                   // file.WriteLine("ProceduralBehavior in procedure " + runningProcedures[iP].procedure.name); file.Flush();

                    ProcedureExecution procInfo = runningProcedures[iP];
                    List<ActionNode> actionNodes = new List<ActionNode>();

                    //remove those which are not agreed upon
                  /*  if (agt.ControlledByHuman)
                    {
                        file.WriteLine("[ProceduralBehavior Info] Agent controlled ... "); file.Flush();
                        actionNodes = procInfo.getActionToExecute();
                        for (int atd = 0; atd < actionNodes.Count; ++atd)
                          file.WriteLine("ProceduralBehavior Info] -- Human has to do : " + actionNodes[atd].name); file.Flush();
                        actionNodes.Clear();

                        for (int atd = 0; atd < actionsToDo.Count; ++atd)
                        {
                            actionNodes.Add(actionsToDo[atd]);
                        }
                        actionsToDo.Clear();
                    }
                    else*/
                    {
                       // file.WriteLine("ProceduralBehavior getActionToExecute() ... "); file.Flush();
                        actionNodes = procInfo.getActionToExecute();
                        
                        //file.WriteLine("Nb actions : " + actionNodes.Count);
                        if (agt.ControlledByHuman)
                        {
                            for (int i = 0; i < actionNodes.Count; i++)
                            {
                                actionsToDo.Add(new KeyValuePair<ProcedureExecution,ActionNode> (procInfo,actionNodes[i]));
                            }
                        }
                    }

                    if (actionNodes.Count > 0)
                    {
   
                        for (int i = 0; i < actionNodes.Count; i++)
                        {
                            Dictionary<string, InstanceSpecification> affectations = procInfo.getAffectations();

                            if (!agt.ControlledByHuman)
                            {
                                file.WriteLine("ProceduralBehavior starting : " + actionNodes[i].Description); file.Flush();

                                string fml = actionNodes[i].Fml;
                                if (fml != "")
                                    agt.setIntention(fml);

                                actionNodes[i].start(agt, procInfo.getAffectations(), false); //TODO ///////////////////////////////////////// if not flag start, else start
                                sendActionRealisationMessage(actionNodes[i], procInfo); // TODO
                                procInfo.informActionRunning(agt.Aid, actionNodes[i]);


                                if (actionNodes[i].CurrentExecution != null)
                                {
                                    //cerr << "execution ok" << endl;
                                    behaviorToNode.Add(actionNodes[i].CurrentExecution, actionNodes[i]);

                                    actionNodes[i].CurrentExecution.addCallbackOnBehaviorStop(onBehaviorStop);
                                }
                                else
                                {
                                    //cerr << "execution ko --> ignore (= says realization ok)" << endl;
                                    procInfo.informActionDone(agt.Aid, actionNodes[i]); //TODO
                                    sendActionDoneMessage(actionNodes[i], procInfo);
                                }
                            }
                            else
                            {
                                file.WriteLine("Action Type : " + actionNodes[i].Action.GetType().ToString()); file.Flush();
                                if (actionNodes[i].Action.GetType().ToString() == "Mascaret.CallBehaviorAction")
                                {
                                    actionNodes[i].start(agt, procInfo.getAffectations(), false); //TODO ///////////////////////////////////////// if not flag start, else start
                                    sendActionRealisationMessage(actionNodes[i], procInfo); // TODO
                                    procInfo.informActionRunning(agt.Aid, actionNodes[i]);


                                    if (actionNodes[i].CurrentExecution != null)
                                    {
                                        //cerr << "execution ok" << endl;
                                        behaviorToNode.Add(actionNodes[i].CurrentExecution, actionNodes[i]);

                                        actionNodes[i].CurrentExecution.addCallbackOnBehaviorStop(onBehaviorStop);
                                    }
                                    else
                                    {
                                        //cerr << "execution ko --> ignore (= says realization ok)" << endl;
                                        procInfo.informActionDone(agt.Aid, actionNodes[i]); //TODO
                                     sendActionDoneMessage(actionNodes[i], procInfo);
                                    }
                                }
                            }
                        }
                        //ispause = true;
                    }
                    else if (agt.getMessageQueue().Count == 0)
                    {
                        //pause();
                        //ispause = true;
                    }

                    if (procInfo.isFinished() && ! agt.ControlledByHuman)
                    {
                        file.WriteLine("ProceduralBehavior Procedure : " + runningProcedures[iP].procedure.name + " finie "); file.Flush();
                        //inform all agents that this agent's role in this procedure has ended (used to unblock tokens in ProcedureExecution)
                        sendProcedureDoneMessage(procInfo);

                        runningProcedures[iP] = runningProcedures[runningProcedures.Count - 1];
                        runningProcedures.RemoveAt(runningProcedures.Count - 1);
                        //cerr << getHost()->getName()<<" : suppression de la procedure"<< endl;
                    }
                    else if (procInfo.isFinished2() && agt.ControlledByHuman)
                    {
                        bool StillAction = false;
                        for (int iA = 0; iA < actionsToDo.Count; iA++)
                        {
                          //  file.WriteLine("Comparing Procs : " + actionsToDo[iA].Key.procedure.name + " / " + procInfo.procedure.name);
                            if (actionsToDo[iA].Key.procedure.name == procInfo.procedure.name)
                            { StillAction = true;
                         //   file.WriteLine(" Still action : " + actionsToDo[iA].Value.Action.Description);
                            }
                        }

                        if (!StillAction)
                        {
                         //   file.WriteLine("ProceduralBehavior Procedure : " + runningProcedures[iP].procedure.name + " finie 2"); file.Flush();
                            //inform all agents that this agent's role in this procedure has ended (used to unblock tokens in ProcedureExecution)
                            sendProcedureDoneMessage(procInfo);

                            runningProcedures[iP] = runningProcedures[runningProcedures.Count - 1];
                            runningProcedures.RemoveAt(runningProcedures.Count - 1);
                            //cerr << getHost()->getName()<<" : suppression de la procedure"<< endl;
                        }
                        
                    }
                }
            }
            else
            {
//file.Close();

                ispause = true;
                //pause();
            }
            //file.Close();

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
                ispause = true;
            }
        }

        public void onBehaviorStop(BehaviorExecution be)
        {
          //  file.WriteLine(" ### BEHAVIOR STOP " + behaviorToNode[be].name + " ended ");
            Agent agt = (Agent)(this.Host);
            if (agt.ControlledByHuman)
            {
                for (int i = 0; i < actionsToDo.Count; i++)
                    if (behaviorToNode[be].Action.name == actionsToDo[i].Value.Action.name)
                        actionsToDo.RemoveAt(i);
            }

            for (int iP = 0; iP < runningProcedures.Count; iP++)
            {
                ProcedureExecution procInfo = runningProcedures[iP];
                procInfo.informActionDone(((Agent)(this.Host)).Aid, behaviorToNode[be]);
                sendActionDoneMessage(behaviorToNode[be], procInfo);
            }
            
            //restart();
            ispause = false;
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

            //send message to other agents
            List<AID> agents = procInfo.getOtherAgents();

            for (int iA = 0; iA < agents.Count; iA++)
            {
                procMsg.Receivers.Add(agents[iA]);
            }
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

        public List<ProcedureExecution> runningProcedures;
        public List< KeyValuePair<ProcedureExecution, ActionNode> > actionsToDo = new List<KeyValuePair<ProcedureExecution, ActionNode>>();
        public Dictionary<BehaviorExecution, ActionNode> behaviorToNode;


    }
}

