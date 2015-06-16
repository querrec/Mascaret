using System;
using System.Collections.Generic;

namespace Mascaret
{
    public class Model : Element
    {

        [Serializable]
        public struct Asso
        {
            int id;
            string propid;
            string ownClassid;
        }

        [Serializable]
        public struct Evt
        {
            MascaretEvent evt;
            StateMachine machine;
        }

        [Serializable]
        public struct ActivityMethod
        {
            string operationName;
            //ActivityMethod activity;
        }


        private Package package = null;
        public Package Package
        {
            get { return package; }

            set { package = value; }
        }


        protected ModelLoader2 modelLoader = null;
        public ModelLoader2 ModelLoader
        {
            get { return modelLoader; }
            set { modelLoader = value; }
        }


        protected Dictionary<string, Class> allClassesFullName;
        public Dictionary<string, Class> AllClassesFullName
        {
            get { return allClassesFullName; }
            set { allClassesFullName = value; }
        }



        protected Dictionary<string, List<Class>> allClasses;
        public Dictionary<string, List<Class>> AllClasses
        {
            get { return allClasses; }
            set { allClasses = value; }
        }


        protected string xmiUrl;
        public string XmiUrl
        {
            get { return xmiUrl; }
            set { xmiUrl = value; }
        }


        protected List<Asso> assoToWrite = null;

        protected List<Evt> eventsToWrite = null;

        protected List<ActivityMethod> activitiesToWrite = null;

        protected int cpt;


        protected Dictionary<string, string> ids = null;


        private Dictionary<string, MascaretPrimitiveType> basicTypes;
        public Dictionary<string, MascaretPrimitiveType> BasicTypes
        {
            get { return basicTypes; }
            set { basicTypes = value; }
        }

        public void addBasicType(MascaretPrimitiveType element)
        {
            basicTypes.Add(element.name, element);
        }


        protected Dictionary<string, Environment> environments;
        public Dictionary<string, Environment> Environments
        {
            get { return environments; }
            set { environments = value; }
        }

        public void addEnvironment(Environment env)
        {
            if (environments.Count != 0) environments.Clear();
            environments.Add(env.Url, env);
        }

        public Model(string name)
            : base(name)
        {
            allClassesFullName = new Dictionary<string, Class>();
            allClasses = new Dictionary<string, List<Class>>();
            basicTypes = new Dictionary<string, MascaretPrimitiveType>();
            environments = new Dictionary<string, Environment>();


            MascaretPrimitiveType boolPrimitiveType = new MascaretPrimitiveType("boolean");
            basicTypes.Add("boolean", boolPrimitiveType);

            MascaretPrimitiveType integerPrimitiveType = new MascaretPrimitiveType("integer");
            basicTypes.Add("integer", integerPrimitiveType);

            MascaretPrimitiveType integerPrimitiveTypeEA = new MascaretPrimitiveType("integer");
            basicTypes.Add("eajava_int", integerPrimitiveTypeEA);

            MascaretPrimitiveType realPrimitiveType = new MascaretPrimitiveType("real");
            basicTypes.Add("real", realPrimitiveType);

            MascaretPrimitiveType stringPrimitiveType = new MascaretPrimitiveType("string");
            basicTypes.Add("string", stringPrimitiveType);

            MascaretPrimitiveType undefinedPrimitiveType = new MascaretPrimitiveType("undefined");
            basicTypes.Add("undefined", undefinedPrimitiveType);

            MascaretPrimitiveType charPrimitiveType = new MascaretPrimitiveType("char");
            basicTypes.Add("char", charPrimitiveType);

            MascaretPrimitiveType shapePrimitiveType = new MascaretPrimitiveType("shape");
            basicTypes.Add("shape", shapePrimitiveType);

            MascaretPrimitiveType pointPrimitiveType = new MascaretPrimitiveType("point");
            basicTypes.Add("point", pointPrimitiveType);

            MascaretPrimitiveType pathPrimitiveType = new MascaretPrimitiveType("path");
            basicTypes.Add("path", pathPrimitiveType);

            MascaretPrimitiveType soundPrimitiveType = new MascaretPrimitiveType("sound");
            basicTypes.Add("sound", soundPrimitiveType);

            MascaretPrimitiveType animationPrimitiveType = new MascaretPrimitiveType("animation");
            basicTypes.Add("animation", animationPrimitiveType);

            //basicTypes.Add("vector3",new PrimitiveType("vector3"));

            MascaretPrimitiveType rotationPrimitiveType = new MascaretPrimitiveType("rotation");
            basicTypes.Add("rotation", rotationPrimitiveType);

            //basicTypes.Add("color",new PrimitiveType("color"));
            //basicTypes.Add("quaternion",new PrimitiveType("quaternion"));
        }

        public void addClassesFromPackage(Package p)
        {
            foreach (Class cl in p.Classes.Values)
            {
                allClassesFullName.Add(cl.getFullName(), cl);
                if (!allClasses.ContainsKey(cl.name))
                    allClasses.Add(cl.name, new List<Class>());

                allClasses[cl.name].Add(cl);
            }
            foreach (Package p2 in p.Packages.Values)
                addClassesFromPackage(p2);
        }

        protected string newID(string val)
        {
            return null;
        }

        protected string getID(string val)
        {
            return null;
        }

        public void registerAllClasses()
        {
            if (package == null)
                return;

            if (allClassesFullName != null)
                allClassesFullName.Clear();

            if (allClasses != null)
                allClasses.Clear();

            addClassesFromPackage(package);
        }

        public MascaretPrimitiveType getBasicType(string name)
        {
            if (basicTypes.ContainsKey(name))
                return basicTypes[name];
            else
                return basicTypes["real"];

            //c'est pas tout a fait ça dans l'original, verif traduction okay
        }

        public bool isPrimitiveType(string name)
        {
            if (basicTypes.ContainsKey(name) == true)
                return true;
            return false;
        }

        public void print()
        {
            System.Console.WriteLine(package.getFullName());
            package.print();
        }

        /* A REVOIR ENTIEREMENT POUR ADAPTER
            public bool toXMI(ostream os)
            public bool toXMI2(ostream os)
            public writeToWMI2(ostream os)
            public void writeHeader(ostream os)
            public void writeHeader2(ostream os)
            public void writeFooter(ostream os)
            public void writeContent(ostream os)
            public void writeModel(ostream os)
            public void writeModel2(ostream os)
            public void writePackage(Package package, ostream os)
            public void writePackage2(Package package, ostream os)
            public void writeClasse(Class classe, ostream os)
            public void writeClasse2(Class classe,ostream os)
            public void writeSignal2(Signal signal, ostream os)
            public void writeProperty2(Class classe,Property property, ostream os)
            public void writeOperation2(Class classe, Operation operation, ostream os)
            public void writeAssociation2(ostream os)
            public void writeEvent2(ostream os)
            public void writeBehaviorStateMachine2(Class classe, StateMachine machine, ostream os)
            public void writeStateMachineRegion2(Class classe, StateMachine machine, Region region, ostream os)
            public void writeActivityFrom2(Class classe, StateMachine machine, State state,Operation operation,  ostream os)
            public void writeActivity2(ostream os, Activity activity, string operationName="")
            public void writeDataType(ostream os)
            public void writeDataType2(ostream os)
            public void writeStereoType(ostream os)
            public void writeDocumentation(Element element, ostream os)
        */

    }
}

