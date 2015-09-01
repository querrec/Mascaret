using System.Collections;
using System.Collections.Generic;

namespace Mascaret
{
    public class CallOperationBehaviorExecution : BehaviorExecution
    {
        public CallOperationAction action;
        public BehaviorExecution behaviorExecution;


        public CallOperationBehaviorExecution(CallOperationAction paction, InstanceSpecification host, Dictionary<string, ValueSpecification> p)
            : base(paction, host, p, false)
        {
            this.action = paction;
            MascaretApplication.Instance.VRComponentFactory.Log("CallOperationAction : " + action.Operation.Method);

            foreach (ValuePin pin in action.ValuePins)
            {
                p.Add(pin.name, pin.ValueSpec);
            }
            MascaretApplication.Instance.VRComponentFactory.Log("READY TO Start");

            behaviorExecution = action.Operation.Method.createBehaviorExecution(this.Host, p, false);
            if (behaviorExecution == null) MascaretApplication.Instance.VRComponentFactory.Log("Chérie ca va trancher");
  
        }

        public override void stop()
        {
            base.stop();
            behaviorExecution.stop();
        }

        public override void restart()
        {
            base.restart();
            behaviorExecution.restart();
        }

        public override void pause()
        {
            base.pause();
            behaviorExecution.pause();
        }

        public override double execute(double dt)
        {
            return behaviorExecution.execute(dt);
        }
    }
}
