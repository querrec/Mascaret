using System;
using System.Collections.Generic;


namespace Mascaret
{
    public class LiteralReal : LiteralSpecification
    {


        protected double rValue;
        public double RValue
        {
            get { return rValue; }
            set { rValue = value; }
        }

        public bool setValueFromString(string value)
        {
            rValue = double.Parse(value);
            return true;
        }

        public override string getStringFromValue()
        {
            return (rValue.ToString());
        }

        public override double getDoubleFromValue()
        {
            return rValue;
        }

        public LiteralReal()
            : base(MascaretApplication.Instance.Model.getBasicType("real"))
        {
        }

        public LiteralReal(string svalue)
            : base(MascaretApplication.Instance.Model.getBasicType("real"))
        {
            setValueFromString(svalue);
        }

        public LiteralReal(double r)
            : base(MascaretApplication.Instance.Model.getBasicType("real"))
        {
            rValue = r;
        }

        public override ValueSpecification clone()
        {
            LiteralReal vs = new LiteralReal(0.0);
            vs.CLASSTYPE = CLASSTYPE;
            vs.Type = Type;
            vs.rValue = rValue;
            return ((ValueSpecification)vs);
        }

    }
}

