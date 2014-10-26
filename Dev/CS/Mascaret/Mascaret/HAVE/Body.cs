
using System;
using System.Collections.Generic;


namespace Mascaret
{
    public class Body : Entity
    {


        private EmbodiedAgent agent;
        public EmbodiedAgent Agent
        {
            get { return agent; }
            set { agent = value; }
        }

        public double prepareSpeak(string text)
        {
            return this.ActiveShape.prepareSpeak(text);
        }

        public bool speak(string text)
        {
            return this.ActiveShape.speak(text);
        }

        public Body(EmbodiedAgent ea)
            : base("body of " + ea.name, (EntityClass)((VirtualHumanClass)ea.Classifier).BodyClass)
        {
            agent = ea;
            //ActiveShape = (UnityShapeSpecification)ScriptableObject.CreateInstance("UnityShapeSpecification");
            //ActiveShape.instantiate(ea.name,ea.name,true,false,"");

            //face = (Face)ScriptableObject.CreateInstance("Face");
            //face.instantiate("Face of " + this.name, this);

        }

    }
}



