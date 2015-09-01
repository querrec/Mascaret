using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Mascaret
{
    public class Port : Property
    {
        private Boolean isService = false;
        public Boolean IsService
        {
            get { return isService; }
            set { isService = value; }
        }

        private Boolean isBehavior = false;
        public Boolean IsBehavior
        {
            get { return isBehavior; }
            set { isBehavior = value; }
        }

        public Port(string name, Class cl, Classifier type, Property opposite, ValueSpecification def, DomainSpecification domain):
            base(name, cl, type, opposite, def, domain)
        {

        }
    }
}
