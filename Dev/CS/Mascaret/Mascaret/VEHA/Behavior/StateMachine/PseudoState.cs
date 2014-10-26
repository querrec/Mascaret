
using System;
using System.Collections.Generic;

namespace Mascaret
{
    public enum PseudoStateKind
    { INITIAL = 0, DEEPHISTORY = 1, SHALLOWHISTORY = 2, JOIN = 3, FORK = 4, JUNCTION = 5, CHOICE = 6, ENTRYPOINT = 7, EXITPOINT = 8, TERMINATE = 9 };

    [Serializable]
    public class PseudoState : Vertex
    {

        public PseudoStateKind kind;


        public PseudoState(string name)
            : base(name)
        {
        }

        public override BehaviorExecution activate(InstanceSpecification host, Dictionary<string, ValueSpecification> p)
        {
            return null;
        }

        public override void desactivate()
        {

        }
    }
}

