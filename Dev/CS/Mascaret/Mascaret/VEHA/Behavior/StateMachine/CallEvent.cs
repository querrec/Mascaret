using System;
using System.Collections.Generic;

namespace Mascaret
{
    public class CallEvent : MascaretEvent, IEquatable<CallEvent>
    {

        private Operation operation = null;
        public Operation Operation
        {
            get { return operation; }
            set { operation = value; }
        }

        public CallEvent(string name)
            : base(name)
        {
            Type = "CallEvent";
        }

        public bool Equals(CallEvent other)
        {
            if (other == this)
                return true;
            return false;
        }
    }
}