using System;
using System.Collections.Generic;


namespace Mascaret
{
    public class Operation : BehavioralFeature
    {

        private Classifier type = null;
        public Classifier Type
        {
            get { return type; }
            set { type = value; }
        }


        private Class classifier = null;
        public Class Classifier
        {
            get { return classifier; }
            set { classifier = value; }
        }


        private Constraint bodycondition = null;
        public Constraint Bodycondition
        {
            get { return bodycondition; }
            set { bodycondition = value; }
        }


        private List<Parameter> parameters = new List<Parameter>();
        public List<Parameter> Parameters
        {
            get { return parameters; }
            set { parameters = value; }
        }

        public void addParameter(Parameter param)
        {
            parameters.Add(param);
        }


        private List<Constraint> preconditions = new List<Constraint>();
        public List<Constraint> Preconditions
        {
            get { return preconditions; }
            set { preconditions = value; }
        }

        public void addPrecondition(Constraint preCond)
        {
            preconditions.Add(preCond);
        }


        private List<Constraint> postconditions = new List<Constraint>();
        public List<Constraint> Postconditions
        {
            get { return postconditions; }
            set { postconditions = value; }
        }

        public void addPostcondition(Constraint postCond)
        {
            postconditions.Add(postCond);
        }

        public override string getFullName()
        {
            return Classifier.getFullName() + "." + name;
        }

        public Operation(string name, Class owner, Behavior behavior)
            : base(name, behavior)
        {
            this.classifier = owner;
            this.type = null;

        }

        public bool createBehaviorParameters(List<ValueSpecification> p, Dictionary<String, ValueSpecification> behaviorParameters)
        {
            if (p.Count != parameters.Count)
                return false;
            for (int i = p.Count; i > 0; i--)
            {
                if (p[i].Type.isA(parameters[i].Type))
                    return false;
                behaviorParameters.Add(parameters[i].name, p[i]);
            }
            return true;
        }
    }
}

