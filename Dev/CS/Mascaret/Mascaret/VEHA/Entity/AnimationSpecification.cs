using System;
using System.Collections.Generic;

namespace Mascaret
{
    public abstract class AnimationSpecification : VirtualRealitySpecification
    {
        public string name;

        private string url;
        public string Url
        {
            get { return url; }
            set { url = value; }
        }

        //callbacks missing

        public AnimationSpecification(string name, string url)
            : base(MascaretApplication.Instance.Model.getBasicType("animation"))
        {
            this.name = name;
            this.url = url;
        }

        public override ValueSpecification clone()
        {
            throw new NotImplementedException();
        }

        //callbakcs operation

        public abstract void setShape(ShapeSpecification shape);
        public abstract ShapeSpecification getShape();
        //Default parameters : speed = 1.0 sens = 1 cycle = false
        public abstract bool play(double speed, int sens, bool cycle);
        public abstract bool stop();
    }
}


