using System;
using System.Collections.Generic;


namespace Mascaret
{
    public class EmbodiedAgent : Agent
    {
        public BehaviorRealizer behaviorRealizer;

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

        public override void addIntention(string fml)
        {
            List<string> bmlList = behaviorPlanner.parseIntention(fml);
            foreach (string bmlI in bmlList)
                this.behaviorRealizer.addBehavior(bmlI);
        }

    }
}


