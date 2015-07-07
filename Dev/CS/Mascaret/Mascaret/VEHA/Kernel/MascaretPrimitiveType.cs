using System;
using System.Collections.Generic;

namespace Mascaret
{
    public class MascaretPrimitiveType : Class
    {


        private string type;
        public string Type
        {
            get { return type; }
            set { type = value; }
        }

        public override ValueSpecification createValueFromString(string str)
        {
            ValueSpecification valueSpec = null;
            if (type == "real" || type == "double")
            {
                valueSpec = new LiteralReal(str);
            }
            else if (type == "integer")
            {
                valueSpec = new LiteralInteger(str);
            }
            else if (type == "string")
            {
                valueSpec = new LiteralString(str);
            }
            else if (type == "boolean")
            {
                valueSpec = new LiteralBoolean(str);
            }
            /*else if (type =="color")
                valueSpec = new Color(str);
            else if (type =="Vector3")
                valueSpec = new Vector3(str);*/
            else if (type == "rotation")
            {
                valueSpec = new RotationVector(str);
            }
            else if (type == "shape")
            {
                //valueSpec = (UnityShapeSpecification)ScriptableObject.CreateInstance("UnityShapeSpecification");
                //((UnityShapeSpecification)valueSpec).instantiate(str);
            }
            else if (type == "point")
            {
                //valueSpec = (UnityPointSpecification)ScriptableObject.CreateInstance("UnityPointSpecification");
                //((UnityPointSpecification)valueSpec).instantiate(str);
            }
            /*else if (type =="path")
                valueSpec = new PathSpecification(str);
            else if (type =="sound")
                valueSpec = new UnitySoundSpecification(str);
            else if (type =="animation")
                valueSpec = new UnityAnimationSpecification(str);*/
            else
            {
                Console.WriteLine("Default : value is string");
                valueSpec = new LiteralString(str);
            }
            return valueSpec;

        }

        public MascaretPrimitiveType(string name)
            : base(name)
        {
            type = name;
        }

    }
}

