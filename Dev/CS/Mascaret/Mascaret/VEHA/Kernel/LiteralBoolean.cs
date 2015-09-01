using System;
using System.Collections.Generic;


namespace Mascaret
{
    public class LiteralBoolean : LiteralSpecification
    {

        protected bool bValue;
        public bool BValue
        {
            get { return bValue; }
            set { bValue = value; }
        }

        public bool setValueFromString(string sValue)
        {
            if (sValue.ToLower() == "true") bValue = true;
            else if (sValue.ToLower() == "false") bValue = false;
            else return false;
            return true;
        }

        public override string getStringFromValue()
        {
            return (bValue ? "true" : "false");
        }

        public override bool getBoolFromValue()
        {
            return bValue;
        }

        public LiteralBoolean()
            : base(MascaretApplication.Instance.Model.getBasicType("boolean"))
        {
        }

        public LiteralBoolean(bool b)
            : base(MascaretApplication.Instance.Model.getBasicType("boolean"))
        {
            bValue = b;
        }

        public LiteralBoolean(string svalue)
            : base(MascaretApplication.Instance.Model.getBasicType("boolean"))
        {
            bValue = false;
            setValueFromString(svalue);
        }

        public override ValueSpecification clone()
        {
            LiteralBoolean vs = new LiteralBoolean("");
            vs.CLASSTYPE = CLASSTYPE;
            vs.Type = this.Type;
            vs.bValue = bValue;
            return ((ValueSpecification)vs);
        }
    }
}