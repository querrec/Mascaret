using System;
using System.Collections.Generic;

namespace Mascaret
{
    public class SimpleBehaviorExecution : AgentBehaviorExecution
    {

        private double interval;
        public double Interval
        {
            get { return interval; }
            set { interval = value; }
        }

        public SimpleBehaviorExecution(Behavior specif, InstanceSpecification host, Dictionary<String, ValueSpecification> p)
            : base(specif, host, p)
        {

        }

        public SimpleBehaviorExecution()
        {
        }

        public override double execute(double dt)
        {
            action();
            if (!done())
                return interval;
            else return 0;
        }

        public override void action()
        {
            throw new NotImplementedException();
        }

        public override bool done()
        {
            throw new NotImplementedException();
        }
    }
}

