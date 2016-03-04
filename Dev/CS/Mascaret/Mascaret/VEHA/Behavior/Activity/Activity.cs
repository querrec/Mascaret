using System;
using System.Collections.Generic;

namespace Mascaret
{
    public class Activity : Behavior
    {

        private List<ActivityEdge> edges = new List<ActivityEdge>();
        public List<ActivityEdge> Edges
        {
            get { return edges; }
            set { edges = value; }
        }

        public Dictionary<string, ValueSpecification> Context = new Dictionary<string,ValueSpecification>();


        private List<ActivityPartition> partitions = new List<ActivityPartition>();
        public List<ActivityPartition> Partitions
        {
            get { return partitions; }
            set { partitions = value; }
        }


        private List<ActivityNode> nodes = new List<ActivityNode>();
        public List<ActivityNode> Nodes
        {
            get { return nodes; }
            set { nodes = value; }
        }


        private ActivityNode initial;
        public ActivityNode Initial
        {
            get { return initial; }
            set { initial = value; }
        }

        public Activity(string name)
            : base(name)
        {
        }

        public void addNode(ActivityNode val)
        {
            val.Activity = this;
            nodes.Add(val);
        }

        public void addPartition(ActivityPartition val)
        {
            val.Activity = this;
            partitions.Add(val);
        }

        public void addEdge(ActivityEdge val)
        {
            val.Activity = this;
            edges.Add(val);
        }

        //default parameter sync = false
        public override BehaviorExecution createBehaviorExecution(InstanceSpecification host, Dictionary<string, ValueSpecification> p, bool sync)
        {
            ActivityBehaviorExecution be = new ActivityBehaviorExecution(this, host, p, sync);
            return be;
        }

    }
}
