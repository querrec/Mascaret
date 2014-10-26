using System;
using System.Collections.Generic;

namespace Mascaret
{
    public class CallBehaviorAction : Action, IEquatable<CallBehaviorAction>
    {

        private InstanceSpecification target;
        public InstanceSpecification Target
        {
            get { return target; }
            set { target = value; }
        }

        private string stringArguments;
        public string StringArguments
        {
            get { return stringArguments; }
            set { stringArguments = value; }
        }

        private Behavior behavior;
        public Behavior Behavior
        {
            get { return behavior; }
            set { behavior = value; }
        }

        public CallBehaviorAction()
        {
            Kind = "CallBehavior";
        }

        public bool Equals(CallBehaviorAction other)
        {
            if (other == this)
                return true;
            return false;
        }

        public override BehaviorExecution createBehaviorExecution(InstanceSpecification host, Dictionary<string, ValueSpecification> p, bool sync)
        {
            CallBehaviorBehaviorExecution behavior = new CallBehaviorBehaviorExecution(this, host, p);
            return behavior;
        }
    }
}