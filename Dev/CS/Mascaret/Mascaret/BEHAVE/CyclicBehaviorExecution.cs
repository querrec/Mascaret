using System;
using System.Collections.Generic;

namespace Mascaret
{
    public class CyclicBehaviorExecution : SimpleBehaviorExecution
    {
        public CyclicBehaviorExecution(Behavior specif, InstanceSpecification host, Dictionary<String, ValueSpecification> p)
            : base(specif, host, p)
        {
        }

        public CyclicBehaviorExecution()
        {
        }

        public override bool done()
        {
            return false;
        }
    }
}

