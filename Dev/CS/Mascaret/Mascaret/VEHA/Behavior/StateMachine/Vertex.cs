using System;
using System.Collections.Generic;

namespace Mascaret
{
    public abstract class Vertex : Element
    {

        public bool running = false;


        private List<Transition> outgoing = new List<Transition>();
        public List<Transition> Outgoing
        {
            get { return outgoing; }
            set { outgoing = value; }
        }


        private List<Transition> incoming = new List<Transition>();
        public List<Transition> Incoming
        {
            get { return incoming; }
            set { incoming = value; }
        }


        private bool noninterpt = false;
        public bool Noninterpt
        {
            get { return noninterpt; }
            set { noninterpt = value; }
        }

        public Vertex(string name)
            : base(name)
        {
        }

        public void simulate(InstanceSpecification host)
        {

        }

        public abstract void desactivate();
        public abstract BehaviorExecution activate(InstanceSpecification host, Dictionary<string, ValueSpecification> p);

    }

}