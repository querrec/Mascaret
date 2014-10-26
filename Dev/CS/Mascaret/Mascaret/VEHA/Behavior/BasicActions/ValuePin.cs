using System;

namespace Mascaret
{
    public class ValuePin : InputPin
    {

        private ValueSpecification valueSpec;
        public ValueSpecification ValueSpec
        {
            get { return valueSpec; }
            set { valueSpec = value; }
        }

        public ValuePin()
        {
            Kind = "valuePin";
        }
    }
}


