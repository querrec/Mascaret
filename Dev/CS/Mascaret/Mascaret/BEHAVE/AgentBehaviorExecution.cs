using System;
using System.Collections.Generic;

namespace Mascaret
{
    public abstract class AgentBehaviorExecution : BehaviorExecution
    {
        public AgentBehaviorExecution(Behavior specif, InstanceSpecification host, Dictionary<String, ValueSpecification> p)
            : base(specif, host, p, false)
        {
        }

        public AgentBehaviorExecution()
        {

        }

        public abstract void action();

        public abstract bool done();


    }
}

