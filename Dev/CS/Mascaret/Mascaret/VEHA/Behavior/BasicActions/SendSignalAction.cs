using System;
using System.Collections.Generic;


namespace Mascaret
{
    public class SendSignalAction : Action, IEquatable<SendSignalAction>
    {


        private SendSignalTarget target;
        public SendSignalTarget Target
        {
            get { return target; }
            set { target = value; }
        }


        private InstanceSpecification signal;
        public InstanceSpecification Signal
        {
            get { return signal; }
            set { signal = value; }
        }


        private Signal signalClass;
        public Signal SignalClass
        {
            get { return signalClass; }
            set { signalClass = value; }
        }

        public SendSignalAction()
        {
            Kind = "SendSignal";
            target = null;
        }

        public bool Equals(SendSignalAction other)
        {
            if (other == this)
                return true;
            return false;
        }

        //default parameter sync = false
        public override BehaviorExecution createBehaviorExecution(InstanceSpecification host, Dictionary<string, ValueSpecification> p, bool sync)
        {
            SendSignalBehaviorExecution be = new SendSignalBehaviorExecution(this, host, p);
            return be;
        }

    }
}
