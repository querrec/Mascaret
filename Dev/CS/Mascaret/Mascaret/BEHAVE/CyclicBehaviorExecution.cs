using System;
using System.Collections.Generic;

[Serializable]
public class CyclicBehaviorExecution:SimpleBehaviorExecution
{	
	public CyclicBehaviorExecution(Behavior specif, InstanceSpecification host, Dictionary<String,ValueSpecification> p) : base(specif, host,p)
	{
	}
	
	public override bool done()
	{
		return false;
	}
}

