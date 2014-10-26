using System;
using System.Collections.Generic;

//sans implémentation des methodes liées au callback pour le moment
namespace Mascaret
{
    public class InstanceSpecification : Element
    {


        private Class classifier = null;
        public Class Classifier
        {
            get { return classifier; }
            set { classifier = value; }
        }


        private Dictionary<String, Slot> slots;
        public Dictionary<String, Slot> Slots
        {
            get { return slots; }
            set { slots = value; }
        }

        public void addSlot(Property element)
        {
            System.Console.WriteLine("Adding Slot : " + element.getFullName());
            Slot curSlot = new Slot(this, element);
            slots.Add(element.name, curSlot);
        }

        public void removeSlot(string name)
        {
            if (slots.ContainsKey(name))
                slots.Remove(name);
        }


        private List<StateMachineBehaviorExecution> smBehaviorExecution;
        public List<StateMachineBehaviorExecution> SmBehaviorExecutions
        {
            get { return smBehaviorExecution; }
            set { smBehaviorExecution = value; }
        }


        private List<BehaviorExecution> operationsExecution;
        public List<BehaviorExecution> OperationsExecution
        {
            get { return operationsExecution; }
            set { operationsExecution = value; }
        }

        public InstanceSpecification(string name, Class classe)
            : base(name)
        {
            //Debug.Log("InstanceSpecification::instanciate : " + name + " : " + classe.getFullName());
            this.classifier = classe;
            if (this.classifier != null)
            {
                if (this.classifier.Instances.ContainsKey(this.name)) this.classifier.Instances.Remove(this.name);
                this.classifier.Instances.Add(this.name, this);
                slots = new Dictionary<string, Slot>();
                smBehaviorExecution = new List<StateMachineBehaviorExecution>();
                createInstanceFromClass();
            }
        }

        public void rename(string newName)
        {
            if (Classifier != null)
                Classifier.restockInstance((InstanceSpecification)this, newName);
        }

        public override string getFullName()
        {
            return classifier.getFullName() + "::" + name;
        }

        public void checkSlots()
        {
            int modif = 0;
            foreach (Slot slot in slots.Values)
            {
                foreach (KeyValuePair<string, ValueSpecification> pair in slot.Values)
                {
                    if (pair.Key != pair.Value.getStringFromValue())
                    {
                        ValueSpecification val = pair.Value.clone();
                        slot.removeValue(pair.Key);
                        slot.addValue(val);
                        modif++;
                    }
                }
            }

            System.Console.WriteLine("Slot modifications : " + modif);

        }

        public bool isActiveState(string state)
        {
            for (int i = smBehaviorExecution.Count; i >= 0; i--)
            {
                Vertex v = smBehaviorExecution[i].CurrentState;
                if (v != null && v.name == state) return true;
            }
            return false;
        }


        public void createInstanceFromClass()
        {
            if (Classifier != null)
            {
                //Debug.Log("CreateInstanceFromClass");
                //foreach(KeyValuePair<string,Property> prop in Classifier.Attributes.convertToDictionary())
                //Debug.Log(Classifier.Attributes.KeysList.Count);
                foreach (KeyValuePair<string, Property> prop in Classifier.Attributes)
                {
                    Slot s;
                    //Debug.Log("Adding Slot : " + prop.Key);
                    if (prop.Value.IsDerived)
                    {
                        s = new SlotDerived(this, prop.Value);
                    }
                    else
                    {
                        s = new Slot(this, prop.Value);
                    }

                    slots.Add(prop.Key, s);
                }
            }
        }

        public Slot getProperty(string name)
        {
            if (slots.ContainsKey(name))
                return slots[name];
            else
                return null;
        }

        public BehaviorExecution executeOperation(string name, List<ValueSpecification> p)
        {
            BehaviorExecution be = null;
            if (Classifier != null)
            {
                Operation op;
                if (Classifier.Operations.ContainsKey(name))
                {
                    op = Classifier.Operations[name];
                    Dictionary<string, ValueSpecification> behaviorParameters = new Dictionary<string, ValueSpecification>();
                    if (!op.createBehaviorParameters(p, behaviorParameters))
                    {
                        System.Console.WriteLine("Parameters are not correct");
                        return be;
                    }

                    Behavior b = op.Method;
                    if (b != null)
                    {
                        be = BehaviorScheduler.Instance.executeBehavior(b, this, behaviorParameters, false);
                        //be.addCallBackOnBehaviorStop();
                    }
                }
            }
            if (be != null)
                operationsExecution.Add(be);
            return be;
        }

        public bool stopOperation(string name)
        {
            foreach (BehaviorExecution be in operationsExecution)
            {
                if (be.Specification.name.CompareTo(name) == 0)
                {
                    be.stop();
                    return true;
                }
            }
            return false;
        }

        public bool suspendOperation(string name)
        {
            foreach (BehaviorExecution be in operationsExecution)
            {
                if (be.Specification.name.CompareTo(name) == 0)
                {
                    be.pause();
                    return true;
                }
            }
            return false;

        }

        public bool restartoperation(string name)
        {
            foreach (BehaviorExecution be in operationsExecution)
            {
                if (be.Specification.name.CompareTo(name) == 0)
                {
                    be.restart();
                    return true;
                }
            }
            return false;

        }

        protected void onBehaviorStopped(BehaviorExecution be)
        {
            if (operationsExecution.Contains(be))
                operationsExecution.Remove(be);
        }

        public void print()
        {
            System.Console.WriteLine(name + " : " + this.Classifier.name);
        }

    }
}
