using System;
using System.Collections.Generic;


namespace Mascaret
{
    public class ShapeGeometry : GeometrySpecification
    {

        private ShapeSpecification shape;
        public ShapeSpecification Shape
        {
            get { return shape; }
            set { shape = value; }
        }

        public ShapeGeometry()
            : base()
        {
            shape = null;
        }

    }
}
