using System;
using System.Collections.Generic;

namespace Mascaret
{
    public class ActivityNode : Element
    {

        private string kind;
        public string Kind
        {
            get { return kind; }
            set { kind = value; }
        }


        private List<ActivityEdge> incoming = new List<ActivityEdge>();
        public List<ActivityEdge> Incoming
        {
            get { return incoming; }
            set { incoming = value; }
        }


        private List<ActivityEdge> outgoing = new List<ActivityEdge>();
        public List<ActivityEdge> Outgoing
        {
            get { return outgoing; }
            set { outgoing = value; }
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


        private bool isRunning;
        public bool IsRunning
        {
            get { return isRunning; }
            set { isRunning = value; }
        }

        public ActivityNode(string name, string kind)
            : base(name)
        {
            this.kind = kind;
            isRunning = false;
        }

        public List<ActionNode> getIncomingActionNode()
        {
            List<ActionNode> nodes = new List<ActionNode>();
            foreach (ActivityEdge currentEdge in incoming)
            {
                if (currentEdge.Target != null && currentEdge.Target.Kind == "action")
                    nodes.Add((ActionNode)currentEdge.Target);
                else
                {
                    foreach (ActionNode currentNode in ((ActivityNode)currentEdge.Target).getIncomingActionNode())
                        nodes.Add(currentNode);
                }
            }
            return nodes;
        }

        public List<ObjectNode> getIncomingObjectNode()
        {
            List<ObjectNode> nodes = new List<ObjectNode>();
            foreach (ActivityEdge currentEdge in Incoming)
            {
                if (currentEdge.Source != null && currentEdge.Kind == "ObjectFlow")
                    nodes.Add((ObjectNode)currentEdge.Source);

            }
            return nodes;
        }

        public List<ActionNode> getIncomingActionNodeByPartition(string partitionName)
        {
            List<ActionNode> nodes = new List<ActionNode>();
            foreach (ActivityEdge currentEdge in incoming)
            {
                if (currentEdge.Target != null && currentEdge.Target.Kind == "action")
                {
                    if (currentEdge.Target.partitions != null && currentEdge.Target.partitions[0].name == partitionName)
                        nodes.Add((ActionNode)currentEdge.Target);
                }
                else
                {
                    foreach (ActionNode currentNode in currentEdge.Target.getIncomingActionNode())
                        nodes.Add(currentNode);
                }
            }
            return nodes;
        }

        public List<ActionNode> getOutgoingActionNode()
        {
            List<ActionNode> nodes = new List<ActionNode>();
            foreach (ActivityEdge currentEdge in outgoing)
            {
                //Debug.Log(" ---> " + currentEdge.Target.Kind);
                if (currentEdge.Target != null && currentEdge.Target.Kind == "action")
                {
                    nodes.Add((ActionNode)currentEdge.Target);
                    //Debug.Log(" Action : " + currentEdge.Target.getFullName());
                }
                else if (currentEdge.Target != null && currentEdge.Target.Kind == "loop")
                    nodes.Add((ActionNode)currentEdge.Target);
                else
                {
                    foreach (ActionNode currentNode in ((ActivityNode)currentEdge.Target).getOutgoingActionNode())
                        nodes.Add(currentNode);
                }
            }
            return nodes;
        }

        public List<ActionNode> getPossibleOutgoingActionNode()
        {
            List<ActionNode> nodes = new List<ActionNode>();
            foreach (ActivityEdge currentEdge in outgoing)
            {
                if (currentEdge.Target != null && currentEdge.Target.Kind == "action")
                    nodes.Add((ActionNode)currentEdge.Target);
                else
                {
                    foreach (ActionNode currentNode in currentEdge.Target.getOutgoingActionNode())
                        nodes.Add(currentNode);
                }
            }
            return nodes;
        }

        public List<ObjectNode> getOutgoingObjectNode()
        {
            List<ObjectNode> nodes = new List<ObjectNode>();
            foreach (ActivityEdge currentEdge in Outgoing)
            {
                if (currentEdge.Target != null && currentEdge.Kind == "object")
                    nodes.Add((ObjectNode)currentEdge.Target);

            }
            return nodes;
        }

        public List<ActionNode> getOutgoingActionNodeByPartition(string partitionName)
        {
            List<ActionNode> nodes = new List<ActionNode>();
            foreach (ActivityEdge currentEdge in outgoing)
            {
                if (currentEdge.Target != null && currentEdge.Target.Kind == "action")
                {
                    if (currentEdge.Target.partitions != null && currentEdge.Target.partitions[0].name == partitionName)
                        nodes.Add((ActionNode)currentEdge.Target);
                }
                else
                {
                    foreach (ActionNode currentNode in currentEdge.Target.getOutgoingActionNode())
                        nodes.Add(currentNode);
                }
            }
            return nodes;
        }

        public List<ActivityEdge> getOutgoingActionNodeEdges()
        {
            List<ActivityEdge> actionNodeEdges = new List<ActivityEdge>();
            foreach (ActivityEdge currentEdge in outgoing)
            {
                if (currentEdge.Target != null && currentEdge.Target.Kind == "action")
                    actionNodeEdges.Add(currentEdge);
                else if (currentEdge.Target != null && currentEdge.Target.Kind == "loop")
                    actionNodeEdges.Add(currentEdge);
                else
                {
                    foreach (ActivityEdge currentNode in ((ActivityNode)currentEdge.Target).getOutgoingActionNodeEdges())
                        actionNodeEdges.Add(currentNode);
                }
            }
            return actionNodeEdges;
        }

        public List<ActivityEdge> getOutgoingControlFlowEdges()
        {
            List<ActivityEdge> allOutgoing = this.Outgoing;
            List<ActivityEdge> controlFlows = new List<ActivityEdge>();
            for (int i = 0; i < allOutgoing.Count; ++i)
            {
                if (allOutgoing[i].Kind == "ControlFlow")
                    controlFlows.Add(allOutgoing[i]);
            }
            return controlFlows;
        }

    }
}
