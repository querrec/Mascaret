
using System;
using System.Collections.Generic;

namespace Mascaret
{
    public class PointProperty : TopologicalProperty
    {
        public PointProperty(string name, Class classe)
            : base(name, classe, MascaretApplication.Instance.Model.getBasicType("point"))
        {
        }

    }
}


