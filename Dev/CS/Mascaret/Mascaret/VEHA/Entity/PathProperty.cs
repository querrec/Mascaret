
using System;
using System.Collections.Generic;

namespace Mascaret
{
    public class PathProperty : TopologicalProperty
    {
        public PathProperty(string name, Class classe)
            : base(name, classe, MascaretApplication.Instance.Model.getBasicType("path"))
        {
        }

    }
}


