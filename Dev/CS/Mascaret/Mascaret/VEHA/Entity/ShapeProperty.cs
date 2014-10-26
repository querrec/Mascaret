
using System;
using System.Collections.Generic;

namespace Mascaret
{
    public class ShapeProperty : Property
    {
        public ShapeProperty(string name, Class classe)
            : base(name, classe, MascaretApplication.Instance.Model.getBasicType("shape"), null, null, null)
        {
        }

    }
}


