using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Mascaret
{
    public class FlowPort : Port
    {
        string direction = "";
        public string Direction
        {
            get { return direction; }
            set { direction = value; }
        }

        public FlowPort(string name, Class cl, Classifier type, Property opposite, ValueSpecification def, DomainSpecification domain):
            base(name, cl, type, opposite, def, domain)
        {

        }
    }
}
