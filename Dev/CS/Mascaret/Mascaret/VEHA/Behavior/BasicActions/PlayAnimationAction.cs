using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Mascaret
{
    class PlayAnimationAction : Action, IEquatable<PlayAnimationAction>
    {

        public string animationName;

        public PlayAnimationAction()
        {
            Kind = "PlayAnimation";
        }

        public bool Equals(PlayAnimationAction other)
        {
            if (other == this)
                return true;
            return false;
        }

        public override BehaviorExecution createBehaviorExecution(InstanceSpecification host, Dictionary<string, ValueSpecification> p, bool sync)
        {
            PlayAnimationBehaviorExecution behavior = new PlayAnimationBehaviorExecution(this, host, p);
            return behavior;
        }

    }
}
