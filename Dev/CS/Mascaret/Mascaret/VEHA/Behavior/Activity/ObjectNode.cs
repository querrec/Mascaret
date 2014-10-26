using System;
using System.Collections.Generic;

namespace Mascaret
{
    public class ObjectNode : ActivityNode
    {

        private ValueSpecification val;
        public ValueSpecification Val
        {
            get { return val; }
            set { val = value; }
        }


        private Classifier resourceType;
        public Classifier ResourceType
        {
            get { return resourceType; }
            set { resourceType = value; }
        }


        private string state;
        public string State
        {
            get { return state; }
            set { state = value; }
        }


        private bool inParticularState;
        public bool InParticularState
        {
            get { return inParticularState; }
            set { inParticularState = value; }
        }

        public ObjectNode(string name)
            : base(name, "object")
        {
        }

    }
}