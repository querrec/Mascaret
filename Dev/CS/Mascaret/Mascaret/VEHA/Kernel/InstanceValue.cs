using System;

using System.Collections.Generic;

namespace Mascaret
{
    public class InstanceValue : ValueSpecification
    {
        // --- Attributes ---

        protected InstanceSpecification specValue;
        public InstanceSpecification SpecValue
        {
            get { return specValue; }
            set { specValue = value; }
        }

        public InstanceValue(InstanceSpecification instance)
            : base(instance.Classifier)
        {
            specValue = instance;
            CLASSTYPE = "Instance";
        }

        public InstanceValue(Classifier type)
            : base(type)
        {
            CLASSTYPE = "Instance";
            specValue = null;
        }

        public override string getStringFromValue()
        {
            if (specValue != null)
                return specValue.name;
            else return "None";
        }

        public override ValueSpecification clone()
        {
            InstanceValue vs = new InstanceValue(this.Type);
            vs.specValue = specValue;
            return (ValueSpecification)vs;
        }
    }
}

