
using System;
using System.Collections.Generic;


[Serializable]
public class ObjectFlow : ActivityEdge
{
	
	public ObjectFlow(string name) : base(name)

	{
		Kind="ObjectFlow";
	}
}
