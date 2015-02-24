using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Mascaret
{
    class AcceptEventAction : Action, IEquatable<AcceptEventAction>
    {
        public AcceptEventAction()
        {
            Kind = "AcceptEvent";
        }


        public Trigger getTrigger()
        {
            return _trigger;
        }

        public void setTrigger(Trigger trigger)
        {
            _trigger = trigger;
        }

        public bool Equals(AcceptEventAction other)
        {
            if (other == this)
                return true;
            return false;
        }

        public override BehaviorExecution createBehaviorExecution(InstanceSpecification host, Dictionary<string, ValueSpecification> p, bool sync)
        {
            AcceptEventBehaviorExecution behavior = new AcceptEventBehaviorExecution(this, host, p);
            return behavior;
        }

        protected Trigger _trigger;
    }
}
