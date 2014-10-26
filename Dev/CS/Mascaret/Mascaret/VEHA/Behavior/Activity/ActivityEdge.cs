
using System;
using System.Collections.Generic;

namespace Mascaret
{
    public class ActivityEdge : Element
    {

        private ActivityNode target;
        public ActivityNode Target
        {
            get { return target; }
            set { target = value; }
        }


        private ActivityNode source;
        public ActivityNode Source
        {
            get { return source; }
            set { source = value; }
        }


        private Constraint guard;
        public Constraint Guard
        {
            get { return guard; }
            set { guard = value; }
        }


        private List<ActivityPartition> partitions = new List<ActivityPartition>();
        public List<ActivityPartition> Partitions
        {
            get { return partitions; }
            set { partitions = value; }
        }


        private Activity activity;
        public Activity Activity
        {
            get { return activity; }
            set { activity = value; }
        }


        private string kind;
        public string Kind
        {
            get { return kind; }
            set { kind = value; }
        }

        public ActivityEdge(string name)
            : base(name)
        {
        }

    }
}
