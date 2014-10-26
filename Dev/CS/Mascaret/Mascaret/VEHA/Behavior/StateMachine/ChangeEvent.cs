using System;
using System.Collections.Generic;

namespace Mascaret
{
    public class ChangeEvent : MascaretEvent, IEquatable<ChangeEvent>
    {

        private ValueSpecification changeExpression = null;
        public ValueSpecification ChangeExpression
        {
            get { return changeExpression; }
            set { changeExpression = value; }
        }

        public ChangeEvent(string name)
            : base(name)
        {
            Type = "ChangeEvent";
        }

        public bool Equals(ChangeEvent other)
        {
            if (other == this)
                return true;
            return false;
        }
    }
}