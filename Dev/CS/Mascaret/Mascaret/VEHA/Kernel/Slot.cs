using System;
using System.IO;
using System.Collections.Generic;


namespace Mascaret
{
    public class Slot : Element
    {


        private Property definingProperty = null;
        public Property DefiningProperty
        {
            get { return definingProperty; }
            set { definingProperty = value; }
        }

        //[Serializable]
        //public class MapStringValueSpecification : Map<string,ValueSpecification>{};

        private Dictionary<string, ValueSpecification> values;
        public Dictionary<string, ValueSpecification> Values
        {
            get { return values; }
            set { values = value; }
        }

        public void _addValue(ValueSpecification element)
        {
            if (definingProperty.MaximumNumber == 1 && values.Keys.Count == 1)
            {
                values.Clear();
            }
            else if (values.Keys.Count > definingProperty.MaximumNumber && definingProperty.MaximumNumber != -1)
            {
                System.Console.WriteLine("Slot::addValue Error : " + definingProperty.name + " number of value is " + values.Keys.Count + " maximum is " + definingProperty.MaximumNumber);
            }
            if (element != null) values.Add(element.getStringFromValue(), element);
            //calbakcs functions call
        }

        public bool removeValue(string name)
        {
            Slot curSlot = _getOppositeSlot(name);
            if (_canRemoveValue() && !(definingProperty.Opposite != null && curSlot != null && !curSlot._canRemoveValue()))
            {
                if (definingProperty.Opposite != null)
                    curSlot.removeValue(owningInstance.name);
                //_removeValue(name); fonction utilisant les callBack
                return true;
            }
            else
                return false;
        }


        private InstanceSpecification owningInstance;
        public InstanceSpecification OwningInstance
        {
            get { return owningInstance; }
            set { owningInstance = value; }
        }

        public Slot()
            : base("")
        {
            values = new Dictionary<string, ValueSpecification>();
        }

        public Slot(InstanceSpecification instance, Property property)
            : base(property.name)
        {

            values = new Dictionary<string, ValueSpecification>();
            this.owningInstance = instance;
            this.definingProperty = property;
            if (property.DefaultValue != null) addValue(property.DefaultValue);
        }

        public void addValueFromString(string str)
        {
            addValue(definingProperty.createValueFromString(str));
        }

        public void addValue(ValueSpecification val)
        {
            /*
            Property oppositeProp=null;
            if(definingProperty.IsNavigable)
                oppositeProp = definingProperty.Opposite;
            if(oppositeProp!=null)
            {
                InstanceSpecification test = val.valueSpecificationToInstanceSpecification();
                InstanceValue curInstanceValue = (InstanceValue)ScriptableObject.CreateInstance("InstanceValue");
                curInstanceValue.instantiate(owningInstance);
                test.getProperty(oppositeProp.name).addValue((ValueSpecification)curInstanceValue);
            }*/
            _addValue(val);

            //backupFunction

        }

        public void replaceValueFromString(string old, string newValue)
        {
            _removeValue(old);
            if (definingProperty.IsNavigable && definingProperty.Opposite != null)
                _getOppositeSlot(old)._removeValue(owningInstance.name);
            addValueFromString(newValue);
        }

        public void replaceValue(string old, ValueSpecification newValue)
        {
            _removeValue(old);
            if (definingProperty.IsNavigable && definingProperty.Opposite != null)
                _getOppositeSlot(old)._removeValue(owningInstance.name);
            addValue(newValue);
        }

        protected void _update()
        { }

        protected bool _canRemoveValue()
        {
            return (values.Keys.Count > definingProperty.MinimumNumber);
        }

        protected Slot _getOppositeSlot(string name)
        {
            if (values.ContainsKey(name))
            {
                return (values[name].valueSpecificationToInstanceSpecification()).getProperty(definingProperty.Opposite.name);
            }
            else
                return null;
        }

        protected void _removeValue(string name)
        {
            //callBakcs functions
            if (values.ContainsKey(name))
                values.Remove(name);
        }

        public ValueSpecification getValue()
        {
            //callbackMethod
            _update();
            if (values.Keys.Count == 1)
            {
                foreach (ValueSpecification valueSpec in values.Values)
                {
                    return valueSpec;
                }
            }

            return null;
        }

        public T getValue<T>()
        {
            //callbackMethod
            _update();

            ValueSpecification vSpec = null;
            if (values.Count == 1)
                foreach (ValueSpecification valueSpec in values.Values)
                    vSpec = valueSpec;
            
            // Check if value exist
            if (vSpec == null)
                return default(T);

            // Check if value is a Litteral Specification
            if (vSpec.GetType().IsSubclassOf(typeof(LiteralSpecification)))
            {
                // The value exist now get the Classifier
                Type type = typeof(T);
                IConvertible value = null;
                if (type.Equals(typeof(System.Boolean)))
                    value = vSpec.getBoolFromValue();
                else if (type.Equals(typeof(System.Int32)))
                    value = vSpec.getIntFromValue();
                else if (type.Equals(typeof(System.Double)) || type.Equals(typeof(System.Single)))
                    value = vSpec.getDoubleFromValue();
                else
                    value = vSpec.getStringFromValue();

                return (T)value;
            }
            else
                throw new Exception("Can not use this method with this kind of Value");
        }

        public InstanceSpecification getInstanceValue()
        {
            //callbackMethod
            _update();

            ValueSpecification vSpec = null;
            if (values.Count == 1)
                foreach (ValueSpecification valueSpec in values.Values)
                    vSpec = valueSpec;

            // Check if value exist
            if (vSpec == null)
                return null;

            if (vSpec.GetType().IsSubclassOf(typeof(LiteralSpecification)))
                throw new Exception("Can not use this method with this kind of parameter !");

            InstanceValue iValue = (InstanceValue)vSpec;
            return iValue.SpecValue;
        }

        public Dictionary<string, T> getValues<T> ()
        {
            //callbackMethod
            _update();

            Dictionary<string, T> dict = new Dictionary<string, T>();

            foreach (KeyValuePair<string, ValueSpecification> pair in values)
            {
                // Check if value is a Litteral Specification
                ValueSpecification vSpec = pair.Value;
                if (vSpec.GetType().IsSubclassOf(typeof(LiteralSpecification)))
                {
                    // The value exist now get the Classifier
                    Type type = typeof(T);
                    IConvertible value = null;
                    if (type.Equals(typeof(System.Boolean)))
                        value = vSpec.getBoolFromValue();
                    else if (type.Equals(typeof(System.Int32)))
                        value = vSpec.getIntFromValue();
                    else if (type.Equals(typeof(System.Double)) || type.Equals(typeof(System.Single)))
                        value = vSpec.getDoubleFromValue();
                    else
                        value = vSpec.getStringFromValue();

                    dict.Add(pair.Key, (T)value);
                }
            }

            return dict;
        }

        public Dictionary<string, InstanceSpecification> getInstanceValues()
        {
            //callbackMethod
            _update();

            Dictionary<string, InstanceSpecification> dict = new Dictionary<string, InstanceSpecification>();

            foreach (KeyValuePair<string, ValueSpecification> pair in values)
            {
                InstanceValue iValue = (InstanceValue)pair.Value;
                dict.Add(pair.Key, iValue.SpecValue);
            }

            return dict;
        }
    }

}
