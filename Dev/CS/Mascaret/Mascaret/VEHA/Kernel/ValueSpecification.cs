using System;
using System.Collections.Generic;


namespace Mascaret
{
    public abstract class ValueSpecification
    {

        private Classifier type = null;
        public Classifier Type
        {
            get { return type; }
            set { type = value; }
        }

        public string CLASSTYPE;

        public ValueSpecification(Classifier type)
        {
            this.type = type;
            CLASSTYPE = "ValueSpecification";
        }

        public abstract ValueSpecification clone();

        public bool getIntValue(int val)
        {
            return false;
        }

        public bool getDoubleValue(int val)
        {
            return false;
        }

        public bool getStringValue(int val)
        {
            return false;
        }

        public bool getBoolValue(int val)
        {
            return false;
        }

        public string getTypeName()
        {
            if (type != null)
                return type.name;
            else
                return "undef";
        }

        public virtual string getStringFromValue()
        {
            return "";
        }

        public virtual bool getBoolFromValue()
        {
            return false;
        }

        public virtual int getIntFromValue()
        {
            return 0;
        }

        public virtual double getDoubleFromValue ()
        {
            return 0f;
        }

        public InstanceSpecification valueSpecificationToInstanceSpecification()
        {
            InstanceSpecification curInstanceSpec = new InstanceSpecification("", (Class)type);
            return curInstanceSpec;
        }
    }
}
