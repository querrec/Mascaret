using System;
using System.Collections.Generic;


namespace Mascaret
{
    public class SendSignalBehaviorExecution : BehaviorExecution
    {


        private SendSignalAction action;
        public SendSignalAction Action
        {
            get { return action; }
            set { action = value; }
        }


        private InstanceSpecification signal;
        public InstanceSpecification Signal
        {
            get { return signal; }
            set { signal = value; }
        }


        private Signal signalClass;
        public Signal SignalClass
        {
            get { return signalClass; }
            set { signalClass = value; }
        }

        public SendSignalBehaviorExecution(SendSignalAction action, InstanceSpecification host, Dictionary<string, ValueSpecification> p)
            : base(action, host, p, false)
        {
            this.action = action;
        }

        public bool Equals(SendSignalAction other)
        {
            if (other == this.Action)
                return true;
            return false;
        }


        public override double execute(double dt)
        {
            //MascaretApplication.Instance.VRComponentFactory.Log(" EXECUTION SEND SIGNAL ... : " + action.Target.target.getFullName());

            if (action.Target.target != null)
            {
              //  MascaretApplication.Instance.VRComponentFactory.Log("Send Signal Action to " + action.Target.target.getFullName() + " : " + action.Target.target.SmBehaviorExecutions.Count);
                foreach (StateMachineBehaviorExecution smBe in action.Target.target.SmBehaviorExecutions)
                {
                    MascaretApplication.Instance.VRComponentFactory.Log("Send signal Machine : " + smBe.getStateMachine().name);
                    if (action.Signal != null)
                        smBe.addSignal(action.Signal);
                    else
                    {
                        if (action.SignalClass != null)
                        {
                            MascaretApplication.Instance.VRComponentFactory.Log("Send Signal " + action.SignalClass.name + " to " + action.Target.target.name);

                            InstanceSpecification sig = new InstanceSpecification(action.SignalClass.name, action.SignalClass);
                            smBe.addSignal(sig);
                        }
                        else
                            MascaretApplication.Instance.VRComponentFactory.Log("Pas de Signal");
                    }
                }
            }
            else
            {
                if (action.Signal != null)
                    MascaretApplication.Instance.VRComponentFactory.Log(action.Signal.name);
                else if (action.SignalClass != null)
                    MascaretApplication.Instance.VRComponentFactory.Log(" (Class) " + action.SignalClass.name);
                else
                    MascaretApplication.Instance.VRComponentFactory.Log(" No signal or signalClass");

                foreach (InstanceSpecification currentInstance in MascaretApplication.Instance.getEnvironment().InstanceSpecifications.Values)
                {

                    foreach (StateMachineBehaviorExecution smBe in currentInstance.SmBehaviorExecutions)
                    {
                        if (action.Signal != null)
                            smBe.addSignal(action.Signal);
                        else
                        {
                            if (action.SignalClass != null)
                            {
                                InstanceSpecification sig = new InstanceSpecification(action.SignalClass.name, action.SignalClass);
                                smBe.addSignal(sig);
                            }
                            else
                                MascaretApplication.Instance.VRComponentFactory.Log("Pas de Signal");
                        }
                    }
                    
                    if (currentInstance.GetType().Name == "VirtualHuman")
                    {
                        ProceduralBehavior pb = (ProceduralBehavior)((VirtualHuman)currentInstance).getBehaviorExecutingByName("ProceduralBehavior");
                        if (pb != null)
                        {
                            pb._signals.Add(action.Signal);
                        }

                    }
                    //if (currentInstance.GetType().Name())
                }
            }


            return 0;
        }
    }
}
