
using System;
using System.Collections.Generic;


[Serializable]
public class TimeEvent : MascaretEvent
{
	 
	private bool isRelative;
	public bool IsRelative
	{
		get{return isRelative;}
		set{isRelative=value;}
	}
	
	 
	private ValueSpecification when;
	public ValueSpecification When
	{
		get{return when;}
		set{when=value;}
	}
	
	public TimeEvent(string name) : base(name)
	{
		Type = "TimeEvent";
	}
	
}