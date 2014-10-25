using System;
using System.Collections.Generic;


[Serializable]
public class Expression : ValueSpecification
{
	 
	private bool needUpdate;
	public bool NeedUpdate
	{
		get{return needUpdate;}
		set{needUpdate = value;}
	}
	
	 
	private string expressionValue;
	public string ExpressionValue
	{
		get{return expressionValue;}
		set{expressionValue = value;}
	}
	
	//callback
	
	public Expression(string expression, Classifier returnType) : base(returnType)
	{
		expressionValue=expression;
	}
	
	public override ValueSpecification clone ()
	{
		return null;
	}	
	
}