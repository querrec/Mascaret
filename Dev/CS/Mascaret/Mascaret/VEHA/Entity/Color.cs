
using System;

namespace Mascaret
{
    public class Color : ValueSpecification
    {

        public Color(double r, double g, double b)
            : base(MascaretApplication.Instance.Model.getBasicType("color"))
        {
            this.r = r;
            this.g = g;
            this.b = b;
        }

        public Color(double r, double g, double b, float a)
            : base(MascaretApplication.Instance.Model.getBasicType("color"))
        {
            this.r = r;
            this.g = g;
            this.b = b;
            this.a = a;
        }


        public Color(float alpha)
            : base(MascaretApplication.Instance.Model.getBasicType("color"))
        {
            this.a = alpha;
        }

        public override ValueSpecification clone()
        {
            return new Color(r, g, b);
        }

        public double r, g, b;
        public float a;
    }
}
