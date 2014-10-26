
using System;

namespace Mascaret
{
    public class Vector3 : ValueSpecification
    {
        public double x;
        public double y;
        public double z;

        public Vector3(double x, double y, double z)
            : base(MascaretApplication.Instance.Model.getBasicType("vector3"))
        {
            this.x = x;
            this.y = y;
            this.z = z;
        }

        public override ValueSpecification clone()
        {
            return new Vector3(x, y, z);
        }

        /** Normalize / get length */
        public void normalize()
        {
            double l = length();
            // TODO: Maybe to change, in AReVi we use an "if(l > ARMATH_PRECISION)"
            x /= l;
            y /= l;
            z /= l;
        }

        public Vector3 unit()
        {
            double l = length();
            // TODO: Maybe to change, in AReVi we use an "if(l > ARMATH_PRECISION)"
            return new Vector3(x / l, y / l, z / l);

        }

        public double squareLength()
        {
            return x * x + y * y + z * z;
        }

        public double length()
        {
            return 0;
            //return sqrt(squareLength());
        }

    }
}