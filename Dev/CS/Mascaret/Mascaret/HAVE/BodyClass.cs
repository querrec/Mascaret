using System;
using System.Collections.Generic;


namespace Mascaret
{
    public class BodyClass : EntityClass
    {

        private VirtualHumanClass ownerClass = null;
        public VirtualHumanClass OwnerClass
        {
            get { return ownerClass; }
            set { ownerClass = value; }
        }

        public Dictionary<string, Operation> getOperations()
        {
            Dictionary<string, Operation> ops = new Dictionary<string, Operation>();

            foreach (string key in Operations.Keys)
                ops.Add(key, Operations[key]);

            return ops;
        }

        public override bool hasOperation(string name)
        {
            return Operations.ContainsKey(name) || ownerClass.hasOperation(name);
        }

        public Dictionary<string, Property> getAttributes()
        {
            Dictionary<string, Property> atts = new Dictionary<string, Property>();
            foreach (string key in Attributes.Keys)
                atts.Add(key, Attributes[key]);
            return atts;
        }

        public override bool hasAttribute(string name)
        {
            return Attributes.ContainsKey(name) || ownerClass.hasAttribute(name);
        }

        public BodyClass(string name, VirtualHumanClass ownerClass)
            : base(name)
        {
            this.ownerClass = ownerClass;
        }

    }
}


