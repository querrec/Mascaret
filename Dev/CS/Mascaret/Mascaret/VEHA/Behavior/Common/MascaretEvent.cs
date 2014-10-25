using System;
using System.Collections.Generic;


[Serializable]
public class MascaretEvent : Element
{
	 
	private string type;
	public string Type
	{
		get{return type;}
		set{type = value;}
	}
	
	public MascaretEvent(string name) : base(name)
	{
	}

}

