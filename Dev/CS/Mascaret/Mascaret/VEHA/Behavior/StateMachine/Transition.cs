
using System;
using System.Collections.Generic;


namespace Mascaret
{
    public class Transition : Element
    {

        private List<Trigger> trigger;
        public List<Trigger> Trigger
        {
            get { return trigger; }
            set { trigger = value; }
        }


        private Action effect = null;
        public Action Effect
        {
            get { return effect; }
            set { effect = value; }
        }


        private Vertex source;
        public Vertex Source
        {
            get { return source; }
            set { source = value; }
        }


        private Vertex target;
        public Vertex Target
        {
            get { return target; }
            set { target = value; }
        }


        private Constraint guard;
        public Constraint Guard
        {
            get { return guard; }
            set { guard = value; }
        }

        public Transition()
            : base("")
        {
            this.trigger = new List<Trigger>();
        }


    }
}