using System;
using System.Collections.Generic;



namespace Mascaret
{
    public class LiteralString : LiteralSpecification
    {


        protected string sValue;
        public string SValue
        {
            get { return sValue; }
            set { sValue = value; }
        }

        public override string getStringFromValue()
        {
            return (sValue);
        }

        public LiteralString(string svalue)
            : base(MascaretApplication.Instance.Model.getBasicType("string"))
        {
            sValue = svalue;
        }

        public LiteralString()
            : base(MascaretApplication.Instance.Model.getBasicType("string"))
        {
            sValue = "";
        }

        public override ValueSpecification clone()
        {
            LiteralString vs = new LiteralString("");
            vs.CLASSTYPE = CLASSTYPE;
            vs.Type = this.Type;
            vs.sValue = sValue;
            return ((ValueSpecification)vs); ;
        }
    }
}

