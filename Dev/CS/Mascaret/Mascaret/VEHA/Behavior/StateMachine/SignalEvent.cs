
using System;
using System.Collections.Generic;


namespace Mascaret
{
    public class SignalEvent : MascaretEvent
    {

        private Signal signalClass;
        public Signal SignalClass
        {
            get { return signalClass; }
            set { signalClass = value; }
        }


        private InstanceSpecification signal;
        public InstanceSpecification Signal
        {
            get { return signal; }
            set { signal = value; }
        }

        public SignalEvent(string name)
            : base(name)
        {
            Type = "SignalEvent";
        }

    }
}
