using System;
using System.Collections.Generic;

namespace Mascaret
{
    public class OneShotBehaviorExecution : SimpleBehaviorExecution
    {
        public OneShotBehaviorExecution(Behavior specif, InstanceSpecification host, Dictionary<String, ValueSpecification> p)
            : base(specif, host, p)
        {

        }

        public override bool done()
        {
            return true;
        }
    }
}

