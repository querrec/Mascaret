using System;
using System.Collections.Generic;


namespace Mascaret
{
    public class ActivityPartition : Element, IEquatable<ActivityPartition>
    {


        private Role role;
        public Role Role
        {
            get { return role; }
            set { role = value; }
        }


        private List<ActivityEdge> edge = new List<ActivityEdge>();
        public List<ActivityEdge> Edge
        {
            get { return edge; }
            set { edge = value; }
        }


        private List<ActivityNode> node = new List<ActivityNode>();
        public List<ActivityNode> Node
        {
            get { return node; }
            set { node = value; }
        }


        private Activity activity;
        public Activity Activity
        {
            get { return activity; }
            set { activity = value; }
        }

        public ActivityPartition(string name)
            : base(name)
        {
        }

        public bool Equals(ActivityPartition other)
        {
            return (this == other);
        }

    }
}
