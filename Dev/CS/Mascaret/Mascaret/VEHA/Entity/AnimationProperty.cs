
using System;
using System.Collections.Generic;

[Serializable]
public class AnimationProperty : Property
{
	public AnimationProperty(string name, Class classe) : base(name, classe,MascaretApplication.Instance.Model.getBasicType("animation"),null,null,null)

	{ 
	}
}

