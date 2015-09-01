using System;
using System.Collections.Generic;

namespace Mascaret
{
    public class LiteralInteger : LiteralSpecification
    {

        protected int iValue;
        public int IValue
        {
            get { return iValue; }
            set { iValue = value; }
        }

        public bool setValueFromString(string intvalue)
        {
            iValue = int.Parse(intvalue);
            return true;
        }

        public override string getStringFromValue()
        {
            return (iValue.ToString());
        }

        public override int getIntFromValue()
        {
            return iValue;
        }

        public LiteralInteger(string intvalue)
            : base(MascaretApplication.Instance.Model.getBasicType("integer"))
        {
            setValueFromString(intvalue);
        }

        public LiteralInteger(int i)
            : base(MascaretApplication.Instance.Model.getBasicType("integer"))
        {
            iValue = i;
        }

        public LiteralInteger()
            : base(MascaretApplication.Instance.Model.getBasicType("integer"))
        {
        }

        public override ValueSpecification clone()
        {
            LiteralInteger vs = new LiteralInteger(0);
            vs.CLASSTYPE = CLASSTYPE;
            vs.Type = this.Type;
            vs.iValue = iValue;
            return ((ValueSpecification)vs);
        }

    }
}

