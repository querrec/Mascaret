using System;
using System.Collections.Generic;

namespace Mascaret
{
    public abstract class PointSpecification : TopologicalSpecification
    {

        private PointSpecification parent = null;
        public PointSpecification Parent
        {
            get { return parent; }
            set { parent = value; }
        }

        public PointSpecification()
            : base(MascaretApplication.Instance.Model.getBasicType("point"))
        {
        }

        public override ValueSpecification clone()
        {
            //	PointSpecification p = new UnityPointSpecification();//getGlobalPosition(),getGlobalRotation());
            //	return p;

            return null;
        }

        public abstract Vector3 getLocalPosition();
        public abstract void setLocalPosition(Vector3 pos);
        public abstract Vector3 getGlobalPosition();
        public abstract void setGlobalPosition(Vector3 pos);

        public abstract void translate(Vector3 pos);

        public abstract RotationVector getLocalRotation();
        public abstract void setLocalRotation(RotationVector orientation);
        public abstract RotationVector getGlobalRotation();
        public abstract void setGlobalRotation(RotationVector orientation);
        public abstract void rotate(RotationVector orientation);
        public abstract void rotate(Vector3 orientation);

        public abstract void setLocalOrientation(Vector3 ori);
        public abstract Vector3 getLocalOrientation();
        public abstract void setGlobalOrientation(Vector3 ori);
        public abstract Vector3 getGlobalOrientation();

        public abstract void globalToLocalLocation(PointSpecification pointInOut);
        public abstract void localToGlobalLocation(PointSpecification pointInOut);

        public abstract void globalToLocalPosition(Vector3 posInOut);
        public abstract void localToGlobalPosition(Vector3 posInOut);

        public abstract void localToGlobalOrientation(Vector3 oriInOut);
        public abstract void globalToLocalOrientation(Vector3 oriInOut);
    }
}

