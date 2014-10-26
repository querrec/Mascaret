
using System;
using System.Collections.Generic;


namespace Mascaret
{
    public class State : Vertex
    {

        private bool isSimple = true;
        public bool IsSimple
        {
            get { return isSimple; }
            set { isSimple = value; }
        }


        private List<Region> regions;
        public List<Region> Regions
        {
            get { return regions; }
            set { regions = value; }
        }

        public bool isComposite()
        {
            return regions.Count >= 1;
        }

        public bool isOrthogonal()
        {
            return regions.Count >= 2;
        }


        private Operation doBehavior = null;
        public Operation DoBehavior
        {
            get { return doBehavior; }
            set { doBehavior = value; }
        }


        private Operation entryBehavior = null;
        public Operation EntryBehavior
        {
            get { return entryBehavior; }
            set { entryBehavior = value; }
        }


        private Operation exitBehavior;
        public Operation ExitBehavior
        {
            get { return exitBehavior; }
            set { exitBehavior = value; }
        }


        private BehaviorExecution currentExecution;
        public BehaviorExecution CurrentExecution
        {
            get { return currentExecution; }
            set { currentExecution = value; }
        }


        private StateMachine submachine;
        public StateMachine Submachine
        {
            get { return submachine; }
            set { submachine = value; }
        }


        private bool isSubMachineState = false;
        public bool IsSubMachineState
        {
            get { return isSubMachineState; }
            set { isSubMachineState = value; }
        }


        public State(string name, string kind)
            : base(name)
        {
        }

        public override BehaviorExecution activate(InstanceSpecification entity, Dictionary<string, ValueSpecification> param)
        {
            BehaviorExecution be = null;
            if (isSubMachineState)
            {
                be = BehaviorScheduler.Instance.executeBehavior(submachine, entity, param, false);
            }
            else if (!isSimple)
            {
                System.Console.WriteLine("!!!!!!!!!!! SUBREGION !!!!!!!");
            }

            System.Console.WriteLine(" Running State : " + name + " : " + DoBehavior);

            if (doBehavior != null)
            {
                be = BehaviorScheduler.Instance.executeBehavior(doBehavior.Method, entity, param, false);
            }

            return be;
        }

        public new void simulate(InstanceSpecification entity)
        {
            BehaviorExecution be = null;
            if (isSubMachineState)
            {

                be = BehaviorScheduler.Instance.executeBehavior(submachine, entity, new Dictionary<string, ValueSpecification>(), false);
            }
            else if (!isSimple)
            {
                System.Console.WriteLine("!!!!!!!!!!! SUBREGION !!!!!!!");
            }

            if (doBehavior != null)
            {
                be = BehaviorScheduler.Instance.executeBehavior(doBehavior.Method, entity, new Dictionary<string, ValueSpecification>(), true);
            }
        }

        public override void desactivate()
        {

        }

    }
}