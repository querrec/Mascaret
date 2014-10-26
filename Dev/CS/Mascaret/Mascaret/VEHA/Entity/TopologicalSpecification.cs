using System;
using System.Collections.Generic;

namespace Mascaret
{
    public class TopologicalSpecification : VirtualRealitySpecification
    {
        public TopologicalSpecification(Classifier type)
            : base(type)
        {
        }

        public override ValueSpecification clone()
        {
            throw new NotImplementedException();
        }
    }
}


