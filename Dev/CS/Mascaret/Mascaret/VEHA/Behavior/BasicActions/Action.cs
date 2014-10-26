
using System;
using System.Collections.Generic;


namespace Mascaret
{
    public abstract class Action : Behavior
    {

        private string kind;
        public string Kind
        {
            get { return kind; }
            set { kind = value; }
        }


        private List<InputPin> inputPins;
        public List<InputPin> InputPins
        {
            get { return inputPins; }
            set { inputPins = value; }
        }


        private List<OutputPin> outputPins;
        public List<OutputPin> OutputPins
        {
            get { return outputPins; }
            set { outputPins = value; }
        }


        private List<ValuePin> valuePins;
        public List<ValuePin> ValuePins
        {
            get { return valuePins; }
            set { valuePins = value; }
        }

        public Action()
            : base("BasicAction")
        {
            outputPins = new List<OutputPin>();
            inputPins = new List<InputPin>();
            valuePins = new List<ValuePin>();
        }

        //default parameters sync = false
        //public abstract BehaviorExecution createBehaviorExecution (InstanceSpecification host, MapStringValueSpecification p, bool sync);
    }
}