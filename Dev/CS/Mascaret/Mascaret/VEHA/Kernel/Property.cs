using System;
using System.IO;
using System.Collections.Generic;

namespace Mascaret
{
    public class Property : Feature
    {

        private Classifier type = null;
        public Classifier Type
        {
            get { return type; }
            set { type = value; }
        }


        private Classifier classifier = null;
        public Classifier Classifier
        {
            get { return classifier; }
            set { classifier = value; }
        }


        private bool isComposite = false;
        public bool IsComposite
        {
            get { return isComposite; }
            set { isComposite = value; }
        }


        private bool isNavigable = false;
        public bool IsNavigable
        {
            get { return isNavigable; }
            set { isNavigable = value; }
        }


        private bool isReadOnly = false;
        public bool IsReadOnly
        {
            get { return isReadOnly; }
            set { isReadOnly = value; }
        }


        private bool isDerived = false;
        public bool IsDerived
        {
            get { return isDerived; }
            set { isDerived = value; }
        }


        private bool isOrdered = false;
        public bool IsOrdered
        {
            get { return isOrdered; }
            set { isOrdered = value; }
        }


        private bool isUnique = false;
        public bool IsUnique
        {
            get { return isUnique; }
            set { isUnique = value; }
        }


        private int minimumNumber = 0;
        public int MinimumNumber
        {
            get { return minimumNumber; }
            set { minimumNumber = value; }
        }


        private int maximumNumber = 1;
        public int MaximumNumber
        {
            get { return maximumNumber; }
            set { maximumNumber = value; }
        }


        private Property opposite = null;
        public Property Opposite
        {
            get { return opposite; }
            set { opposite = value; }
        }


        private ValueSpecification defaultValue = null;
        public ValueSpecification DefaultValue
        {
            get { return defaultValue; }
            set { defaultValue = value; }
        }


        private DomainSpecification domain = null;
        public DomainSpecification Domain
        {
            get { return domain; }
            set { domain = value; }
        }

        public override string getFullName()
        {
            return type.getFullName() + "." + name;
        }

        //Default Parameters opposite = null ,ValueSpecification def= ,null,DomainSpecification domain=null
        public Property(string name, Class cl, Classifier type, Property opposite, ValueSpecification def, DomainSpecification domain)
            : base(name)
        {
            this.type = type;
            this.classifier = cl;
            this.opposite = opposite;
            this.defaultValue = null;
            this.domain = domain;
            this.isComposite = false;
            this.isNavigable = false;
            this.isReadOnly = false;
            this.isDerived = false;
            this.isUnique = false;
            this.isOrdered = false;
            this.minimumNumber = 0;
            this.maximumNumber = 1;

            if (def != null)
                this.defaultValue = def;
            else
            {
                string typeName = type.name;
                if (typeName == "real")
                {
                    this.defaultValue = new LiteralReal();
                }
                else if (typeName == "integer")
                {
                    this.defaultValue = new LiteralInteger();
                }
                else if (typeName == "string")
                {
                    this.defaultValue = new LiteralString();
                }
                else if (typeName == "boolean")
                {
                    this.defaultValue = new LiteralBoolean();
                }
                else if (typeName == "bool")
                {
                    this.defaultValue = new LiteralBoolean();
                }
            }

        }

        public ValueSpecification createValueFromString(string str)
        {
           // MascaretApplication.Instance.logfile.WriteLine("Property::createValue : " + str); MascaretApplication.Instance.logfile.Flush();
            return type.createValueFromString(str);
        }

    }
}