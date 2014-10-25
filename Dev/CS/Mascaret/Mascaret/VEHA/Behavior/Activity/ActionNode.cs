using System;
using System.Collections.Generic;



[Serializable]
public class ActionNode : ActivityNode
{
	 
	private Action action;
	public Action Action
	{
		get{return action;}
		set{action = value;}
	}
	
	 
	private BehaviorExecution currentExecution;
	public BehaviorExecution CurrentExecution
	{
		get{return currentExecution;}
		set{currentExecution = value;}
	}
	
	//default parameters kind = "action"
	public ActionNode (string name, string kind) :  base(name,kind)
	{
	}
	
	//default parameters sync = false
	public void start(InstanceSpecification host, Dictionary<string,InstanceSpecification> affectations, bool sync)
	{
		if(action==null)
		{
			System.Console.WriteLine("Le noeud "+name+" n'a pas d'action associée!!!");
			return;
		}
		
		Dictionary<string, ValueSpecification>  param = new Dictionary<string, ValueSpecification>();
		if(action.Kind == "SendSignal")
		{
			if(((SendSignalAction)action).Target==null)
			{
				foreach(ActivityEdge currentEdge in Outgoing)
				{
					if(currentEdge.Target.Kind =="object")
					{
						if(affectations.ContainsKey(currentEdge.Target.name))
						{
							System.Console.WriteLine("Sending signal to : " + currentEdge.Target.name);
							SendSignalAction ssAct = (SendSignalAction)action;
							ssAct.Target = new SendSignalTarget();
							ssAct.Target.target =  affectations[currentEdge.Target.name];
						}

						else
						{
							System.Console.WriteLine("affectation of "+currentEdge.Target.name+" not found");
						}
						
					}
				}
			}
		}
		if(action.Kind == "CallOperation")
		{
			System.Console.WriteLine("Executing Call operation" +action.name +" "+((CallOperationAction)action).Operation.getFullName());
			
			if(((CallOperationAction)action).isDynamic())
			{
				param = ((CallOperationAction)action).getParameters();
				
			}
			else
			{
				foreach(InputPin currentPin in action.InputPins)
				{
					string paramName = currentPin.getIncomingObjectNode()[0].name;
					InstanceValue val= new InstanceValue(affectations[currentPin.getIncomingObjectNode()[0].name]);
					param.Add(currentPin.name, val);
				}
			}
			
			if(Activity!=null)
			{
			//	action.Context(Activity.Context);
			}	
			
		}
		currentExecution = BehaviorScheduler.Instance.executeBehavior(action, host, param, sync);
	}
	
	public void stop()
	{
		if(currentExecution!=null)
			currentExecution.stop();
	}
	
	public bool isRunning()
	{
		if(currentExecution == null)
			return false;
		else
			return !currentExecution.IsFinished;
	}
}
