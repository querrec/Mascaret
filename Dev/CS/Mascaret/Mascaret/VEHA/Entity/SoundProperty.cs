using System;
using System.Collections.Generic;


[Serializable]
public class SoundProperty : Property
{
	public SoundProperty(string name, Class classe) : base(name, classe, MascaretApplication.Instance.Model.getBasicType("sound"),null, null, null)

	{
	}
}


