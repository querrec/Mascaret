using System;
using System.Collections.Generic;

namespace Mascaret
{

    public enum AgentBehaviorType { Perception, Decision, Action };

    public class AgentBehavior : Behavior
    {

        private AgentBehaviorType type;
        public AgentBehaviorType Type
        {
            get { return type; }
            set { type = value; }
        }

        //default parameters KBName = "default"
        public AgentBehavior(string name)
            : base(name)
        {

        }

        //default parameters sync = false
        public override BehaviorExecution createBehaviorExecution(InstanceSpecification host, Dictionary<String, ValueSpecification> p, bool sync)
        {
            if (name == "SimpleCommunicationBehavior")
            {
                System.Console.WriteLine(" ######## CREATE SIMPLE COM BEHAVIOR");
                SimpleCommunicationBehavior scb = new SimpleCommunicationBehavior((Behavior)this, host, p);
                scb.Interval = 0.5;
                return scb;
            }
            else if (name == "ProceduralBehavior")
            {
                ProceduralBehavior pb = new ProceduralBehavior((Behavior)this, host, p);
                pb.Interval = 0.5;
                return pb;
            }
            else if (name == "ActionListenerBehavior")
            {
                ActionListenerBehavior alb = new ActionListenerBehavior((Behavior)this, host, p);
                return alb as BehaviorExecution;
            }
            else
            {
                //BehaviorInitDeleterFunc operations
                return null;
            }
        }



    }
}

