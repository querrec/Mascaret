using System;
using System.Collections.Generic;

[Serializable]
public class SpecificGeometry : GeometrySpecification
{
	 
	private string body;
	public string Body
	{
		get{return body;}
		set{body = value;}
	}
	
	public SpecificGeometry():base()
	{
		body = "";
	}
	
}

