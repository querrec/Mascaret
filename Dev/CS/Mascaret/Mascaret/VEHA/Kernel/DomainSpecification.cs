using System;
using System.Collections.Generic;


namespace Mascaret
{
    public class DomainSpecification
    {

        protected ValueSpecification lowerBound;
        public ValueSpecification LowerBound
        {
            get { return lowerBound; }
            set { lowerBound = value; }
        }


        protected ValueSpecification upperBound;
        public ValueSpecification UpperBound
        {
            get { return upperBound; }
            set { upperBound = value; }
        }


        protected ValueSpecification discretisation;
        public ValueSpecification Discretisation
        {
            get { return discretisation; }
            set { discretisation = value; }
        }

        public DomainSpecification()
        {

        }

    }
}
