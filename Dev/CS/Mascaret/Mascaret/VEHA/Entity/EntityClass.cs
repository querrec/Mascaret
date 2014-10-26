using System;
using System.Collections.Generic;

namespace Mascaret
{
    public class EntityClass : Class
    {

        private List<ShapeProperty> shape = new List<ShapeProperty>();
        public List<ShapeProperty> Shape
        {
            get { return shape; }
            set { shape = value; }
        }


        private GeometryProperty geometry = null;
        public GeometryProperty Geometry
        {
            get { return geometry; }
            set { geometry = value; }
        }


        private TopologicalProperty referentialPoint = null;
        public TopologicalProperty ReferentialPoint
        {
            get { return referentialPoint; }
            set { referentialPoint = value; }
        }

        public ShapeProperty getShapeAt(int index)
        {
            return shape[index];
        }

        public void addShape(ShapeProperty shapeProperty)
        {
            shape.Add(shapeProperty);
        }

        public EntityClass(string name)
            : base(name)
        {
            ShapeProperty property = new ShapeProperty("shape", this);
            addAttribute((Property)property);
        }

    }
}
