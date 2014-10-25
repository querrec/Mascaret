using System;
using System.Collections.Generic;

[Serializable]
public class ProceduralBehavior:CyclicBehaviorExecution
{

	public bool ispause = false;
	
	public ProceduralBehavior(Behavior behavior, InstanceSpecification host, Dictionary<String,ValueSpecification> p) : base(behavior, host, p)
	{
		
		runningProcedures = new List<ProcedureExecution>();
		behaviorToNode = new Dictionary<BehaviorExecution, ActionNode>();
	}
	
	public override void action()
	{
		Agent agt = (Agent)(this.Host);
		
		if (ispause) return;
		
		System.Console.WriteLine("RUNNING PROC : " + runningProcedures.Count);
		
		if (runningProcedures.Count > 0)
		{
			for (int iP = 0; iP < runningProcedures.Count; iP++)
			{
				ProcedureExecution procInfo = runningProcedures[iP];
				List<ActionNode> actionNodes = new List<ActionNode>();
				
				//remove those which are not agreed upon
				if (agt.ControlledByHuman)
				{
					System.Console.WriteLine("[ProceduralBehavior Info] Agent controlled ... ");
					actionNodes = procInfo.getActionToExecute();
					for (int atd=0; atd<actionNodes.Count; ++atd)
						System.Console.WriteLine("ProceduralBehavior Info] -- Human has to do : " + actionNodes[atd].name);
					actionNodes.Clear();

					for (int atd=0; atd<actionsToDo.Count; ++atd)
					{
						actionNodes.Add(actionsToDo[atd]);
					}
					actionsToDo.Clear();
				}
				else
				{
					//Debug.Log("ProceduralBehavior getActionToExecute() ... ");
					actionNodes = procInfo.getActionToExecute();
				}
				
				if (actionNodes.Count > 0)
				{
					System.Console.WriteLine(" ############################################### ");
					System.Console.WriteLine(" ##     " + agt.name + " : " + actionNodes.Count);
					System.Console.WriteLine(" ############################################### ");

					for(int i=0;i<actionNodes.Count;i++)
					{
						actionNodes[i].start(agt,procInfo.getAffectations(),false); //TODO ///////////////////////////////////////// if not flag start, else start
						sendActionRealisationMessage(actionNodes[i], procInfo); // TODO
						
						procInfo.informActionRunning(agt.Aid,actionNodes[i]);
						
						if(actionNodes[i].CurrentExecution!= null)
						{
							//cerr << "execution ok" << endl;
							behaviorToNode.Add(actionNodes[i].CurrentExecution,actionNodes[i]);
                            
							actionNodes[i].CurrentExecution.addCallbackOnBehaviorStop(onBehaviorStop);
						}
						else
						{
							//cerr << "execution ko --> ignore (= says realization ok)" << endl;
							procInfo.informActionDone(agt.Aid,actionNodes[i]); //TODO
							sendActionDoneMessage(actionNodes[i], procInfo);
						}
					}
					ispause = true;
				}
				else if (agt.getMessageQueue().Count == 0)
				{
					//pause();
					ispause = true;
				}
				
				if (procInfo.isFinished())
				{
					//inform all agents that this agent's role in this procedure has ended (used to unblock tokens in ProcedureExecution)
					sendProcedureDoneMessage(procInfo);
					
					runningProcedures[iP]=runningProcedures[runningProcedures.Count -1];
					runningProcedures.RemoveAt(runningProcedures.Count -1);
					//cerr << getHost()->getName()<<" : suppression de la procedure"<< endl;
				}
				else
				{
					iP ++;
				}
				
			}
		}
		else
		{
			ispause = true;
			//pause();
		}
	}
	
	public void pushProcedureToDo(Procedure proc, OrganisationalEntity org, Role role, Dictionary<String,ValueSpecification> parameters)
	{
		Agent agt = (Agent)(this.Host);
		
		ProcedureExecution procInfo = new ProcedureExecution(proc, org, agt);
		procInfo.buildAffectations(parameters, this.Host);

		/* Search if the current agent have something to do in this procedure */
		List<ActivityPartition>  parts = proc.Activity.Partitions;
		bool found = false;
		for (int iP = 0; iP < parts.Count; iP++)
		{
			if (parts[iP].name == role.name)
			{
				System.Console.WriteLine("[ProceduralBehavior Info] Found a role " + role.name + " in procedure " + proc.name);
				found = true;
			}
		}

		runningProcedures.Add(procInfo);
		ispause = false;

		if (!found)
		{
			//inform all agents that this agent's role in this procedure has ended (used to unblock tokens in ProcedureExecution)
			sendProcedureDoneMessage(procInfo);
			runningProcedures.RemoveAt(runningProcedures.Count-1); //remove it..
			ispause = true;
		}
	}
	
	protected void onBehaviorStop(BehaviorExecution be)
	{
		System.Console.WriteLine(" ### BEHAVIOR STOP " + behaviorToNode[be].name + " ended ");
		for (int iP = 0; iP < runningProcedures.Count; iP++)
		{
			ProcedureExecution procInfo = runningProcedures[iP];
			procInfo.informActionDone(((Agent)(this.Host)).Aid,behaviorToNode[be]);
			sendActionDoneMessage(behaviorToNode[be], procInfo);
		}
		//restart();
		ispause = false;
}
	
	protected void sendProcedureDoneMessage(ProcedureExecution procInfo)
	{
		Agent agt = (Agent)(this.Host);
	
		ACLMessage procMsg = new ACLMessage(ACLPerformative.INFORM);
	
		//we inform at wich time the procedure finish
		TimeExpression timestamp = BehaviorScheduler.Instance.getCurrentVirtualTime();
		procMsg.Timestamp = timestamp;

		string content ="((done (action ";
		content += agt.name;
		content += " ";
		content += "(" + clean(procInfo.procedure.name) +")";
		content += ")))";
		procMsg.Content = content;
	
		procMsg.Receivers.Add(MascaretApplication.Instance.Agent.Aid);
	
		agt.send(procMsg);
	}
	
	protected void sendActionRealisationMessage(ActionNode action, ProcedureExecution procInfo)
	{
		Agent agt = (Agent)(this.Host);

		ACLMessage procMsg = new ACLMessage(ACLPerformative.INFORM);
	
		//we inform at wich time the action start
		TimeExpression timestamp = action.CurrentExecution.Start;
	
		procMsg.Timestamp = timestamp;
	
		//set ACLMessage content
		string content ="((action ";
		content += agt.name;
		content += " ";
		content += "(" + clean(action.name) +")";
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
	
	void sendActionDoneMessage(ActionNode action, ProcedureExecution procInfo)
	{
		Agent agt = (Agent)(this.Host);

		ACLMessage procMsg = new ACLMessage(ACLPerformative.INFORM);
	
		//we inform at wich time the action finished
		TimeExpression timestamp = action.CurrentExecution.Finish;
	
		procMsg.Timestamp = timestamp;

		//set ACLMessage content
		string content ="((done (action ";
		content += agt.name;
		content += " ";
		content += "(" + clean(action.name) +")";
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
		string returned=toClean;
		returned.Replace("/","");
		returned.Replace("(","");
		returned.Replace(")","");
		returned.Replace("^","");
	
		return returned;
	}
	
	protected List<ProcedureExecution> runningProcedures;
	protected List<ActionNode> actionsToDo;
	protected Dictionary<BehaviorExecution,ActionNode>  behaviorToNode ;
	
	
}

