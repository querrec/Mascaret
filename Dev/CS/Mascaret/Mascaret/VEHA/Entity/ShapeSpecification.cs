
using System;
using System.Collections.Generic;

namespace Mascaret
{
    public abstract class ShapeSpecification : VirtualRealitySpecification
    {
        public string name;

        private string url;
        public string Url
        {
            get { return url; }
        }

        public ShapeSpecification(string name)
            : base(MascaretApplication.Instance.Model.getBasicType("shape"))
        {
            this.name = name;
        }

        //Default parameters movable=true, recursive =false, shader=""
        public ShapeSpecification(string name, string url, bool movable, bool recursive, string shader)
            : base(MascaretApplication.Instance.Model.getBasicType("shape"))
        {
            this.name = name;
            this.url = url;
        }

        //Default parameters movable=true, recursive =false, shader=""
        public ShapeSpecification(string name, List<double> distances, List<string> urls, bool movable, bool recursive, string shader)
            : base(MascaretApplication.Instance.Model.getBasicType("shape"))
        {
            this.name = name;
            if (urls.Count > 0)
            {
                this.url = urls[0];
            }
        }

        public abstract void setVisibility(bool v);
        public abstract bool getVisibility();

        public abstract void setScale(Vector3 scale);
        public abstract Vector3 getScale();


        public abstract void setEntity(Entity entity);
        public abstract Entity getEntity();

        // --- Shape Modifier Operations ---
        public abstract Color getColor();
        public abstract void setColor(Color color);
        public abstract void setTransparent(Color color);
        public abstract void growUp(float percent);
        public abstract bool restaureColor(Color color);
        public abstract bool restaureShape();

        public abstract double prepareSpeak(string text);
        public abstract bool speak(string text);

        public abstract double playAnimation(string animationName);

        // --- Operations ---
        public override ValueSpecification clone()
        {
            return null;
        }
    }
}

