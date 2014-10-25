using System;
using System.Collections.Generic;


[Serializable]
public class EmbodiedAgent : Agent
{
	 
	private Body body;
	public Body Body
	{
		get{return body;}
		set{body = value;}
	}
	
	public EmbodiedAgent(AgentPlateform ap, string name, AgentClass agentclass) : base(ap, name, agentclass,"default")
	{
		body = new Body(this);
	}

}


