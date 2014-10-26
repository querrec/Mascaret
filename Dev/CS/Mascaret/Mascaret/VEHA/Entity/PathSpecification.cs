using System;
using System.Collections.Generic;


namespace Mascaret
{
    public class PathSpecification : TopologicalSpecification
    {

        private List<PointSpecification> points = new List<PointSpecification>();
        public List<PointSpecification> Points
        {
            get { return points; }
            set { points = value; }
        }

        public PathSpecification()
            : base(MascaretApplication.Instance.Model.getBasicType("path"))
        {
        }

        public PathSpecification(string str)
            : base(MascaretApplication.Instance.Model.getBasicType("path"))
        {
            //TODO parse string
        }

        public override ValueSpecification clone()
        {
            throw new NotImplementedException();
        }
    }
}


