
using System;
using System.Collections.Generic;


namespace Mascaret
{
    public class StateMachine : Behavior
    {


        private List<Region> region = new List<Region>();
        public List<Region> Region
        {
            get { return region; }
            set { region = value; }
        }


        private List<PseudoState> connectionPoint = new List<PseudoState>();
        public List<PseudoState> ConnectionPoint
        {
            get { return connectionPoint; }
            set { connectionPoint = value; }
        }


        private List<State> submachinestates = new List<State>();
        public List<State> Submachinestates
        {
            get { return submachinestates; }
            set { submachinestates = value; }
        }

        public StateMachine(string name)
            : base(name)
        {
        }

        public override BehaviorExecution createBehaviorExecution(InstanceSpecification host, Dictionary<string, ValueSpecification> p, bool sync)
        {
            StateMachineBehaviorExecution behavior = new StateMachineBehaviorExecution(this, host, p, sync);
            host.SmBehaviorExecutions.Add(behavior);
            return behavior;
        }

    }
}