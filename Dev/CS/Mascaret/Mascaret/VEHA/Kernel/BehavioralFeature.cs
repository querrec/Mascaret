using System;
using System.Collections.Generic;

namespace Mascaret
{
    public class BehavioralFeature : Feature
    {

        private Behavior method;
        public Behavior Method
        {
            get { return method; }
            set { method = value; }
        }

        public BehavioralFeature(string name, Behavior behavior)
            : base(name)
        {
            method = behavior;
        }

    }
}
