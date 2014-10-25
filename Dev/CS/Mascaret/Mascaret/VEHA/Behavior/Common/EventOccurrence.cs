using System;
using System.Collections.Generic;


[Serializable]
public class EventOccurence : Element
{
	 
	private string type;
	public string Type
	{
		get{return type;}
		set{type = value;}
	}
	
	public EventOccurence(string name) : base(name)
	{
	}

}

