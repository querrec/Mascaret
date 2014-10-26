
using System;
using System.Collections.Generic;


namespace Mascaret
{
    public class ObjectFlow : ActivityEdge
    {

        public ObjectFlow(string name)
            : base(name)
        {
            Kind = "ObjectFlow";
        }
    }
}
