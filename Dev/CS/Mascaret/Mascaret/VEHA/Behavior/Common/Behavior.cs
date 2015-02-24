using System;
using System.Collections.Generic;


namespace Mascaret
{
    public abstract class Behavior : Classifier
    {

        private List<Constraint> preCondition;
        public List<Constraint> PreCondition
        {
            get { return preCondition; }
            set { preCondition = value; }
        }


        private List<Constraint> postCondition;
        public List<Constraint> PostCondition
        {
            get { return preCondition; }
            set { preCondition = value; }
        }


        private Classifier owner;
        public Classifier Owner
        {
            get { return owner; }
            set { owner = value; }
        }

        public Dictionary<string,ValueSpecification> context;

        public Behavior(string name)
            : base(name)
        {
        }

        public override ValueSpecification createValueFromString(string str)
        {
            throw new NotImplementedException();
        }

        public override string getFullName()
        {
            if (owner != null)
                return owner.getFullName() + "::" + name;
            else
                return name;
        }

        //default parameters sync = false
        public abstract BehaviorExecution createBehaviorExecution(InstanceSpecification host, Dictionary<String, ValueSpecification> p, bool sync);
    }
}
