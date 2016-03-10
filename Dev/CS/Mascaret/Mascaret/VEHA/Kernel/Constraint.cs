using System;
using System.Collections.Generic;

namespace Mascaret
{
    public class Constraint
    {

        private ValueSpecification specification;
        public ValueSpecification Specification
        {
            get { return specification; }
            set { specification = value; }
        }

        Expression expr = null;
        bool result;

        public Constraint()
        {
        }

        public bool eval(Dictionary<string, ValueSpecification> context)
        {
           /* MascaretApplication.Instance.VRComponentFactory.Log("Evaluate constraint : " + context.Count);
            foreach(KeyValuePair<string,ValueSpecification> c in context)
                MascaretApplication.Instance.VRComponentFactory.Log(c.Key);
            * */

            if (expr != null) return result;
            else
            {
                expr = (Expression)(specification);
                ValueSpecification resultVS = expr.evaluateExpression(context);
                result = ((LiteralBoolean)(resultVS)).BValue;
                // MascaretApplication.Instance.VRComponentFactory.Log("Result " + result);
                return result;
            }
        }

    }
}

