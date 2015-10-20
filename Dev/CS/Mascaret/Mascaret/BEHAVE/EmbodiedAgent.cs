using System;
using System.Collections.Generic;


namespace Mascaret
{
    public class EmbodiedAgent : Agent
    {
        //bilal 19-10-2015
        public override void addIntention(string fml)
        {
            behaviorPlanner.parseIntention(fml);
        }
        //bilal 19-10-2015

        private Body body;
        public Body Body
        {
            get { return body; }
            set { body = value; }
        }

        public EmbodiedAgent(AgentPlateform ap, string name, AgentClass agentclass)
            : base(ap, name, agentclass, "default")
        {
            body = new Body(this);
        }

    }
}


