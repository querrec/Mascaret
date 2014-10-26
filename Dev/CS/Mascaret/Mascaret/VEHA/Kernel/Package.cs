using System;
using System.Collections.Generic;

namespace Mascaret
{
    public class Package : Element
    {

        private Dictionary<string, Package> packages;
        public Dictionary<string, Package> Packages
        {
            get { return packages; }
            set { packages = value; }
        }

        public void addPackages(Package element)
        {
            packages.Add(element.name, element);
            element.ParentPackage = this;
        }


        private Package parentPackage;
        public Package ParentPackage
        {
            get { return parentPackage; }
            set { parentPackage = value; }
        }


        private Dictionary<string, Class> classes;
        public Dictionary<string, Class> Classes
        {
            get { return classes; }
            set { classes = value; }
        }

        public void addClasses(Class element)
        {
            if (element != null)
            {
                classes.Add(element.name, element);
                element.Package = this;
            }
        }

        public Class getClass(string name)
        {
            if (classes.ContainsKey(name))
                return classes[name];
            else
                return null;
        }


        private Dictionary<string, Signal> signals;
        public Dictionary<string, Signal> Signals
        {
            get { return signals; }
            set { signals = value; }
        }

        public void addSignals(Signal element)
        {
            if (!signals.ContainsKey(element.name))
                signals.Add(element.name, element);
        }

        public Signal getSignal(string signalName)
        {
            if (signals.ContainsKey(signalName))
                return signals[signalName];
            else
                return null;
        }

        public Package(string name)
            : base(name)
        {
            signals = new Dictionary<string, Signal>();
            classes = new Dictionary<string, Class>();
            packages = new Dictionary<string, Package>();
        }

        public override string getFullName()
        {
            if (parentPackage != null)
                return ParentPackage.name + "::" + name;
            else
                return name;
        }

        public void print()
        {
            System.Console.WriteLine(getFullName());

            /*for (int i = 0; i < classes.Keys.Count; i++)
                Classes. .Values[i].print();
		
            for (int i = 0; i < packages.Keys.Count; i++)
                packages.Values[i].print();
                */
        }

    }
}
