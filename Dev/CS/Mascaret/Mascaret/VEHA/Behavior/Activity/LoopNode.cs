
using System;
using System.Collections.Generic;


[Serializable]
public class LoopNode : ActionNode
{
	
	 
	private bool testedFirst;
	public bool TestedFirst
	{
		get{return testedFirst;}
		set{testedFirst = value;}
	}
	
	 
	private	ActivityNode initial;
	public ActivityNode Initial
	{
		get{return initial;}
		set{initial = value;}
	}

	
	public LoopNode(string name,string kind) : 	base(name,kind)

	{
	}
	
	public bool eval()
	{
		return false;
	}
}
