using System;
using System.Collections.Generic;


[Serializable]
public class SendSignalBehaviorExecution : BehaviorExecution
{

	 
	private SendSignalAction action;
	public SendSignalAction Action
	{
		get{return action;}
		set{action = value;}
	}
	
	 
	private InstanceSpecification signal;
	public InstanceSpecification Signal
	{
		get{return signal;}
		set{signal = value;}
	}
	
	 
	private Signal signalClass;
	public Signal SignalClass
	{
		get{return signalClass;}
		set{signalClass = value;}
	}
	
	public SendSignalBehaviorExecution(SendSignalAction action, InstanceSpecification host, Dictionary<string,ValueSpecification> p) :	base(action, host, p, false)

	{
		this.action = action;
	}
	
	public bool Equals (SendSignalAction other)
	{
		if(other == this.Action)
			return true;
		return false;
	}
	
	
	public override double execute (double dt)
	{
		//Debug.Log(" EXECUTION SEND SIGNAL ... : " + action.Target.target.getFullName());
		
		if(action.Target.target!=null)
		{
			System.Console.WriteLine("Send Signal Action to "+action.Target.target.getFullName() + " : " + action.Target.target.SmBehaviorExecutions.Count);
			foreach(StateMachineBehaviorExecution smBe in action.Target.target.SmBehaviorExecutions)
			{
				if(action.Signal!=null)
					smBe.addSignal(action.Signal);
				else
				{
					if(action.SignalClass!=null)
					{
						InstanceSpecification sig = new InstanceSpecification(action.SignalClass.name, action.SignalClass);
						smBe.addSignal(sig);
					}
					else
						System.Console.WriteLine("Pas de Signal");
				}
			}
		}
		else
		{
			if(action.Signal!=null)
				System.Console.WriteLine(action.Signal.name);
			else if(action.SignalClass!=null)
				System.Console.WriteLine(" (Class) "+action.SignalClass.name);
			else
				System.Console.WriteLine(" No signal or signalClass");
			
			foreach(InstanceSpecification currentInstance in MascaretApplication.Instance.getEnvironment().InstanceSpecifications.Values)
			{
				
				foreach(StateMachineBehaviorExecution smBe in currentInstance.SmBehaviorExecutions)
				{
					if(action.Signal!=null)
						smBe.addSignal(action.Signal);
					else
					{
						if(action.SignalClass!=null)
						{
							InstanceSpecification sig = new InstanceSpecification(action.SignalClass.name, action.SignalClass);
							smBe.addSignal(sig);
						}
						else
							System.Console.WriteLine("Pas de Signal");
					}
				}
			}
		}
		
		
		return 0;
	}
}
