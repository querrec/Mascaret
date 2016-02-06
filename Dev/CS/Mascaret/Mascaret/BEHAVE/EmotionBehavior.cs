using System;
using System.Collections.Generic;
using System.IO;

namespace Mascaret
{
    public class EmotionBehavior : CyclicBehaviorExecution

    {
        public EmotionBehavior(Behavior specif, InstanceSpecification host, Dictionary<String, ValueSpecification> p)
            : base(specif, host, p)
        {
        }

        public EmotionBehavior()
        {
        }

        public override void action()
        {
            MascaretApplication.Instance.VRComponentFactory.Log("JE SUIS LE NOUVEAU COMPORTEMENT*********************JE FAIS RIEN POUR L INSTANT");
        }
    }
}
