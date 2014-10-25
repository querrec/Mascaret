using System;
using System.Collections.Generic;

[Serializable]
public class CallBehaviorAction : Action, IEquatable<CallBehaviorAction>
{
	
	public CallBehaviorAction()
	{
		Kind = "CallBehavior";
	}
	
	public bool Equals (CallBehaviorAction other)
	{
		if(other== this)
			return true;
		return false;
	}
	
	public override BehaviorExecution createBehaviorExecution (InstanceSpecification host, Dictionary<string,ValueSpecification> p, bool sync)
	{
		throw new NotImplementedException ();
	}
}