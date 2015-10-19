using System;
using System.IO;
using System.Xml.Linq;
using System.Collections.Generic;
using System.Runtime.Serialization;

namespace Mascaret
{
    public class InstanceLoader
    {

        private string basedir;
        public string Basedir
        {
            get { return basedir; }
            set { basedir = value; }
        }


        private Environment environment;
        public Environment Environment
        {
            get { return environment; }
            set { environment = value; }
        }


        private Model model;
        public Model Model
        {
            get { return model; }
            set { model = value; }
        }


        private AgentPlateform agentPlateform;
        public AgentPlateform AgentPlateform
        {
            get { return agentPlateform; }
            set { agentPlateform = value; }
        }


        private bool toRun;
        public bool ToRun
        {
            get { return toRun; }
            set { toRun = value; }
        }

        [Serializable]
        public struct Relation
        {

            public Entity e;

            public string property;

            public string rValue;
        }


        private List<Relation> relations;
        public List<Relation> Relations
        {
            get { return relations; }
            set { relations = value; }
        }
        //CLASSE NON TESTE POUR LE MOMENT L'INSTANTIATION SE FAIT EN DRAG AND DROP OU VIA LE MENU COMPONENT
        //[MenuItem("MASCARET/Import Instances Information")]
        public static void Init()
        {
            //InstanceLoader instanceLoader = new InstanceLoader (MascaretApplication.Instance.AgentPlateform, MascaretApplication.Instance.getEnvironment(), MascaretApplication.Instance.BaseDir+"/"+MascaretApplication.Instance.getEnvironment().Url, true);

            /*	string actFile = EditorUtility.OpenFilePanel("Browse actors file",MascaretApplication.Instance.BaseDir , "xml");
                if (actFile!="")
                {
                    instanceLoader.parseInstances(MascaretApplication.Instance.AgentPlateform,MascaretApplication.Instance.getEnvironment(),actFile, true);
                }
		
                string orgFile = EditorUtility.OpenFilePanel("Browse organisation file", MascaretApplication.Instance.BaseDir,"xml");
                if (orgFile!="")
                {
                    instanceLoader.parseInstances(MascaretApplication.Instance.AgentPlateform,MascaretApplication.Instance.getEnvironment(),orgFile, true);
	
                }*/
        }

        //defaultParamter toRun = true
        public InstanceLoader(AgentPlateform agentPlateform, Environment environment, string filename, bool toRun)
        {
           
            relations = new List<Relation>();
            this.toRun = toRun;
            try
            {
                if (filename != "")
                {
                    this.environment = environment;
                    this.agentPlateform = agentPlateform;
                    this.model = this.environment.Model;

                    XDocument parser;
                    this.basedir = filename;

                    //parser = XDocument.Load(filename);
                    String s = MascaretApplication.Instance.readFlow(filename);
                    parser = XDocument.Parse(s);

                    _parseInstances(parser.Root);
                    _addRelations();
                }
                relations.Clear();
            }
            catch (FileLoadException e)
            {
            }

        }

        //typedef KnowledgeBase* (*BaseInitFunc)();

        //int _id=0;


        public void parseInstances(AgentPlateform agentPlateform, Environment environment, string filename, bool toRun)
        {
            this.toRun = toRun;
            this.environment = environment;
            this.agentPlateform = agentPlateform;
            this.model = this.environment.Model;
            XDocument parser = null;
            this.basedir = filename;
            parser = XDocument.Load(filename);

            _parseInstances(parser.Root);
            _addRelations();
            relations.Clear();
        }


        protected void _parseInstances(XElement node)
        {

            if (node.Name.LocalName == "Area")
            {
                Entity instance = _createEntity(node, null);
                environment.MainZone = (Zone)instance;
            }
            else
            {
                foreach (XElement child in node.Elements())
                {
                    if (child.Name.LocalName == "Entity" || child.Name.LocalName == "Area")
                    {
                        Entity instance = _createEntity(child, null);
                    }
                    else if (child.Name.LocalName == "Human")
                    {
                        VirtualHuman human = _createHuman(child);
                        //if (human != null)
                            //agentPlateform.addAgent(human);
                    }
                    else if (child.Name.LocalName == "Organisation")
                    {
                        OrganisationalEntity org = _createOrganisation(child);
                        agentPlateform.Organisations.Add(org);
                    }

                }
            }
        }


        protected Entity _createEntity(XElement node, Entity parent)
        {

            Entity e = null;
            XAttribute attr;
            if (parent == null)
            {
                string parentName = "";
                attr = (XAttribute)node.Attribute("area");
                if (attr != null) parentName = attr.Value;
                parent = MascaretApplication.Instance.getEnvironment().getEntity(parentName);
                //Debug.Log (" #### Looking parent : " + parentName);
            }

            string className = "";
            attr = (XAttribute)node.Attribute("class");
            if (attr != null) className = attr.Value;

            Class classe = null;
            if (className == "")
                className = "undef";

            string nodeName = "";
            attr = (XAttribute)node.Attribute("name");
            if (attr != null) nodeName = attr.Value;


            classe = _findClass(className);
            if (classe == null)
            {
                return e;
            }
            EntityClass entityClasse = (EntityClass)classe;
            if (entityClasse == null)
            {
                return e;
            }

            if (node.Name.LocalName == "Area")
            {
                Zone currentZone = new Zone(nodeName, entityClasse);

                e = (Entity)currentZone;
            }
            else
            {
                e = new Entity(nodeName, entityClasse);
                //Debug.Log("Create Entity : " + nodeName);
            }

            if (toRun)
            {
                foreach (Behavior currentBehaviour in classe.OwnedBehavior.Values)
                {
                    BehaviorScheduler.Instance.executeBehavior(currentBehaviour, e, null, false);
                }
            }
            environment.addInstance(e);

            if (parent != null)
            {
                e.Parent = parent;
                parent.addChild(e);
            }

            _readEntity(node, e);
            return e;
        }


        protected void _readEntity(XElement node, Entity entity)
        {
            if (entity == null) return;

            //Debug.Log(" ------- > Reading Entity : " + entity.getFullName());

            Vector3 scaleFactor = new Vector3(1, 1, 1);
            //entity.setParent(entity.getParent());
            //point = entity.getReferentialPoint();

            foreach (XElement child in node.Elements())
            {
                if (child.Name.LocalName == "Entity" || child.Name.LocalName == "Area")
                {
                    Entity instance = _createEntity(child, entity);
                    //Debug.Log( "Reading Entity : " + child.Name );
                }
                else if (child.Name.LocalName == "Human")
                {
                    VirtualHuman human = _createHuman(child);
                    //if (human != null)
                        //agentPlateform.addAgent(human);
                }
                else if (child.Name.LocalName == "Organisation")
                {
                    OrganisationalEntity org = _createOrganisation(child);
                    agentPlateform.Organisations.Add(org);
                }
                else if (child.Name.LocalName == "Position" || child.Name.LocalName == "Location")
                {
                    Vector3 localPosition = new Vector3(0, 0, 0);
                    /*
                    XAttribute attr = (XAttribute) child.Attribute("x");
                    if(attr!=null) localPosition.x = float.Parse(attr.Value);
				
                    attr = (XAttribute) child.Attribute("y");
                    if(attr!=null) localPosition.y = float.Parse(attr.Value);
				
                    attr = (XAttribute) child.Attribute("z");
                    if(attr!=null) localPosition.z = float.Parse(attr.Value);
                    */
                    //Debug.Log( "Reading Position : " + child.Name );
                    entity.setLocalPosition(localPosition);
                    //Debug.Log( "Reading Position : " + child.Name );
                }
                else if (child.Name.LocalName == "Orientation")
                {
                    XAttribute angle = (XAttribute)child.Attribute("angle");
                    if (angle != null)
                    {
                        RotationVector rotationAngle = new RotationVector(0, 0, 0, 0);

                        XAttribute attr = (XAttribute)child.Attribute("x");
                        if (attr != null) rotationAngle.x = float.Parse(attr.Value);

                        attr = (XAttribute)child.Attribute("y");
                        if (attr != null) rotationAngle.y = float.Parse(attr.Value);

                        attr = (XAttribute)child.Attribute("z");
                        if (attr != null) rotationAngle.z = float.Parse(attr.Value);

                        attr = (XAttribute)child.Attribute("angle");
                        if (attr != null) rotationAngle.angle = float.Parse(attr.Value);

                        //Debug.Log( "Reading Rotation : " + child.Name );
                        entity.setLocalRotation(rotationAngle);
                        //Debug.Log( "Reading Rotation : " + child.Name );
                    }
                    else
                    {
                        Vector3 localPosition = new Vector3(0, 0, 0);

                        XAttribute attr = (XAttribute)child.Attribute("x");
                        if (attr != null) localPosition.x = float.Parse(attr.Value);

                        attr = (XAttribute)child.Attribute("y");
                        if (attr != null) localPosition.y = float.Parse(attr.Value);

                        attr = (XAttribute)child.Attribute("z");
                        if (attr != null) localPosition.z = float.Parse(attr.Value);

                        //Debug.Log( "Reading Orientation : " + child.Name );
                        entity.setLocalOrientation(localPosition);
                        //Debug.Log( "Reading Orientation : " + child.Name );
                    }
                }
                else if (child.Name.LocalName == "Topological")
                {
                    string name = "";

                    XAttribute attr = (XAttribute)child.Attribute("name");
                    if (attr != null) name = attr.Value;

                    XElement posNode = null, oriNode = null;


                    foreach (XElement currentNode in child.Elements())
                    {
                        if (currentNode.Name.LocalName == "Position")
                            posNode = currentNode;
                        if (currentNode.Name.LocalName == "Orientation")
                            oriNode = currentNode;
                    }

                    Vector3 pos = new Vector3(0, 0, 0);

                    attr = (XAttribute)posNode.Attribute("x");
                    if (attr != null) pos.x = float.Parse(attr.Value);

                    attr = (XAttribute)posNode.Attribute("y");
                    if (attr != null) pos.y = float.Parse(attr.Value);

                    attr = (XAttribute)posNode.Attribute("z");
                    if (attr != null) pos.z = float.Parse(attr.Value);

                    //PointSpecification point = new PointSpecification();

                    //regarder creation de point, anim et autre par la VRApplication dans mascaret

                    //((UnityPointSpecification)point).instantiate(pos,(RotationVector)ScriptableObject.CreateInstance("RotationVector"));
                    //point.Parent=entity.ReferentialPoint;
                    //point.setLocalPosition(pos);
                    if (oriNode != null)
                    {
                        attr = (XAttribute)oriNode.Attribute("angle");
                        if (attr != null)
                        {
                            RotationVector rotationAngle = new RotationVector(0, 0, 0, 0);

                            XAttribute attr2 = (XAttribute)oriNode.Attribute("x");
                            if (attr2 != null) rotationAngle.x = float.Parse(attr2.Value);

                            attr2 = (XAttribute)oriNode.Attribute("y");
                            if (attr2 != null) rotationAngle.y = float.Parse(attr2.Value);

                            attr2 = (XAttribute)oriNode.Attribute("z");
                            if (attr2 != null) rotationAngle.z = float.Parse(attr2.Value);

                            attr2 = (XAttribute)oriNode.Attribute("angle");
                            if (attr2 != null) rotationAngle.angle = float.Parse(attr2.Value);

                            //point.setLocalRotation(rotationAngle);
                        }
                        else
                        {
                            Vector3 localPosition = new Vector3(0, 0, 0);

                            XAttribute attr2 = (XAttribute)child.Attribute("x");
                            if (attr2 != null) localPosition.x = float.Parse(attr2.Value);

                            attr2 = (XAttribute)child.Attribute("y");
                            if (attr2 != null) localPosition.y = float.Parse(attr2.Value);

                            attr2 = (XAttribute)child.Attribute("z");
                            if (attr2 != null) localPosition.z = float.Parse(attr2.Value);

                            //point.setLocalOrientation(localPosition);
                        }
                    }
                    //entity.getProperty(name).addValue(point);
                }
                else if (child.Name.LocalName == "Viewpoint")
                {
                    //Debug.Log( "Reading Viewpoint : " + child.Name );
                    string name = "";

                    XAttribute attr = (XAttribute)child.Attribute("name");
                    if (attr != null) name = attr.Value;

                    XElement posNode = null, oriNode = null;

                    foreach (XElement currentNode in child.Elements())
                    {
                        if (currentNode.Name.LocalName == "Position")
                            posNode = currentNode;
                        if (currentNode.Name.LocalName == "Orientation")
                            oriNode = currentNode;
                    }

                    Vector3 pos = new Vector3(0, 0, 0);
                    /*	
                    attr = (XAttribute) posNode.Attribute("x");
                    if(attr!=null) pos.x = float.Parse(attr.Value);
				
                    attr = (XAttribute) posNode.Attribute("y");
                    if(attr!=null) pos.y = float.Parse(attr.Value);
				
                    attr = (XAttribute) posNode.Attribute("z");
                    if(attr!=null) pos.z = float.Parse(attr.Value);
                    */
                    //PointSpecification viewPoint=(UnityPointSpecification)ScriptableObject.CreateInstance("UnityPointSpecification");
                    //((UnityPointSpecification)viewPoint).instantiate(pos, (RotationVector)ScriptableObject.CreateInstance("RotationVector"));
                    //viewPoint.Parent=entity.ReferentialPoint;

                    attr = (XAttribute)oriNode.Attribute("angle");
                    if (attr != null)
                    {
                        RotationVector rotationAngle = new RotationVector(0, 0, 0, 0);

                        XAttribute attr2 = (XAttribute)oriNode.Attribute("x");
                        if (attr2 != null) rotationAngle.x = float.Parse(attr2.Value);

                        attr2 = (XAttribute)oriNode.Attribute("y");
                        if (attr2 != null) rotationAngle.y = float.Parse(attr2.Value);

                        attr2 = (XAttribute)oriNode.Attribute("z");
                        if (attr2 != null) rotationAngle.z = float.Parse(attr2.Value);

                        attr2 = (XAttribute)oriNode.Attribute("angle");
                        if (attr2 != null) rotationAngle.angle = float.Parse(attr2.Value);

                        //viewPoint.setLocalRotation(rotationAngle);
                    }
                    else
                    {
                        Vector3 localPosition = new Vector3(0, 0, 0);

                        XAttribute attr2 = (XAttribute)oriNode.Attribute("x");
                        if (attr2 != null) localPosition.x = float.Parse(attr2.Value);

                        attr2 = (XAttribute)oriNode.Attribute("y");
                        if (attr2 != null) localPosition.y = float.Parse(attr2.Value);

                        attr2 = (XAttribute)oriNode.Attribute("z");
                        if (attr2 != null) localPosition.z = float.Parse(attr2.Value);

                        //viewPoint.setLocalOrientation(localPosition);
                    }
                    //Debug.Log(entity.name );
                    //entity.ViewPoints.Add(name,viewPoint);
                    //Debug.Log( "Reading Viewpoint : " + child.Name );
                }
                else if (child.Name.LocalName == "OrientationLC")
                {
                    ////Debug.Log( "Reading OrientationLC : " + child.Name );
                    XAttribute attr = (XAttribute)child.Attribute("angle");
                    if (attr != null)
                    {
                        RotationVector rotationAngle = new RotationVector(0, 0, 0, 0);

                        XAttribute attr2 = (XAttribute)child.Attribute("x");
                        if (attr2 != null) rotationAngle.x = float.Parse(attr2.Value);

                        attr2 = (XAttribute)child.Attribute("y");
                        if (attr2 != null) rotationAngle.y = float.Parse(attr2.Value);

                        attr2 = (XAttribute)child.Attribute("z");
                        if (attr2 != null) rotationAngle.z = float.Parse(attr2.Value);

                        attr2 = (XAttribute)child.Attribute("angle");
                        if (attr2 != null) rotationAngle.angle = float.Parse(attr2.Value);

                        entity.setLocalRotation(rotationAngle);
                    }
                    else
                    {
                        Vector3 localPosition = new Vector3(0, 0, 0);

                        XAttribute attr2 = (XAttribute)child.Attribute("x");
                        if (attr != null) localPosition.x = float.Parse(attr2.Value);

                        attr2 = (XAttribute)child.Attribute("y");
                        if (attr2 != null) localPosition.y = float.Parse(attr2.Value);

                        attr2 = (XAttribute)child.Attribute("z");
                        if (attr2 != null) localPosition.z = float.Parse(attr2.Value);

                        entity.setLocalOrientation(localPosition);
                    }
                    Vector3 ori = entity.getLocalOrientation();

                    //x = ori.x; y = ori.y; z = ori.z;
                    // TODO: ATTENTION problÃ¨me, ici c'est un setGlobalOrientation sans doute
                    PointSpecification parentPoint = entity.Parent.ReferentialPoint;
                    parentPoint.localToGlobalOrientation(ori);
                    //Vector3 ori2(x,y,z);
                    entity.setLocalOrientation(ori);
                    ////Debug.Log( "Reading OrientationLC : " + child.Name );
                }

                else if (child.Name.LocalName == "Shape")
                {
                    //Debug.Log( "Reading Shape : " + child.Name );
                    string url = "";

                    XAttribute attr = (XAttribute)child.Attribute("url");
                    if (attr != null) url = attr.Value;
                    else
                    {
                        attr = (XAttribute)child.Attribute("value");
                        if (attr != null) url = attr.Value;
                    }
                    //Debug.Log ("==== Shape = " + url);

                    bool movable = false;
                    attr = (XAttribute)child.Attribute("movable");
                    if (attr != null)
                    {
                        movable = Boolean.Parse(attr.Value);
                    }
                    bool recursive = false;
                    attr = (XAttribute)child.Attribute("recursive");
                    if (attr != null)
                    {
                        recursive = Boolean.Parse(attr.Value);
                    }

                    /*
                     url += basedir;
	
                    if(url.Length==0)
                    {
                        attr = (XAttribute) child.Attribute("url");
                        if(attr!=null)
                            Debug.Log( "Fichier " + attr.Value + " non trouvé pour l'entity "+ entity.name);
                    }
                    */

                    string shader = "";
                    attr = (XAttribute)child.Attribute("shader");
                    if (attr != null)
                    {
                        shader = attr.Value;
                    }
                    string shapeName = "shape";
                    attr = (XAttribute)child.Attribute("name");
                    if (attr != null)
                    {
                        shapeName = attr.Value;
                    }
                    ShapeSpecification specif = MascaretApplication.Instance.VRComponentFactory.createShape(entity.name + "_" + shapeName, url, movable, recursive, shader);
                    //entity.getProperty(shapeName).addValue(specif);
                    entity.setActiveShape(shapeName);

                    specif.setEntity(entity);
                    entity.ActiveShape = specif;

                }

                else if (child.Name.LocalName == "Shapes")
                {
                    bool movable = false;
                    XAttribute attr = (XAttribute)child.Attribute("movable");
                    if (attr != null)
                    {
                        movable = Boolean.Parse(attr.Value);
                    }
                    bool recursive = false;
                    attr = (XAttribute)child.Attribute("recursive");
                    if (attr != null)
                    {
                        recursive = Boolean.Parse(attr.Value);
                    }

                    List<string> urls = new List<string>();
                    List<double> distances = new List<double>();

                    foreach (XElement shapeNode in child.Elements())
                    {
                        if (shapeNode.Name.LocalName == "Shape")
                        {
                            string url = "";
                            attr = (XAttribute)shapeNode.Attribute("url");
                            if (attr != null) url = attr.Value;

                            url += basedir;

                            if (url.Length == 0)
                            {
                            }

                            urls.Add(url);
                            double dist = 0;
                            attr = (XAttribute)shapeNode.Attribute("dist");
                            if (attr != null)
                            {
                                dist = Double.Parse(attr.Value);
                            }
                            distances.Add(dist);
                        }
                    }
                    string shapeName = "shape";
                    attr = (XAttribute)child.Attribute("name");
                    if (attr != null)
                    {
                        shapeName = attr.Value;
                    }
                    //ShapeSpecification specif=(UnityShapeSpecification)ScriptableObject.CreateInstance("UnityShapeSpecification");
                    //specif.instantiate(shapeName,distances,urls,movable,recursive,"");
                    //entity.getProperty(shapeName).addValue(specif);
                    //entity.set_DEP_current(specif);
                    //entity.setActiveShape(shapeName);
                }
                else if (child.Name.LocalName == "Scale")
                {
                    XAttribute attr = (XAttribute)child.Attribute("sx");
                    if (attr != null) scaleFactor.x = float.Parse(attr.Value);

                    attr = (XAttribute)child.Attribute("sy");
                    if (attr != null) scaleFactor.y = float.Parse(attr.Value);

                    attr = (XAttribute)child.Attribute("sz");
                    if (attr != null) scaleFactor.z = float.Parse(attr.Value);

                }
                else if (child.Name.LocalName == "Animation")
                {
                    string name = "";
                    string file = "";

                    XAttribute attr = (XAttribute)child.Attribute("name");
                    if (attr != null) name = attr.Value;

                    attr = (XAttribute)child.Attribute("url");
                    if (attr != null) file = attr.Value + basedir;

                    //AnimationSpecification animation = (UnityAnimationSpecification)ScriptableObject.CreateInstance("UnityAnimationSpecification");
                    //animation.instantiate(name, file);
                    //entity.getProperty(name).addValue(animation);
                }
                else if (child.Name.LocalName == "Sound")
                {
                    string name = "";
                    string file = "";

                    XAttribute attr = (XAttribute)child.Attribute("name");
                    if (attr != null) name = attr.Value;

                    attr = (XAttribute)child.Attribute("url");
                    if (attr != null) file = attr.Value + basedir;

                    double gain = 1.0;
                    double minDist = 1.0;
                    double maxDist = 100.0;

                    attr = (XAttribute)child.Attribute("gain");
                    if (attr != null) gain = float.Parse(attr.Value);

                    attr = (XAttribute)child.Attribute("minDistance");
                    if (attr != null) minDist = float.Parse(attr.Value);

                    attr = (XAttribute)child.Attribute("maxDistance");
                    if (attr != null) maxDist = float.Parse(attr.Value);

                    //SoundSpecification sound = (UnitySoundSpecification)ScriptableObject.CreateInstance("UnitySoundSpecification");
                    //((UnitySoundSpecification)sound).instantiate(name, file, gain,minDist,maxDist, false);
                    //entity.getProperty(name).addValue(sound);
                    //sound.attachTo(entity.ReferentialPoint);
                }
               /* else if (child.Name.LocalName == "Inline")
                {
                    XDocument parser = null;
                    try
                    {

                        XAttribute attr = (XAttribute)child.Attribute("url");
                        if (attr != null) parser = XDocument.Load(attr.Value + basedir);
                        _readEntity(parser.Root, entity);
                    }
                    catch (FileLoadException e)
                    {
                    }
                }*/
                else if ((child.Name.LocalName == "Attribute") || (child.Name.LocalName == "Relation"))
                {
                    Relation r = new Relation();
                    r.e = entity;

                    XAttribute attr = (XAttribute)child.Attribute("name");
                    if (attr != null) r.property = attr.Value;

                    attr = (XAttribute)child.Attribute("value");
                    if (attr != null) r.rValue = attr.Value;
                    relations.Add(r);
                }
                else if (child.Name.LocalName == "InternalOrientationAxe")
                {
                    //TODO: Pourquoi non utilisé ?
                    /*Axe _front = AxeFromNodeProperty(child, "frontAxe");
                    Axe _left = AxeFromNodeProperty(child, "leftAxe");
                    Axe _above = AxeFromNodeProperty(child, "aboveAxe");*/
                }
                else if (child.Name.LocalName == "InternalOrientationVector")
                {
                    //TODO:
                }
                else if (child.Name.LocalName == "State")
                {
                    _initializeStateMachine(child, entity);
                }

            }
            if (entity.ActiveShape != null)
            {
                entity.ActiveShape.setScale(scaleFactor);
            }

        }

        protected bool _initializeStateMachine(XElement node, Entity entity)
        {
            string machine = "";
            string state = "";
            XAttribute attr = (XAttribute)node.Attribute("machine");
            if (attr != null) machine = attr.Value;

            attr = (XAttribute)node.Attribute("state");
            if (attr != null) state = attr.Value;

            if (machine.Length == 0 || state.Length == 0)
            {
                //Debug.Log( "[InstanceLoader Error] Entity: " + entity.name + " found empty string for machine or state." );
                return false;
            }
            else
            {
                bool found_machine = false;
                foreach (StateMachineBehaviorExecution smbe in entity.SmBehaviorExecutions)
                {
                    if (smbe.getStateMachine().name == machine)
                    {
                        found_machine = true;
                        smbe.setCurrentState(state);
                        // TODO: Test if state exist on state machine ?
                    }
                }
                if (!found_machine)
                {
                    //Debug.Log( "[InstanceLoader Error] Entity: " + entity.name + " StateMachine " + machine + " not found." );
                    return false;
                }
            }
            return true;
        }

        protected void _readGeometryEntity(XElement node, Entity entity)
        {
            Vector3 scaleFactor = new Vector3(1, 1, 1);
            entity.setParent(entity.Parent);
            PointSpecification point;
            point = entity.ReferentialPoint;

            foreach (XElement child in node.Elements())
            {
                if (child.Name.LocalName == "Position" || child.Name.LocalName == "Location")
                {

                    XAttribute attr = (XAttribute)child.Attribute("entity");
                    if (attr != null)
                    {
                        string entityName = attr.Value;
                        //InstanceSpecification instance = MascaretApplication.Instance.getEnvironment().getInstance(entityName);
                        InstanceSpecification instance = environment.getInstance(entityName);
                        if (instance != null)
                        {
                            Entity entPos = instance as Entity;
                            if (entPos != null)
                            {
                                entPos.addChild(entity);

                                attr = (XAttribute)child.Attribute("point");
                                if (attr != null)
                                {
                                    string pointName = attr.Value;
                                    PointSpecification pt = entPos.getPoint(pointName);
                                    if (pt != null)
                                    {
                                        Vector3 pos = pt.getGlobalPosition();
                                        Vector3 ori = pt.getGlobalOrientation();
                                        point.setGlobalPosition(pos);
                                        point.setGlobalOrientation(ori);
                                    }
                                    
                                }
                                else
                                {
                                    Vector3 pos = entPos.getGlobalPosition();
                                    Vector3 ori = entPos.getGlobalOrientation();
                                    point.setGlobalPosition(pos);
                                    point.setGlobalOrientation(ori);
                                }
                            }
                           
                        }
                        

                    }
                    else
                    {/*
					Vector3 localPosition = new Vector3(0,0,0);
					attr = (XAttribute) child.Attribute("x");
					if(attr!=null) localPosition.x = float.Parse(attr.Value);
					
					attr = (XAttribute) child.Attribute("y");
					if(attr!=null) localPosition.y = float.Parse(attr.Value);
					
					attr = (XAttribute) child.Attribute("z");
					if(attr!=null) localPosition.z = float.Parse(attr.Value);
					
                	point.setLocalPosition(localPosition);
                	*/
                    }
                }
                else if (child.Name.LocalName == "Orientation")
                {

                    XAttribute attr = (XAttribute)child.Attribute("angle");
                    if (attr != null)
                    {
                        RotationVector rotationAngle = new RotationVector(0, 0, 0, 0);

                        attr = (XAttribute)child.Attribute("x");
                        if (attr != null) rotationAngle.x = float.Parse(attr.Value);

                        attr = (XAttribute)child.Attribute("y");
                        if (attr != null) rotationAngle.y = float.Parse(attr.Value);

                        attr = (XAttribute)child.Attribute("z");
                        if (attr != null) rotationAngle.z = float.Parse(attr.Value);

                        attr = (XAttribute)child.Attribute("angle");
                        if (attr != null) rotationAngle.angle = float.Parse(attr.Value);

                        point.setLocalRotation(rotationAngle);
                    }
                    else
                    {
                        /*
                        Vector3 localPosition = new Vector3(0,0,0);
                        attr = (XAttribute) child.Attribute("x");
                        if(attr!=null) localPosition.x = float.Parse(attr.Value);
					
                        attr = (XAttribute) child.Attribute("y");
                        if(attr!=null) localPosition.y = float.Parse(attr.Value);
					
                        attr = (XAttribute) child.Attribute("z");
                        if(attr!=null) localPosition.z = float.Parse(attr.Value);
					
                        point.setLocalOrientation(localPosition);
                        */
                    }
                }

                else if (child.Name.LocalName == "Shape")
                {
                    string url = "";


                    XAttribute attr = (XAttribute)child.Attribute("url");
                    if (attr != null) url = attr.Value;

                    bool movable = false;
                    attr = (XAttribute)child.Attribute("movable");
                    if (attr != null)
                    {
                        movable = Boolean.Parse(attr.Value);
                    }
                    bool recursive = false;
                    attr = (XAttribute)child.Attribute("recursive");
                    if (attr != null)
                    {
                        recursive = Boolean.Parse(attr.Value);
                    }

                    //url += basedir;

                    string shader = "";
                    attr = (XAttribute)child.Attribute("shader");
                    if (attr != null)
                    {
                        shader = attr.Value;
                    }
                    string name = "shape";
                    attr = (XAttribute)child.Attribute("name");
                    if (attr != null)
                    {
                        name = attr.Value;
                    }
                    ShapeSpecification specif = MascaretApplication.Instance.VRComponentFactory.createShape(entity.name, url, movable, recursive, shader);
                    //ShapeSpecification specif = MascaretApplication.Instance.VRComponentFactory.createShape(entity.name + "_" + name, url, movable, recursive, shader);
                    //entity.getProperty(shapeName).addValue(specif);
                    entity.setActiveShape(name);

                    specif.setEntity(entity);
                    entity.ActiveShape = specif;
                }
                else if (child.Name.LocalName == "Shapes")
                {
                    bool movable = false;
                    XAttribute attr = (XAttribute)child.Attribute("movable");
                    if (attr != null)
                    {
                        movable = Boolean.Parse(attr.Value);
                    }
                    bool recursive = false;
                    attr = (XAttribute)child.Attribute("recursive");
                    if (attr != null)
                    {
                        recursive = Boolean.Parse(attr.Value);
                    }

                    List<string> urls = new List<string>();
                    List<double> distances = new List<double>();

                    foreach (XElement shapeNode in child.Elements())
                    {
                        if (shapeNode.Name.LocalName == "Shape")
                        {
                            string url = "";
                            attr = (XAttribute)shapeNode.Attribute("url");
                            if (attr != null) url = attr.Value;

                            url += basedir;

                            if (url.Length == 0)
                            {
                            }
                            urls.Add(url);
                            double dist = 0;

                            attr = (XAttribute)shapeNode.Attribute("dist");
                            if (attr != null)
                            {
                                dist = Double.Parse(attr.Value);
                            }
                            distances.Add(dist);
                        }
                    }
                    string name = "shape";
                    attr = (XAttribute)child.Attribute("name");
                    if (attr != null)
                    {
                        name = attr.Value;
                    }
                    //ShapeSpecification specif = (UnityShapeSpecification)ScriptableObject.CreateInstance("UnityShapeSpecification");
                    //((UnityShapeSpecification)specif).instantiate(name,distances,urls,movable,recursive,"");
                    //entity.getProperty(name).addValue(specif);
                    //entity.set_DEP_current(specif);
                    //entity.setActiveShape(name);
                }
                else if (child.Name.LocalName == "Scale")
                {
                    XAttribute attr = (XAttribute)child.Attribute("sx");
                    if (attr != null) scaleFactor.x = float.Parse(attr.Value);

                    attr = (XAttribute)child.Attribute("sy");
                    if (attr != null) scaleFactor.y = float.Parse(attr.Value);

                    attr = (XAttribute)child.Attribute("sz");
                    if (attr != null) scaleFactor.z = float.Parse(attr.Value);

                }
                else if (child.Name.LocalName == "Animation")
                {
                    string name = "";

                    XAttribute attr = (XAttribute)child.Attribute("name");
                    if (attr != null) name = attr.Value;

                    bool running = false;

                    attr = (XAttribute)child.Attribute("running");
                    if (attr != null) running = Boolean.Parse(attr.Value);

                    string file = "";
                    attr = (XAttribute)child.Attribute("url");
                    if (attr != null)
                    {
                        file = attr.Value + basedir;
                    }

                    //AnimationSpecification animation = (UnityAnimationSpecification)ScriptableObject.CreateInstance("UnityAnimationSpecification");
                    //((UnityAnimationSpecification)animation).instantiate(name,file, 1,1.0,false);
                    //entity.getProperty(name).addValue(animation);
                    //if (running) entity.playAnimation(name,1,1.0, true);
                }
                else if (child.Name.LocalName == "Sound")
                {
                    string name = "";

                    XAttribute attr = (XAttribute)child.Attribute("name");
                    if (attr != null) name = attr.Value;

                    string file = "";

                    attr = (XAttribute)child.Attribute("url");
                    if (attr != null) file = attr.Value + basedir;

                    double gain = 1.0;
                    double minDist = 1.0;
                    double maxDist = 100.0;

                    attr = (XAttribute)child.Attribute("gain");
                    if (attr != null) gain = float.Parse(attr.Value);

                    attr = (XAttribute)child.Attribute("minDistance");
                    if (attr != null) minDist = float.Parse(attr.Value);

                    attr = (XAttribute)child.Attribute("maxDistance");
                    if (attr != null) maxDist = float.Parse(attr.Value);

                    //SoundSpecification sound = (UnitySoundSpecification)ScriptableObject.CreateInstance("UnitySoundSpecification");
                    //((UnitySoundSpecification)sound).instantiate(name, file, gain,minDist,maxDist,false);
                    //entity.getProperty(name).addValue(sound);
                    //sound.attachTo(entity.ReferentialPoint);
                }
                else if (child.Name.LocalName == "Inline")
                {
                    XDocument parser = null;

                    XAttribute attr = (XAttribute)child.Attribute("url");
                    if (attr != null)
                    {
                        try
                        {
                            parser = XDocument.Load(attr.Value + basedir);
                            _readGeometryEntity(parser.Root, entity);
                        }
                        catch (FileLoadException e)
                        {
                        }
                    }
                }
                else if (child.Name.LocalName == "InternalOrientationAxe")
                {
                    //TODO: Pourquoi non utilisé ?
                    /*Axe _front = AxeFromNodeProperty(child, "frontAxe");
                    Axe _left = AxeFromNodeProperty(child, "leftAxe");
                    Axe _above = AxeFromNodeProperty(child, "aboveAxe");*/
                    /*point.setFront(_front);
                    point.setLeft(_left);
                    point.setAbove(_above);*/
                }
                else if (child.Name.LocalName == "InternalOrientationVector")
                {
                    //TODO:
                }
                else if (child.Name.LocalName == "State")
                {
                    // TODO: Maybe it's not normal to found a State here ?
                    _initializeStateMachine(child, entity);
                }

            }
            /*
            if (entity.getActiveShape())
            {
                entity.getActiveShape().setScale(scaleFactor);
            }	 */
        }

        protected OrganisationalEntity _createOrganisation(XElement node)
        {
            //StreamWriter file = new StreamWriter("Organisation.txt");

            string name = "";
            OrganisationalEntity organisation = null;
            XAttribute attr = (XAttribute)node.Attribute("name");
            if (attr != null) name = attr.Value;


            foreach (XElement struc in node.Elements())
            {
                if (struc.Name.LocalName == "Structure")
                {
                    string structName = struc.Value;

                    OrganisationalStructure structOrg = null;

                    foreach (OrganisationalStructure currentOrg in agentPlateform.Structures)
                    {
                        if (currentOrg.name == structName)
                        {
                            structOrg = currentOrg;
                            break;
                        }
                    }
                    if (structOrg == null)
                    {  }
                    else
                    {
                      //  MascaretApplication.Instance.logfile.WriteLine("Structure found"); MascaretApplication.Instance.logfile.Flush();
                        organisation = new OrganisationalEntity(name);
                        organisation.Structure = structOrg;

                        structOrg.Entities.Add(organisation);

                        foreach (XElement affects in node.Elements())
                        {
                            if (affects.Name.LocalName == "Affect")
                            {
                                attr = (XAttribute)affects.Attribute("type");

                                if (attr != null && attr.Value == "Role")
                                {

                                //    file.WriteLine("Affect Role"); file.Flush();
                                    XElement roleNode = null, agentNode = null, aidNode = null;

                                    foreach (XElement currentNode in affects.Elements())
                                    {
                                        if (currentNode.Name.LocalName == "Role")
                                            roleNode = currentNode;
                                        else if (currentNode.Name.LocalName == "Agent")
                                            agentNode = currentNode;
                                        else if (currentNode.Name.LocalName == "AID")
                                            aidNode = currentNode;
                                    }

                                    string agentName = "";
                                    string plateformeName = "localhost";
                                    int plateformePort = agentPlateform.PortNumber;
                                    if (aidNode != null)
                                    {
                                        foreach (XElement currentNode in aidNode.Elements())
                                        {
                                            if (currentNode.Name.LocalName == "Name")
                                            {
                                                agentName = currentNode.Value;
                                            }
                                            else if (currentNode.Name.LocalName == "Plateform")
                                            {
                                                plateformeName = currentNode.Value;
                                            }
                                            else if (currentNode.Name.LocalName == "Port")
                                            {
                                                string ports = currentNode.Value;
                                                plateformePort = int.Parse(ports);
                                            }
                                        }

                                    }
                                    else
                                    {
                                        agentName = agentNode.Value;
                                    }

                                    string roleName = roleNode.Value;

                                    if (plateformeName == "localhost") plateformeName = agentPlateform.name;

                                    AID aid = new AID(agentName, plateformeName, plateformePort);

                                    Role role = null;

                                //    file.WriteLine("Nb Roles : " + structOrg.Roles.Count); file.Flush();

                                    foreach (Role currentRole in structOrg.Roles)
                                    {
                                     //   file.WriteLine(currentRole.name + " == " + roleName); file.Flush();
                                        if (currentRole.name == roleName)
                                        {
                                            role = currentRole;
                                            break;
                                        }
                                    }
                                    if (role == null)
                                    {  }
                                    else
                                    {
                                     //   file.WriteLine("Assign"); file.Flush();
                                        RoleAssignement assignement = new RoleAssignement(organisation);
                                        assignement.Agent = aid;
                                        assignement.Role = role;
                                        organisation.RoleAssignement.Add(assignement);
                                    }

                                }
                                else if (attr != null && attr.Value == "Ressource")
                                {
                                    XElement ressourceNode = null, entityNode = null;
                                    string ressourceName = "";
                                    string entityName = "";

                                    foreach (XElement currentNode in affects.Elements())
                                    {
                                        if (currentNode.Name.LocalName == "Ressource")
                                            ressourceNode = currentNode;

                                        else if (currentNode.Name.LocalName == "Entity")
                                            entityNode = currentNode;
                                    }

                                    ressourceName = ressourceNode.Value;
                                    entityName = entityNode.Value;

                                    //Debug.Log( " Affectation Ressource : " + ressourceName + " = " + entityName );
                                    // Recherche de l'entite .....
                                  //  MascaretApplication.Instance.logfile.WriteLine("Ressource affect looking entity : " + entityName);
                                  //  MascaretApplication.Instance.logfile.Flush();
                                    InstanceSpecification entite = environment.getInstance(entityName);
                                    if (entite == null) {
                                     //   MascaretApplication.Instance.logfile.WriteLine("Not found");
                                     //   MascaretApplication.Instance.logfile.Flush();
                                    }

                                    // Recherche de la ressource
                                    Ressource res = null;
                                    //Debug.Log( " *** " + structOrg.Ressources.Count );
                                    foreach (Ressource currentRes in structOrg.Ressources)
                                    {
                                     //   MascaretApplication.Instance.logfile.WriteLine( "Resource search: comparing " + currentRes.name + " to " + ressourceName );
                                     //   MascaretApplication.Instance.logfile.Flush();
                                        if (currentRes.name == ressourceName)
                                        {
                                          //  MascaretApplication.Instance.logfile.WriteLine("found");
                                          //  MascaretApplication.Instance.logfile.Flush();
                                            res = currentRes;
                                            break;
                                        }
                                    }

                                   
                                    // Creation de l'assignement
                                    RessourceAssignement ra = new RessourceAssignement();
                                    ra.Entity = ((Entity)entite);
                                    ra.Ressource = res;

                                    organisation.RessourcesAssignement.Add(ra);

                                }
                            }
                        }
                    }
                }
            }
            
            return organisation;
        }

        protected VirtualHuman _createHuman(XElement node)
        {


            VirtualHuman h = null;
            Class classe = null;

            string className = "";

            XAttribute attr = (XAttribute)node.Attribute("class");
            if (attr != null) className = attr.Value;

            if (className == "")
                className = "undef";

            classe = _findClass(className);
            string nodeName = "";
            attr = (XAttribute)node.Attribute("name");
            if (attr != null) nodeName = attr.Value;

            if (classe == null)
            {
                return h;
            }
            VirtualHumanClass vhClasse = (VirtualHumanClass)classe;
            if (vhClasse == null)
            {
                return h;
            }
            h = new VirtualHuman(agentPlateform, nodeName, vhClasse);
            agentPlateform.addAgent(h);

            h.ControlledByHuman = false; //default

            attr = (XAttribute)node.Attribute("controlled");
            if (attr != null && attr.Value == "yes")
            {
                h.ControlledByHuman = true;
            }


            if (toRun)
            {
                foreach (Behavior currentBehaviour in classe.OwnedBehavior.Values)
                {
                    BehaviorScheduler.Instance.executeBehavior(currentBehaviour, h, new Dictionary<string, ValueSpecification>(), false);
                }
            }


            environment.addInstance(h);
            _readGeometryEntity(node, h.Body);

            foreach (XElement child in node.Elements())
            {
                if (child.Name.LocalName == "Face")
                {
                    string indiceFile = "";
                    string fdpFile = "";

                    indiceFile = ((XAttribute)child.Attribute("indiceFile")).Value;
                    fdpFile = ((XAttribute)child.Attribute("fdpFile")).Value;
                    //h.Body.Face.FdpDataFile = fdpFile;
                    //h.Body.Face.IndiceFile = indiceFile;

                    //file.WriteLine(h.Body.Face.FdpDataFile + " / " + h.Body.Face.IndiceFile);
                    //h.Body.Face.loadData();

                }
                else if (child.Name.LocalName == "Behavior")
                {
                    string beNodeName = "";

                    bool added = false;
                    bool start = true;
                    attr = (XAttribute)child.Attribute("start");
                    if (attr != null) start = Boolean.Parse(attr.Value);

                    XAttribute attr2 = (XAttribute)child.Attribute("name");
                    if (attr2 != null) beNodeName = attr2.Value;

                    attr = (XAttribute)child.Attribute("interval");
                    if (attr != null)
                    {
                        /*
                        double interval = double.Parse(attr.Value);
                        */
                        double interval = 0.2;

                        if (interval != 0.0f)
                        {

                            added = true;

                            h.addBehavior(beNodeName, interval, start);
                        }
                    }
                    if (!added)
                        h.addBehavior(beNodeName, 0, start);
                }
                /* Est-ce utilisé quelque part? Voir en détail le fonctionnement avec le pointeur sur fonction symbol*/
                else if (child.Name == "KnowledgeBase")
                {
                    string name = "";
				
                    attr = (XAttribute)child.Attribute("name");
                    if(attr!=null) name = attr.Value;
				
                    KnowledgeBase kb = h.KnowledgeBase;
                    kb.name = name;
                   
                    XElement envNode = child.Element("Environment");
                    if (envNode != null)
                    {
                        string url = ((XAttribute)envNode.Attribute("url")).Value;
                        Environment env = parseEnvironment(url);
                        kb.Environment = env;
                    }
                    else
                    {
                        MascaretApplication.Instance.VRComponentFactory.Log( " Agent has global knowledge on environment" );
                        kb.Environment = MascaretApplication.Instance.getEnvironment();
                    }
                    
                }
                else if (child.Name.LocalName == "Attribute")
                {
                    attr = (XAttribute)child.Attribute("name");
                    if (attr != null && h.Slots.ContainsKey(attr.Value))
                    {
                        XAttribute attr2 = (XAttribute)child.Attribute("value");
                        if (attr2 != null)
                        {
                            ValueSpecification val = h.Slots[attr.Value].DefiningProperty.createValueFromString(attr2.Value);
                            h.Slots[attr.Value].addValue(val);
                        }
                    }
                    else
                    {
                    }
                }
                else if (child.Name.LocalName == "ExecuteOperation")
                {

                    Operation operation = null;

                    attr = (XAttribute)child.Attribute("name");
                    if (attr != null) operation = h.Classifier.Operations[attr.Value];
                    //Parameters param;

                    List<ValueSpecification> param = new List<ValueSpecification>();
                    foreach (Parameter currentParam in operation.Parameters)
                    {
                        bool found = false;
                        string strVal = "";

                        foreach (XElement parameterNode in child.Elements())
                        {
                            if (parameterNode.Name.LocalName == "Parameter")
                            {
                                attr = (XAttribute)parameterNode.Attribute("name");
                                if (attr != null && currentParam.name == attr.Value)
                                {
                                    attr = (XAttribute)parameterNode.Attribute("name");
                                    if (attr != null) strVal = attr.Value;
                                    found = true;
                                    break;
                                }
                            }
                        }
                        if (!found)
                        {
                          
                            continue;
                        }
                        strVal.Replace("+", " ");
                        try
                        {
                            param.Add(currentParam.Type.createValueFromString(strVal));
                        }
                        catch (ArgumentOutOfRangeException)
                        {
                          
                        }
                    }
                    attr = (XAttribute)child.Attribute("name");
                    if (attr != null) h.executeOperation(attr.Value, param);

                    //Debug.Log( " Launch oper : " + operation.getName() + " on entity : " + h.getName() );
                    //BehaviorScheduler::getInstance().executeBehavior(operation.getMethod(),h,param);
                }
                else if (child.Name.LocalName == "Entity")
                {
                    Entity instance = _createEntity(child, h.Body);
                }

            }
            return h;
        }


        protected Environment parseEnvironment(string url)
        {
            Environment env = null;
            Model model = null;

            XDocument parser = null;
            string baseDir = MascaretApplication.Instance.BaseDir;
            parser = XDocument.Load(baseDir+"/"+url);
            XElement root = parser.Root;

            foreach (XElement childNode in root.Elements())
            {
                if (childNode.Name.LocalName == "Model")
                {
                    string urlModel = "";
                    XAttribute attr = (XAttribute)childNode.Attribute("url");
                    if (attr != null) urlModel = attr.Value;
                    model = parseModel(urlModel);
                }
            }
            if (model == null)
                model = this.model;

            env = new Environment(model);
            env.Url = url;
            model.addEnvironment(env);

            
            MascaretApplication.Instance.VRComponentFactory.Log(baseDir);

            //InstanceLoader instanceLoader = new InstanceLoader(agentPlateform, env, url + "/" + baseDir, false);
            InstanceLoader instanceLoader = new InstanceLoader(agentPlateform, env, baseDir + "/" + url, false);

            return env;
        }

        protected Model parseModel(string url)
        {
            string baseDir = MascaretApplication.Instance.BaseDir;

          //  ModelLoader2 modelLoader = new ModelLoader2(url + "/" + baseDir, true);
            ModelLoader2 modelLoader = new ModelLoader2(baseDir + "/" + url, true);
            Model model = modelLoader.Model;
            model.ModelLoader = modelLoader;

            return model;
        }



        protected Class _findClass(string className)
        {
            Class classe = null;

            classe = _findClassPackage(model.Package, className);
            return classe;
        }


        protected Class _findClassPackage(Package package, string className)
        {
            Class classe = null;

            foreach (Class currentClass in package.Classes.Values)
            {
                if (currentClass.name == className)
                {
                    classe = currentClass;
                    break;
                }
            }

            if (classe == null)
            {
                foreach (Package currentPackage in package.Packages.Values)
                {
                    classe = _findClassPackage(currentPackage, className);
                    if (classe != null)
                        break;
                }
            }
            return classe;
        }


        protected void _addRelations()
        {
          //  StreamWriter file = MascaretApplication.Instance.logfile;

            //Debug.Log( "Add Relation" );
            foreach (Relation currentRelation in relations)
            {
                if (currentRelation.e.getProperty(currentRelation.property) != null)
                {
                   // file.WriteLine("Relation : " + currentRelation.e.getFullName() + " . " + currentRelation.property + " = " + currentRelation.rValue); file.Flush();
                    Property p = currentRelation.e.getProperty(currentRelation.property).DefiningProperty;
                   // file.WriteLine(p.Type.getFullName()); file.Flush();
                    currentRelation.e.getProperty(currentRelation.property).addValueFromString(currentRelation.rValue);
                }
            }

        }
    }
}
