using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Mascaret
{
    public abstract class VirtualRealityComponentFactory
    {
        public abstract ShapeSpecification createShape(string name, string url, bool movable = true, bool recursive = false, string shader = "");
        public abstract BehaviorExecution InstanciateOpaqueBehavior(Behavior behavior, string typeName, InstanceSpecification host, Dictionary<string, ValueSpecification> p);
    }
}