using System;
using System.Collections.Generic;

[Serializable]
public class Constraint 
{
	 
	private ValueSpecification  specification;
	public ValueSpecification Specification
	{
		get{return specification;}
		set{specification = value;}
	}
	
	public Constraint()
	{
	}
	
	public bool eval(Context context)
	{
		return false;
	}
	
}

