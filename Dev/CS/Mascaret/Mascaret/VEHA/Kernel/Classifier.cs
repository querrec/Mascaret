using System;
using System.Collections.Generic;

namespace Mascaret
{
    public abstract class Classifier : Element
    {

        private bool isAbstract;
        public bool IsAbstract
        {
            get { return isAbstract; }
            set { isAbstract = value; }
        }


        private List<Classifier> children = new List<Classifier>();
        public List<Classifier> Children
        {
            get { return children; }
            set { children = value; }
        }

        public void addChild(Classifier child)
        {
            child.parents.Add(this);
            children.Add(child);
            _addFeaturesToChild(child);
        }


        private List<Classifier> parents = new List<Classifier>();
        public List<Classifier> Parents
        {
            get { return parents; }
            set { parents = value; }
        }

        public void addParents(Classifier element)
        {
            parents.Add(element);
        }

        //[Serializable]
        //public class MapStringBehavior : Map<string, Behavior>{};

        private Dictionary<String, Behavior> ownedBehavior;
        public Dictionary<String, Behavior> OwnedBehavior
        {
            get { return ownedBehavior; }
            set { ownedBehavior = value; }
        }

        public void addOwnedBehavior(Behavior element)
        {
            ownedBehavior.Add(element.name, element);
            foreach (Classifier child in children)
            {
                child.addOwnedBehavior(element);
            }
        }


        public abstract ValueSpecification createValueFromString(string str);//normalement virtuelle pure

        public Classifier(string name)
            : base(name)
        {
            isAbstract = false;
            ownedBehavior = new Dictionary<string, Behavior>();
        }

        protected virtual void _addFeaturesToChild(Classifier child)
        {
            foreach (Behavior currentBehavior in ownedBehavior.Values)
            {
                child.addOwnedBehavior(currentBehavior);
            }
        }

        public bool isA(Classifier classifierParam)
        {
            if (classifierParam == this)
                return true;
            else
            {
                foreach (Classifier parent in parents)
                {
                    if (parent.isA(classifierParam))
                        return true;
                }
            }
            return false;
        }

        public bool isAByName(string classifierName)
        {
            if (classifierName.CompareTo(name) == 0)
                return true;
            else if (classifierName.CompareTo(getFullName()) == 0)
                return true;
            else
            {
                foreach (Classifier parent in parents)
                {
                    if (parent.isAByName(classifierName))
                        return true;
                }
            }
            return false;
        }
    }
}
