using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Mascaret
{
    public class ValueType : Class
    {

        public ValueType(string name)
            : base(name)
        {
        }

        private Unit unit;
        public Unit Unit
        {
            get { return unit; }
            set { unit = value; }
        }

        public override ValueSpecification createValueFromString(string str)
        {
            string type = unit.Classifier.name;
            ValueSpecification valueSpec = null;
            if (type == "real" || type == "double")
            {
                valueSpec = new LiteralReal(str);
            }
            else if (type == "integer")
            {
                valueSpec = new LiteralInteger(str);
            }
            else if (type == "string")
            {
                valueSpec = new LiteralString(str);
            }
            else if (type == "boolean")
            {
                valueSpec = new LiteralBoolean(str);
            }

            return valueSpec;
        }





    }
}
