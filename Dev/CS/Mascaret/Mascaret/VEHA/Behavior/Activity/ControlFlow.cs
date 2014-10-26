using System;
using System.Collections.Generic;


namespace Mascaret
{
    public class ControlFlow : ActivityEdge
    {

        private Parameter parameter;
        public Parameter Parameter
        {
            get { return parameter; }
            set { parameter = value; }
        }

        public ControlFlow(string name)
            : base(name)
        {
            Kind = "ControlFlow";
        }
    }
}
