
using System;
using System.Collections.Generic;


[Serializable]
public abstract class LiteralSpecification : ValueSpecification
{
	public LiteralSpecification(Classifier type) : base(type)
	{
		CLASSTYPE="Litteral";
	}
	
	public override ValueSpecification clone()
	{
		return null;
	}

}
