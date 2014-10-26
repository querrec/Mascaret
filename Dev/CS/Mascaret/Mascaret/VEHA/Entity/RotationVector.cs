using System;
using System.Collections.Generic;


namespace Mascaret
{
    public class RotationVector : ValueSpecification
    {
        public double x, y, z, angle;

        //Default values x=1, y=0, z=0, a=0
        public RotationVector(double x, double y, double z, double a)
            : base(MascaretApplication.Instance.Model.getBasicType("rotation"))
        {
            this.x = x;
            this.y = y;
            this.z = z;
            this.angle = a;
        }

        public RotationVector(Vector3 v, double a)
            : base(MascaretApplication.Instance.Model.getBasicType("rotation"))
        {
            this.x = v.x;
            this.y = v.y;
            this.z = v.z;
            this.angle = a;
        }

        public RotationVector(string str)
            : base(MascaretApplication.Instance.Model.getBasicType("rotation"))
        {
            string[] strs = str.Split(char.Parse("\t "));
            if (strs.Length == 4)
            {
                x = Double.Parse(strs[0]);
                y = Double.Parse(strs[1]);
                z = Double.Parse(strs[2]);
                angle = Double.Parse(strs[3]);
            }
            else
                System.Console.WriteLine("string is not formated correctly");
        }

        public override ValueSpecification clone()
        {
            RotationVector valueSpec = new RotationVector(x, y, z, angle);
            return (ValueSpecification)valueSpec;
        }
    }
}

