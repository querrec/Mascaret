
using System;
using System.Collections.Generic;

[Serializable]
public class PointProperty : TopologicalProperty
{
	public PointProperty(string name, Class classe) : base(name, classe, MascaretApplication.Instance.Model.getBasicType("point"))

	{
	}
	
}


