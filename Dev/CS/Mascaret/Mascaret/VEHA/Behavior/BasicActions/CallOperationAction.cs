using System;
using System.Collections.Generic;


namespace Mascaret
{
    public class CallOperationAction : Action, IEquatable<CallOperationAction>
    {

        private Operation operation;
        public Operation Operation
        {
            get { return operation; }
            set { operation = value; }
        }


        private InstanceSpecification target;
        public InstanceSpecification Target
        {
            get { return target; }
            set { target = value; }
        }


        private string stringArguments;
        public string StringArguments
        {
            get { return stringArguments; }
            set { stringArguments = value; }
        }


        private Dictionary<string, string> arguments = new Dictionary<string, string>();
        public Dictionary<string, string> Arguments
        {
            get { return arguments; }
            set { arguments = value; }
        }

        public CallOperationAction()
        {
            Kind = "CallOperation";
        }

        public bool Equals(CallOperationAction other)
        {
            if (other == this)
                return true;
            return false;
        }

        public override BehaviorExecution createBehaviorExecution(InstanceSpecification host, Dictionary<string, ValueSpecification> p, bool sync)
        {
            CallOperationBehaviorExecution behavior = new CallOperationBehaviorExecution(this, host, p);
            return behavior;
        }

        public void addArgument(string name, string val)
        {
            if (!arguments.ContainsKey(name))
                arguments.Add(name, val);
        }

        public string getArgument(string name)
        {
            return arguments[name];
        }

        public Dictionary<string, ValueSpecification> getParameters()
        {
            //MapStringString parameters = (MapStringString) ScriptableObject.CreateInstance("MapStringString");
            return null;
        }

        public bool isDynamic()
        {
            return false;
            /*if(stringArguments.Length>0)
                return true;
            else return false;*/
        }
    }
}
