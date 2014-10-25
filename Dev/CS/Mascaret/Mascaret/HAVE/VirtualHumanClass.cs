using System;
using System.Collections.Generic;


[Serializable]
public class VirtualHumanClass : AgentClass
{
	 
	private BodyClass bodyClass;
	public BodyClass BodyClass
	{
		get{return bodyClass;}
		set{bodyClass=value;}
	}
	
	public VirtualHumanClass(string name) : base(name)
	{
		bodyClass= new BodyClass("Body_"+name,this);
	}
	
}

