using System;
using System.Collections.Generic;
using System.Reflection;


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

            
            Type [] types = Assembly.GetCallingAssembly().GetTypes();
            Type type = null;
            foreach (Type t in types)
            {
                if (t.Name == name) type = t;
            }
            
            
            BehaviorExecution be = null;
            if (type != null)
            {
                be = (BehaviorExecution)(Activator.CreateInstance(type));
                be.init((Behavior)this, host, p, false);
            }
            else MascaretApplication.Instance.VRComponentFactory.Log("ERREUR : " + name + " not found");
            

            return be;
           
        }



    }
}

