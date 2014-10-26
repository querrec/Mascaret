using System;
using System.Collections.Generic;

namespace Mascaret
{
    public class ActionListenerBehavior : CyclicBehaviorExecution
    {

        public ActionListenerBehavior(Behavior specif, InstanceSpecification host, Dictionary<String, ValueSpecification> p)
            : base(specif, host, p)
        {
        }

        public override void action()
        {
            //not implemented
        }

        //protected methods not translated right now

    }
}

