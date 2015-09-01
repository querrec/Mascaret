using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Mascaret
{
    class PlayAnimationBehaviorExecution : BehaviorExecution
    {
        public PlayAnimationAction action;

        public PlayAnimationBehaviorExecution(PlayAnimationAction paction, InstanceSpecification host, Dictionary<string, ValueSpecification> p)
            : base(paction, host, p, false)
        {
            this.action = paction;
          
        }

        public override double execute(double dt)
        {
            MascaretApplication.Instance.VRComponentFactory.Log("%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% Playing : " + action.animationName);
            EmbodiedAgent agt = (EmbodiedAgent)Host;
            agt.Body.ActiveShape.playAnimation(action.animationName);
            return 0;
        }
    }
}
