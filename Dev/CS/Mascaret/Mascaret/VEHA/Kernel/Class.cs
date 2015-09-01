using System;
using System.Collections.Generic;

//gestion des callBacks (fireCBOnCreate, addCallbackOnRemove ... non implémentées pour le moment)

namespace Mascaret
{
    public class Class : Classifier
    {


        private Package package = null;
        public Package Package
        {
            get { return package; }
            set { package = value; }
        }

        protected List<Connector> connectors = new List<Connector>();
        public List<Connector> getConnectors()
        {
            return connectors;
        }
        public void addConnector(Connector connector)
        {
            connectors.Add(connector);
        }

        private Dictionary<String, Property> attributes;
        public Dictionary<String, Property> Attributes
        {
            get { return attributes; }
            set { attributes = value; }
        }

        public bool addAttribute(Property element)
        {
            if (attributes.ContainsKey(element.name)) return false;
            foreach (Classifier child in Children)
            {
                ((Class)child).addAttribute(element);
            }
            foreach (InstanceSpecification instance in instances.Values)
            {
                instance.addSlot(element);
            }
            attributes.Add(element.name, element);
            return true;
        }

        public bool removeAttribute(string name)
        {
            if (attributes.ContainsKey(name))
            {
                Property val = attributes[name];
                Property parentVal = null;
                foreach (Classifier parent in Parents)
                {
                    Class c = (Class)parent;
                    if (c.Attributes.ContainsKey(name))
                    {
                        parentVal = c.Attributes[name];
                        if (val == parentVal)
                            return false;
                        else break;
                    }
                }

                Attributes.Remove(name);

                foreach (InstanceSpecification instance in instances.Values)
                {
                    instance.removeSlot(name);
                }

                foreach (Classifier child in Children)
                {
                    Class c = (Class)child;
                    Property otherVal;
                    if (c.Attributes.ContainsKey(name))
                    {
                        otherVal = c.Attributes[name];
                        if (otherVal == val)
                            c.removeAttribute(name);
                    }
                }

                if (parentVal != null)
                    addAttribute(parentVal);
                return true;

            }
            else
                return false;
        }

        public virtual bool hasAttribute(string name)
        {
            return attributes.ContainsKey(name);
        }


        private Dictionary<String, Operation> operations;
        public Dictionary<String, Operation> Operations
        {
            get { return operations; }
            set { operations = value; }
        }

        public bool addOperation(Operation element)
        {
            if (operations.ContainsKey(element.name)) return false;
            foreach (Classifier child in Children)
            {
                ((Class)child).addOperation(element);
            }
            
            operations.Add(element.name, element);
            return true;
        }

        public bool removeOperation(string name)
        {
            if (operations.ContainsKey(name))
            {
                Operation val = operations[name];
                Operation parentVal = null;
                foreach (Classifier parent in Parents)
                {
                    Class c = (Class)parent;
                    if (c.Operations.ContainsKey(name))
                    {
                        parentVal = c.Operations[name];
                        if (val == parentVal)
                            return false;
                        else break;
                    }
                }

                Operations.Remove(name);

                foreach (Classifier child in Children)
                {
                    Class c = (Class)child;
                    Operation otherVal;
                    if (c.Operations.ContainsKey(name))
                    {
                        otherVal = c.Operations[name];
                        if (otherVal == val)
                            c.removeOperation(name);
                    }
                }

                if (parentVal != null)
                    addOperation(parentVal);
                return true;

            }
            else
                return false;
        }

        public virtual bool hasOperation(string name)
        {
            return operations.ContainsKey(name);
        }


        private Dictionary<String, InstanceSpecification> instances;
        public Dictionary<String, InstanceSpecification> Instances
        {
            get { return instances; }
            set { instances = value; }
        }

        public Class(string name)
            : base(name)
        {
            attributes = new Dictionary<string, Property>();
            instances = new Dictionary<string, InstanceSpecification>();
            operations = new Dictionary<string, Operation>();

        }

        public override string getFullName()
        {
            if (package != null)
                return package.getFullName() + "::" + name;
            else
                return name;
        }

        public override ValueSpecification createValueFromString(string str)
        {
            Dictionary<string, InstanceSpecification> instances = getInstances(true);
            //Debug.Log (" Class :: CreateValueFromString : " + getFullName() + " / " + str +" : " + instances.Count);
            InstanceValue curInstanceValue = new InstanceValue((InstanceSpecification)(instances[str]));

            return (ValueSpecification)curInstanceValue;
        }

        public void restockInstance(InstanceSpecification specValue, string newName)
        {
            instances.Remove(specValue.name);
            instances.Add(newName, specValue);
        }

        public Dictionary<string, InstanceSpecification> getInstances(bool searchInSubclass)
        {
            Dictionary<string, InstanceSpecification> result = new Dictionary<string, InstanceSpecification>();
            foreach (KeyValuePair<string, InstanceSpecification> p in instances)
            {
                result.Add(p.Key, p.Value);
            }
            if (searchInSubclass)
            {
                for (int i = 0; i < Children.Count; i++)
                {
                    foreach (KeyValuePair<string, InstanceSpecification> r in ((Class)Children[i]).getInstances(true))
                    {
                        result.Add(r.Key, r.Value);
                    }
                }
            }
            return result;
        }

        protected override void _addFeaturesToChild(Classifier child)
        {
            Class c = (Class)child;
            foreach (Property p in attributes.Values)
            {
                c.addAttribute(p);
            }
            /*
            foreach(Operation op in operations.ValuesList)
            {
                c.addOperation(op);
            }*/
        }

        public InstanceSpecification getInstance(string name, bool searchinSubclass)
        {
            InstanceSpecification result;
            if (instances.ContainsKey(name))
                return instances[name];
            else
            {
                if (searchinSubclass)
                {
                    foreach (Classifier child in Children)
                    {
                        result = ((Class)child).getInstance(name, searchinSubclass);
                        if (result != null)
                            return result;
                    }
                }
            }
            return null;
        }

        public void print()
        {
            System.Console.WriteLine(getFullName());

            if (OwnedBehavior != null) System.Console.WriteLine("Behaviors : " + OwnedBehavior.Keys.Count);
            /*
            for (int i = 0; i < OwnedBehavior.KeysList.Count; i++)
                Debug.Log(OwnedBehavior.KeysList[i]);
                */
        }


    }
}
