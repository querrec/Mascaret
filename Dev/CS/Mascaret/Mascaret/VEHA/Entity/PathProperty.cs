
using System;
using System.Collections.Generic;


[Serializable]
public class PathProperty : TopologicalProperty
{
	public PathProperty(string name, Class classe) : base(name, classe, MascaretApplication.Instance.Model.getBasicType("path"))

	{
	}

}


