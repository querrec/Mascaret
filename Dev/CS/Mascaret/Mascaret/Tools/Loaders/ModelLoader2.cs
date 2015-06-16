using System;
using System.IO;
using System.Xml.Linq;
using System.Collections.Generic;
using System.Collections;


//VERIFIER SI TOUT A BESOIN DETRE SERIALIZER OU NON!!!!!

namespace Mascaret
{
    public class ModelLoader2 
{


	public struct ConstraintT{
	  public string id;
	  public string constrainedElementID;
	  public string specification;
	}

	 
	private List <OrganisationalStructure> orgStructure = new List<OrganisationalStructure>();
	public List <OrganisationalStructure> OrgStructure
	{
		get{
			return MascaretApplication.Instance.AgentPlateform.Structures;
		}
	}
	
	//STD::SET REMPLACER PAR LIST (DICTIONNAIRE TROP GOURMAND EN MEMOIRE)! 
	//CONVERSION PAS TOUT A FAIT JUSTE!!!!! SET N'AUTORISE QUE DES VALEURS UNIQUES
	 
	protected List<string> _stereoEntities=new List<string>();
	 
	protected List<string> _stereoAgents=new List<string>();
	 
	protected List<string> _stereoVirtualHumans=new List<string>();
	 
	protected List<string> _stereoRoles=new List<string>();
	 
	protected List<string> _stereoPreconditions=new List<string>();
	 
	protected List<string> _stereoPostconditions=new List<string>();
	 
	protected List<string> _stereoBuiltIn=new List<string>();
	 
	protected List<string> _stereoNonInterupt=new List<string>();
	 
	protected List<string> _stereoBlocks=new List<string>();

    protected Dictionary<string, List<String>> _otherStereo = new Dictionary<string,List<string>>();

	 
	private Dictionary<String,Class> _idClass;

	private XDocument _loader = null;

	private XElement _xmi=null;

	 
	private Model model=null;
	public Model Model
	{
		get{return model;}
		set{model = value;}
	}
	
	 
	public List<KeyValuePair<string, string > > _opaqueBehaviorsCode=new List<KeyValuePair<string,string >>();

	 
	public List<KeyValuePair<string,string > > _toGeneralize=new List<KeyValuePair<string,string >>();
	
	
	private Dictionary <string, List<KeyValuePair<Class, XElement> > > _idAsso = new Dictionary<string, List<KeyValuePair<Class, XElement>>>();
	//private Dictionary<String, List<Property> > _idAsso;
	
	 
	private Dictionary<String,Operation> _idOperations;
	
	 
	private Dictionary<String,Behavior> _idBehaviors;
	
	 
	private Dictionary<String,Parameter> _parameters;
	
	 
	private Dictionary<String,ActivityPartition> _partitions;
	
	 
	private Dictionary<String,String> _paramToType;
	
	 
	private Dictionary<String,Classifier> _classifiers;
	
	 
	private Dictionary<CallOperationAction,String> _callOperations;
	
	 
	private Dictionary<CallBehaviorAction,String> _callBehaviors;
	
	 
	private Dictionary<CallEvent,String> _callEvents;
	
	 
	private List<ConstraintT> _constraints=new List<ConstraintT>();
	
	 
	private Dictionary<String,Signal> _signals;
	
	
	 
	private Dictionary<String,MascaretEvent> _events;
	
	 
	private Dictionary<String,ActivityNode> _activityNodes;
	
	 
	private Dictionary<String,String> _waitingNodes;
	
	 
	private Dictionary<String,ObjectNode> _objectNodes;
	
	
	 
	private Dictionary<String,Expression> _activityExpressions;
	
	 
	private List<KeyValuePair<XNode,Package > > _activities=new List<KeyValuePair<XNode,Package > >();
	
	
	 
	private Dictionary<String,Activity> _activitiesMethod;

	 
	private List<KeyValuePair<XNode,Package> > _stateMachines=new List<KeyValuePair<XNode,Package> >();
	
	
	 
    private Dictionary<String,StateMachine> _subStateMachines;
	
	
	 
    private Dictionary<String,State>  _subStates;
	
	 
    private Dictionary<String,String> _subStatesID;
	
	//default parameter strIsFileName=true;
	public ModelLoader2(string str, bool strIsFileName)
	{
		_idClass = new Dictionary<String,Class>();
		//_idAsso =  new Dictionary<string, Property>();
		//_idAsso =  new MapStringProperties();
		_idOperations= new Dictionary<string, Operation>();
		_idBehaviors =  new Dictionary<string, Behavior>();
		_parameters =  new Dictionary<string, Parameter>();
		_partitions =  new Dictionary<string, ActivityPartition>();
		_paramToType=  new Dictionary<string, string>();
		_classifiers =  new Dictionary<string, Classifier>();
		_callOperations= new Dictionary<CallOperationAction, string>();
		_callBehaviors= new Dictionary<CallBehaviorAction, string>();
		_callEvents= new Dictionary<CallEvent, string>();
		_signals=  new Dictionary<string, Signal>();
		_events =  new Dictionary<string, MascaretEvent>();
		_activityNodes =  new Dictionary<string, ActivityNode>();
		_waitingNodes =  new Dictionary<string, string>();
		_objectNodes =  new Dictionary<string, ObjectNode>();
		_activityExpressions =  new Dictionary<string, Expression>();
		_activitiesMethod=  new Dictionary<string, Activity>();
		_subStateMachines =  new Dictionary<string, StateMachine>();
		_subStates =  new Dictionary<string, State>();
		_subStatesID =  new Dictionary<string, string>();
		_loader = new XDocument();

		try
		{
            String s = MascaretApplication.Instance.readFlow(str);
			_loader = XDocument.Parse(s);
			_xmi = _loader.Root;

           // MascaretApplication.Instance.logfile.WriteLine("XMI : " + _xmi.Name.LocalName.ToLower()); MascaretApplication.Instance.logfile.Flush();
					
			//Debug.Log (_xmi.Name);
			
			XElement modelNode = null;


         //   MascaretApplication.Instance.logfile.WriteLine("Parsing Model"); MascaretApplication.Instance.logfile.Flush();
			if ((_xmi.Name.LocalName.ToLower().CompareTo("uml:model")==0) || (_xmi.Name.LocalName.ToLower().CompareTo("model")==0))
            {
               // MascaretApplication.Instance.logfile.WriteLine("OK"); MascaretApplication.Instance.logfile.Flush();
				modelNode = _xmi;
            }
			else 
			{	
				foreach(XElement currentNode in _xmi.Elements())
				{	
					if(currentNode.Name.LocalName.ToLower().CompareTo("model")==0)
					{
						modelNode = currentNode;
					}
				}
			}
			
			if(modelNode!=null)
			{
                
				XAttribute attr = (XAttribute)modelNode.Attribute("name");

               // MascaretApplication.Instance.logfile.WriteLine("ModelName : " + attr.Value); MascaretApplication.Instance.logfile.Flush();

				if (MascaretApplication.Instance.hasModel(attr.Value)) {
					model = MascaretApplication.Instance.getModel(attr.Value);
				} else 
				{
					model = new Model(attr.Value);
					if (strIsFileName)
						model.XmiUrl = str;
		
					MascaretApplication.Instance.addModel(model);

					
					addStereotypes();
					
					addPackage(modelNode,null);
		
					_setParametersType();
					addGeneralizations();
					addAssociation();
					addActivitiesAndStateMachines();
					addCallOperations();
                    addCallBehaviors();
					addConditions();
					
					EntityClass curEntityClass = new EntityClass("undef");
					Class classe = (Class)curEntityClass;
					model.Package.addClasses(classe);
		
					model.registerAllClasses();
				}
			}
		} catch(FileLoadException e) {
		}

		// Bouml preserved body end 0001F4E7
	}		
	
	void addPackage(XElement packageNode, Package parent)
	{
		// Bouml preserved body begin 0001F567
       // StreamWriter file = MascaretApplication.Instance.logfile;

       // file.WriteLine("Parsing Packages ..."); file.Flush();

		XAttribute typeAttr =null, pkgNameAttr = null;
		string type="",pkgName="";
		typeAttr = (XAttribute)packageNode.Attribute("{http://schema.omg.org/spec/XMI/2.1}type");
        if (typeAttr == null) typeAttr = (XAttribute)packageNode.Attribute("{http://www.omg.org/spec/XMI/20131001}type");
		if(typeAttr!=null)type = typeAttr.Value;

        if (typeAttr == null && ( (packageNode.Name.LocalName.CompareTo("Model") == 0) || (packageNode.Name.LocalName.CompareTo("uml:Model") == 0)))
			type = "uml:Model";
	
		pkgNameAttr = (XAttribute)packageNode.Attribute("name");
		if(pkgNameAttr!=null)pkgName = pkgNameAttr.Value;

       // file.WriteLine("type : " + type); file.Flush();

		if (String.Compare(type,"uml:Model")!=0 && String.Compare(type,"uml:Package")!=0) {
			return;
		}
	
		Package pkg=null;
		if (String.Compare(type,"uml:Package")==0 || String.Compare(type,"uml:Model")==0) {
			pkg = new Package(pkgName);
	
			pkg.Description=getComment(packageNode);
			pkg.Summary=getSummary(packageNode);
			pkg.Tags=getTags(packageNode);
	
			if (parent!=null)
			{
				//Debug.Log ("Package got parent");
				parent.addPackages(pkg);
			}
			else
			{
				//Debug.Log("Package is root" );
				model.Package=pkg;
			}
		}
		
		foreach (XElement child in packageNode.Elements()) {

			if (child.Name.LocalName.CompareTo("packagedElement")==0 || child.Name.LocalName.CompareTo("ownedMember")==0) 
			{
				string childType = "";
				
				XAttribute attr = (XAttribute)child.Attribute("{http://schema.omg.org/spec/XMI/2.1}type");
                if (attr == null) attr = (XAttribute)child.Attribute("{http://www.omg.org/spec/XMI/20131001}type");
				if(attr!=null)childType=attr.Value;

				//Debug.Log("================    " + childType);
				if (childType.CompareTo("DataType")==0 || childType.CompareTo("PrimitiveType")==0)
					addDatatype(child);
                else if (childType.CompareTo("uml:Signal") == 0)
                {
                    addSignal(child, pkg);
                }
			} 
			else if (child.Name.LocalName.CompareTo("ownedRule")==0) {
				string childType = "";
				
				XAttribute attr = (XAttribute)child.Attribute("{http://schema.omg.org/spec/XMI/2.1}type");
                if (attr == null) attr = (XAttribute)child.Attribute("{http://www.omg.org/spec/XMI/20131001}type");
				if(attr!=null)childType=attr.Value;
				
				if (childType.CompareTo("Constraint")==0) {
					ConstraintT constraint=new ConstraintT();
					
					attr = (XAttribute)child.Attribute("id");
					if(attr!=null)constraint.id = attr.Value;
					
					attr = (XAttribute)child.Attribute("constrainedElement");
					if(attr!=null)constraint.constrainedElementID = attr.Value;
					
					foreach(XElement specifNode in child.Elements())
					{
						if(specifNode.Name.LocalName.CompareTo("specification")==0)
						{
							attr = (XAttribute)specifNode.Attribute("value");
							if(attr!=null)constraint.specification =attr.Value;
							_constraints.Add(constraint);
							break;
						}
					}
				}
			}
		}

		foreach (XElement child in packageNode.Elements()) {
			
			if (child.Name.LocalName.CompareTo("packagedElement")==0 || child.Name.LocalName.CompareTo("ownedMember")==0) {
				XAttribute attr = (XAttribute)child.Attribute("{http://schema.omg.org/spec/XMI/2.1}type");
                if (attr == null)
                    attr = (XAttribute)child.Attribute("{http://www.omg.org/spec/XMI/20131001}type");
           //     file.WriteLine("Child : " + child.Name.LocalName); file.Flush();
           //     file.WriteLine("Type : " + attr.Value); file.Flush();

				//Debug.Log("ATTR : " + attr.Name + " == " + attr.Value);
				if (attr!=null && attr.Value.CompareTo("uml:Package")==0)
					addPackage(child, pkg);
	
			}
		}


		foreach (XElement child in packageNode.Elements()) {
			
			if (child.Name.LocalName.CompareTo("packagedElement")==0 || child.Name.LocalName.CompareTo("ownedMember")==0) {
				string childType = "";
				
				XAttribute attr = (XAttribute)child.Attribute("{http://schema.omg.org/spec/XMI/2.1}type");
                if (attr == null) attr = (XAttribute)child.Attribute("{http://www.omg.org/spec/XMI/20131001}type");
				if(attr!=null)childType=attr.Value;
				
				if (childType.CompareTo("uml:SignalEvent")==0)
					addSignalEvent(child, pkg);
				if (childType.CompareTo("uml:CallEvent")==0)
					addCallEvent(child, pkg);
				if (childType.CompareTo("uml:ChangeEvent")==0)
					addChangeEvent(child, pkg);
				if (childType.CompareTo("uml:TimeEvent")==0)
					addTimeEvent(child, pkg);
			}
		}

		foreach (XElement child in packageNode.Elements()) {
			
			if (child.Name.LocalName.CompareTo("packagedElement")==0 || child.Name.LocalName.CompareTo("ownedMember")==0) {
				string childType = "";
				
				XAttribute attr = (XAttribute)child.Attribute("{http://schema.omg.org/spec/XMI/2.1}type");
                if (attr == null) attr = (XAttribute)child.Attribute("{http://www.omg.org/spec/XMI/20131001}type");
				if(attr!=null)childType=attr.Value;
				
				if (childType.CompareTo("uml:Class")==0 || childType.CompareTo("uml:Interface")==0)
					addClass(child, pkg);
				else if (childType.CompareTo("Activity")==0)
					_activities.Add(new KeyValuePair<XNode,Package >(child, pkg));
				//addActivity(child,pkg);
				else if (childType.CompareTo("StateMachine")==0)//StateMachine
					_stateMachines.Add(new KeyValuePair<XNode ,Package > (child, pkg));
				//addStateMachine(child,pkg);
			}				
		}

        foreach (XElement child in packageNode.Elements())
        {
            if (child.Name.LocalName.CompareTo("packagedElement") == 0 || child.Name.LocalName.CompareTo("ownedMember") == 0)
            {
                XAttribute attr = (XAttribute)child.Attribute("{http://schema.omg.org/spec/XMI/2.1}type");
                if (attr == null) attr = (XAttribute)child.Attribute("{http://www.omg.org/spec/XMI/20131001}type");

                if (attr != null && attr.Value.CompareTo("uml:Association") == 0)
                {
                    XAttribute id = (XAttribute)child.Attribute("{http://www.omg.org/spec/XMI/20131001}id");
                    if (id != null)
                    {
                        if (_idAsso.ContainsKey(id.Value))
                        {
                            XElement owned = child.Element("ownedEnd");
                            if (owned != null)
                            {
                                XElement clElement = _idAsso[id.Value][0].Value;
                                XAttribute typeA = clElement.Attribute("type");
                                Class cl = _idClass[typeA.Value];
                                _idAsso[id.Value].Add(new KeyValuePair<Class, XElement>(cl, owned));


                            }
                        }
                    }
                }
            }
        }

		foreach (XElement child in packageNode.Elements()) {
			if (child.Name.LocalName.CompareTo("packagedElement")==0 || child.Name.LocalName.CompareTo("ownedMember")==0) {
				XAttribute attr = (XAttribute)child.Attribute("{http://schema.omg.org/spec/XMI/2.1}type");
                if (attr == null) attr = (XAttribute)child.Attribute("{http://www.omg.org/spec/XMI/20131001}type");

				if (attr!=null && attr.Value.CompareTo("uml:Collaboration")==0)
				{	
					addOrganisationalStructure(child);
				}
			}
		}

		//add signals
		//Debug.Log ("ADDING SIG IN PKG : " + pkg.getFullName());
		foreach (Signal itSig in _signals.Values)
		{	//Debug.Log(" ----> Signal : " + itSig);
			pkg.addSignals(itSig);
		}
		//_signals.clear();
		
		// Bouml preserved body end 0001F567
		
	}


	public void addClass(XElement classNode, Package pkg)
	{
        //StreamWriter file = new StreamWriter("log2.txt");
        
		string type="", className="", id="";
	
		XAttribute attr = (XAttribute)classNode.Attribute("{http://schema.omg.org/spec/XMI/2.1}type");
        if (attr == null) attr = (XAttribute)classNode.Attribute("{http://www.omg.org/spec/XMI/20131001}type");

		if(attr!=null)type=attr.Value;
			
		attr = (XAttribute)classNode.Attribute("name");
		if(attr!=null)className=attr.Value;

		
		attr = (XAttribute)classNode.Attribute("{http://schema.omg.org/spec/XMI/2.1}id");
        if (attr == null) attr = (XAttribute)classNode.Attribute("{http://www.omg.org/spec/XMI/20131001}id");

		if(attr!=null)id=attr.Value;

		if (type.CompareTo("uml:Class")!=0 && type.CompareTo("uml:Interface")!=0) {

			return;
		}
	
		bool entity = false;
		bool agent = false;
		bool role = false;
		Class cl=null;

       // file.WriteLine(" Class : " + className);
       // file.Flush();
       // file.Close();
		

		if (isStereotypedEntity(classNode)) {
			cl = new EntityClass(className);
			entity = true;
		} else if (isStereotypedBlock(classNode)) {
			cl = new Block(className);
			entity = true;
		} else if (isStereotypedAgent(classNode)) {
			cl = new AgentClass(className);
			agent = true;
		} else if (isStereotypedVirtualHuman(classNode)) {
			cl = new VirtualHumanClass(className);
		} else if (isStereotypedRole(classNode)) {
			cl = new RoleClass(className);
			role = true;
		} else {
			cl = new EntityClass(className);
		}
	

		_idClass.Add(id,cl);
		
		_classifiers.Add(id,cl);
		//Debug.Log (" Adding Class : " + _classifiers.convertToDictionary().Count + " : " + id);
		if (pkg!=null)
			pkg.addClasses(cl);
		cl.Description = getComment(classNode);
		cl.Summary=getSummary(classNode);
		cl.Tags=getTags(classNode);
	
		foreach (XElement child in classNode.Elements()) {
			if (child.Name.LocalName.CompareTo("ownedOperation")==0)
				addOperation(child, cl);
			else if (child.Name.LocalName.CompareTo("ownedAttribute")==0)
				addAttribute(child, cl);
			else if (child.Name.LocalName.CompareTo("generalization")==0) {
				XAttribute attr2 = (XAttribute)child.Attribute("general");
				if(attr2!=null)
					_toGeneralize.Add(new KeyValuePair<string,string>(id, attr2.Value));
			}
		}
		
		foreach(KeyValuePair <CallEvent, string> pair in _callEvents){
			
			if(_idOperations.ContainsKey(pair.Value))
			{	
				Operation oper = _idOperations[pair.Value];
				// TO DO_callEvents.getKey(pair.Key).Operation=oper;
			}
		}
	
		foreach (XElement child in classNode.Elements()) {
			//Debug.Log(" Parsing class : " + cl.getFullName() + "/ Child : " + child.Name.LocalName);

			if (child.Name.LocalName.CompareTo("ownedBehavior")==0 || child.Name.LocalName.CompareTo("nestedClassifier")==0) 
			{
               // MascaretApplication.Instance.logfile.WriteLine(" Owned Behavior"); MascaretApplication.Instance.logfile.Flush();
				XAttribute attr2 = (XAttribute)child.Attribute("{http://schema.omg.org/spec/XMI/2.1}type");
                if (attr2 == null) attr2 = (XAttribute)child.Attribute("{http://www.omg.org/spec/XMI/20131001}type");
               // MascaretApplication.Instance.logfile.WriteLine(attr2.Value); MascaretApplication.Instance.logfile.Flush();


				//Debug.Log(" Parsing class : " + cl.getFullName() + "/ Attr : " + attr2);
				if(attr2!=null && attr2.Value.CompareTo("uml:StateMachine")==0)
					addStateMachineToClass(child, cl);
				
				if(attr2!=null && attr2.Value.CompareTo("uml:Activity")==0)
					addActivityToClass(child, cl);
			}
		}
     
		// Bouml preserved body end 0001F5E7
	}

	public void addOrganisationalStructure(XElement orgNode)
	{
		
		XAttribute orgName = (XAttribute)orgNode.Attribute("name");


		
	
		OrganisationalStructure organisation = new OrganisationalStructure(orgName.Value);
		organisation.Description=getComment(orgNode);
		organisation.Summary=getSummary(orgNode);
		organisation.Tags=getTags(orgNode);
	
		//Debug.Log(MascaretApplication.Instance.Model.name);
		
		MascaretApplication.Instance.AgentPlateform.Structures.Add(organisation);
	
		foreach (XElement child in orgNode.Elements()) {
			if (child.Name.LocalName.CompareTo( "ownedAttribute")==0) {
				
				string childType="", roleName="";
				Classifier classe=null;
				
				XAttribute attr = (XAttribute)child.Attribute("type");
				
				if(attr!=null)
				{
					childType = attr.Value;
					classe = this._classifiers[childType];
				}
				
				attr =  (XAttribute)child.Attribute("name");
				if(attr!=null)roleName = attr.Value;


				
				if (classe!=null)
				{
					try{
						
						Role role = new Role(roleName);
						role.RoleClass = (RoleClass)classe;
						organisation.addRole(role);
						
					}catch(InvalidCastException e)
					{
						
						try{
							Ressource ressource = new Ressource(roleName);
							ressource.EntityClass=(EntityClass)classe;
							organisation.addResource(ressource);
							
						}catch(InvalidCastException e2)
						{
						}
					}
				}
			}
		}
	
		foreach (XElement child in orgNode.Elements()) {
			if (child.Name.LocalName.CompareTo( "ownedBehavior")==0) {
				
				string childType="",childName="", childId="";
				
				XAttribute attr = (XAttribute)child.Attribute("{http://schema.omg.org/spec/XMI/2.1}type");
                if (attr == null) attr = (XAttribute)child.Attribute("{http://www.omg.org/spec/XMI/20131001}type");

				if(attr!=null)childType = attr.Value;
				
				attr =  (XAttribute)child.Attribute("name");
				if(attr!=null)childName = attr.Value;
				
				attr =  (XAttribute)child.Attribute("{http://schema.omg.org/spec/XMI/2.1}id");
                if (attr == null) attr = (XAttribute)child.Attribute("{http://www.omg.org/spec/XMI/20131001}id");

				if(attr!=null)childId = attr.Value;
	
				if (childType.CompareTo("uml:Activity")==0) {
					Activity activity = addActivity(child);
					Procedure procedure = new Procedure(childName);
					procedure.Activity=activity;
					organisation.addProcedure(procedure);
					_idBehaviors.Add(childId,activity);
				}
			}

		}
	}

	public void addSignal(XElement signalNode, Package pkg)
	{
		// Bouml preserved body begin 0001F5E7

      //  StreamWriter file = new StreamWriter("signal.txt");

		string type="", className="",id="";
	
		XAttribute attr = (XAttribute)signalNode.Attribute("{http://schema.omg.org/spec/XMI/2.1}type");
        if (attr == null) attr = (XAttribute)signalNode.Attribute("{http://www.omg.org/spec/XMI/20131001}type");

		if(attr!=null)type=attr.Value;



		attr = (XAttribute)signalNode.Attribute("name");
		if(attr!=null)className=attr.Value;
		
		if (type.CompareTo("uml:Signal")!=0) {
			//Debug.Log( className + " is a " + type + " and must be a signal");
			return;
		}
		
		attr = (XAttribute)signalNode.Attribute("{http://schema.omg.org/spec/XMI/2.1}id");
        if (attr == null) attr = (XAttribute)signalNode.Attribute("{http://www.omg.org/spec/XMI/20131001}id");

		if(attr!=null)id=attr.Value;
	
		//Debug.Log( " ############## Add Signal : " + className );
	
		Signal signal = new Signal(className);

    //    file.WriteLine("Signal : " + className); file.Flush();
    //    file.Close();
        if (_signals.ContainsKey(id)) return;
		_signals.Add(id,signal);
	
		if (pkg!=null)
			pkg.addSignals(signal);
		signal.Description=getComment(signalNode);
		signal.Summary=getSummary(signalNode);
		signal.Tags=getTags(signalNode);
		
		foreach (XElement child in signalNode.Elements()) {
			string childName = child.Name.LocalName;
	
			if (childName.CompareTo("ownedOperation")==0)
				addOperation(child, signal);
			else if (childName.CompareTo("ownedAttribute")==0) {
				//Debug.Log( "AddAttribute SIGNAL" );
				addAttribute(child, signal);
			}
		}
	
		foreach (XElement child in signalNode.Elements()) {
			string childName = child.Name.LocalName;
			
			XAttribute attr2 = (XAttribute)child.Attribute("{http://schema.omg.org/spec/XMI/2.1}type");
            if (attr == null) attr = (XAttribute)child.Attribute("{http://www.omg.org/spec/XMI/20131001}type");

			if(attr2!=null)
			{
				if (childName.CompareTo("ownedBehavior")==0) {
					if (attr2.Value.CompareTo("uml:StateMachine")==0)
						addStateMachineToClass(child, signal);
				} else if (childName.CompareTo("nestedClassifier")==0) {
					if (attr2.Value.CompareTo("uml:StateMachine")==0)
						addStateMachineToClass(child, signal);
				}
			}
		}
		// Bouml preserved body end 0001F5E7
	}
	
	public void addSignalEvent(XElement signalNode, Package pkg)
	{
		// Bouml preserved body begin 0001F5E7
	
		string type="", className="",id="";
	
		XAttribute attr = (XAttribute)signalNode.Attribute("{http://schema.omg.org/spec/XMI/2.1}type");
        if (attr == null) attr = (XAttribute)signalNode.Attribute("{http://www.omg.org/spec/XMI/20131001}type");

		if(attr!=null)type=attr.Value;
		
		attr = (XAttribute)signalNode.Attribute("{http://schema.omg.org/spec/XMI/2.1}id");
        if (attr == null) attr = (XAttribute)signalNode.Attribute("{http://www.omg.org/spec/XMI/20131001}id");

		if(attr!=null)id=attr.Value;
		
		attr = (XAttribute)signalNode.Attribute("name");
		if(attr!=null)className=attr.Value;
		
		if (type.CompareTo("uml:SignalEvent")!=0) {
			//Debug.Log(className + " is a " + type + " and must be a signal");
			return;
		}
				
		SignalEvent signalEvent = new SignalEvent(className);
		
		attr = (XAttribute)signalNode.Attribute("signal");
		if(attr!=null)
		{
			if(_signals.ContainsKey(attr.Value))
			{
				Signal sig = _signals[attr.Value];
				signalEvent.SignalClass=_signals[attr.Value];
			}				
			
		}
		
		_events.Add(id ,signalEvent);
		
		// Bouml preserved body end 0001F5E7
	}
	
	public void addCallEvent(XElement callNode, Package pkg)
	{
		// Bouml preserved body begin 0001F5E7
		
		string type="", className="",id="";
	
		XAttribute attr = (XAttribute)callNode.Attribute("{http://schema.omg.org/spec/XMI/2.1}type");
        if (attr == null) attr = (XAttribute)callNode.Attribute("{http://www.omg.org/spec/XMI/20131001}type");

		if(attr!=null)type=attr.Value;
		
		attr = (XAttribute)callNode.Attribute("name");
		if(attr!=null)className=attr.Value;
		
		if (type.CompareTo("uml:CallEvent")!=0) {
			//Debug.Log(className + " is a " + type + " and must be a signal");
			return;
		}
		
		attr = (XAttribute)callNode.Attribute("id");
		if(attr!=null)id=attr.Value;
		
		CallEvent callEvent = new CallEvent(id);
		attr = (XAttribute)callNode.Attribute("operation");
	
		if(attr!=null)_callEvents.Add(callEvent,attr.Value);
	
		
		_events.Add(id,callEvent);
		// Bouml preserved body end 0001F5E7
	}
	
	public void addTimeEvent(XElement timeNode, Package pkg)
	{
		string type;

		type = timeNode.Attribute("{http://schema.omg.org/spec/XMI/2.1}type").Value;
        if (type == null) type = timeNode.Attribute("{http://www.omg.org/spec/XMI/20131001}type").Value;

		if (type != "uml:TimeEvent") {
			return;
		}

		string id = timeNode.Attribute("id").Value;

		TimeEvent timeEvent = new TimeEvent(id);

		XElement when = timeNode.Element("when");
		if (when != null && when.Element("expr") != null) 
		{
			string timeExp = when.Element("expr").Attribute("value").Value;
			LiteralReal time = new LiteralReal(timeExp);
			timeEvent.When = time;
		}
		/* add in package*/
		_events.Add(id,timeEvent);
		
	}

	public void addChangeEvent(XElement changeNode, Package pkg)
	{
		string type = "";

        if (changeNode.Attribute("{http://schema.omg.org/spec/XMI/2.1}type") != null)
		    type = changeNode.Attribute("{http://schema.omg.org/spec/XMI/2.1}type").Value;
        else if (changeNode.Attribute("{http://www.omg.org/spec/XMI/20131001}type")!= null) 
            type = changeNode.Attribute("{http://www.omg.org/spec/XMI/20131001}type").Value;

        MascaretApplication.Instance.VRComponentFactory.Log("Type = " + type);
		if (type != "uml:ChangeEvent") 
		{
			return;
		}

        string id = "";
        if (changeNode.Attribute("{http://schema.omg.org/spec/XMI/2.1}id") != null)
            id = changeNode.Attribute("{http://schema.omg.org/spec/XMI/2.1}id").Value;
        else if (changeNode.Attribute("{http://www.omg.org/spec/XMI/20131001}id") != null)
            id = changeNode.Attribute("{http://www.omg.org/spec/XMI/20131001}id").Value;

		ChangeEvent changeEvent = new ChangeEvent(id);
		
		if (changeNode.Element("changeExpression") != null) 
		{
            MascaretApplication.Instance.VRComponentFactory.Log("New Change Event : " + id);

			string changeExp = changeNode.Element("changeExpression").Attribute("value").Value;
            
			Expression exp = new Expression(changeExp,this.Model.BasicTypes["boolean"]);
			changeEvent.ChangeExpression = exp;
		}
		/* add in package*/
		_events.Add(id,changeEvent);
	}

	//TODO: choper contenu de data (tag? description?)

	public void addOperation(XElement opNode, Class cl)
	{
		// Bouml preserved body begin 0001FAE7
		
		string opName="";
		
		XAttribute attr = (XAttribute)opNode.Attribute("name");
		if(attr!=null)opName=attr.Value;
	
		// Pour l'instant que Opaque Behavior (C++ / Python)
		OpaqueBehavior b=null;
		string datas="";
		//default C++
		string opClassName = cl.name + "_" + opName;
		//        xmiOperation.getTaggedValue("C++",data);
		//TODO choper contenu data
		if (datas.CompareTo(String.Empty)!=0) {
			if (datas.Contains("{PYTHON}")) {
				string python = "{PYTHON}";
				datas = datas.Substring(python.Length);
			} else if (datas.Contains("{OCL}")) {
				string ocl = "{OCL}";
				datas = datas.Substring(ocl.Length);

				//b = new OclQuery(opClassName);
				//b.Body=datas;
	
			} else {
				_opaqueBehaviorsCode.Add(new KeyValuePair<string,string>(opClassName, datas));
				//b=new OpaqueBehavior(opClassName); b.Body=opClassName;
				b = new OpaqueBehavior(opName);
				b.Body=opName;
			}
		} else {
			b = new OpaqueBehavior(opName);
			b.Body=opName;
		}
		
		Operation operation = new Operation(opName, cl,b);
		_addParameters(operation, opNode);
		
		attr = (XAttribute)opNode.Attribute("{http://schema.omg.org/spec/XMI/2.1}id");
        if (attr == null) attr = (XAttribute)opNode.Attribute("{http://www.omg.org/spec/XMI/20131001}id");

		if(attr!=null)
		{
			//Debug.Log(" Blurp : " + attr.Value);
			_idOperations.Add(attr.Value,operation);
		}
		
		//Debug.Log opNode.getProperty("id") );
		//TODO : normalement a devrait etre fait par le new Operation
		cl.addOperation(operation);
		operation.Description=getComment(opNode);
		operation.Summary=getSummary(opNode);
		operation.Tags=getTags(opNode);
		// Bouml preserved body end 0001FAE7
	}

	public void addAttribute(XElement attrNode, Class cl)
	{
		// Bouml preserved body begin 0001FB67
		//Debug.Log(" -------- Add Attribute ");
		XElement typeNode = null;
	
		foreach(XElement child in attrNode.Elements())
		{
			if(child.Name.LocalName.CompareTo("type")==0)
			{
				typeNode = child;
				break;
			}
		}
		if (isStereotypedBuiltIn(attrNode)) {//TODO Pour le moment on ignore les propri�t�s builtIn
			return;
		}
		
		string assoId="";
		
		XAttribute attr = (XAttribute)attrNode.Attribute("association");
        if (attr != null)
        {
            assoId = attr.Value;
        }
        else
        {
            attr = (XAttribute)attrNode.Attribute("{http://schema.omg.org/spec/XMI/2.1}id");
            if (attr == null) attr = (XAttribute)attrNode.Attribute("{http://www.omg.org/spec/XMI/20131001}id");

            if (attr != null) assoId = attr.Value;
        }

		//add at end of package
		//Debug.Log ("ASSO : " + assoId);
		
		if(!_idAsso.ContainsKey(assoId))
		{
			List<KeyValuePair<Class, XElement>> liste = new List<KeyValuePair<Class, XElement>>();
			liste.Add(new KeyValuePair<Class, XElement>(cl, attrNode));
			_idAsso.Add(assoId,liste);
		}
		else
		{
			_idAsso[assoId].Add(new KeyValuePair<Class, XElement>(cl, attrNode));
		}
			
		// Bouml preserved body end 0001FB67
	}

	public Property _addAttribute(XElement attrNode, Class cl)
	{
      //  StreamWriter file = new StreamWriter("attribute.txt");

		string type="", attrName="",strVal="", typeNodeType="";
        string id = "";
		bool derived=false;
		Classifier attributeType=null;
		XElement typeNode=null,defaultNode=null;

        XAttribute attrid = (XAttribute)attrNode.Attribute("{http://schema.omg.org/spec/XMI/2.1}id");
        if (attrid != null) id = attrid.Value;

		XAttribute attr = (XAttribute)attrNode.Attribute("{http://schema.omg.org/spec/XMI/2.1}type");
        if (attr == null) attr = (XAttribute)attrNode.Attribute("{http://www.omg.org/spec/XMI/20131001}type");
        if (attr == null) attr = (XAttribute)attrNode.Attribute("type");

		if(attr!=null)type = attr.Value;
		
		attr = (XAttribute)attrNode.Attribute("name");
		if(attr!=null)attrName = attr.Value;		
		
		attr = (XAttribute)attrNode.Attribute("isDerived");
		if(attr!=null && attr.Value.CompareTo("true")==0)
		{
			derived = true;
		}
		else if(attr!=null && attr.Value.CompareTo("true")!=0)
		{
			derived = false;
		}
		
		foreach(XElement child in attrNode.Elements())
		{
			if(child.Name.LocalName.CompareTo("type")==0)
				typeNode = child;
			if(child.Name.LocalName.CompareTo("defaultValue")==0)
				defaultNode = child;
		}

		
		if(defaultNode!=null)
		{
			attr = (XAttribute)defaultNode.Attribute("value");
			if(attr!=null)strVal = attr.Value;
		}
      //  file.WriteLine("Attribut : " + attrName);
      //  file.Flush();

		if(typeNode!=null)
		{
			
			attr = (XAttribute)typeNode.Attribute("{http://schema.omg.org/spec/XMI/2.1}type");
            if (attr == null) attr = (XAttribute)typeNode.Attribute("{http://www.omg.org/spec/XMI/20131001}type");

			if(attr!=null)typeNodeType = attr.Value;
            else
            { 
              //  file.WriteLine("Pas d'attribut type"); file.Flush();
                typeNodeType = ((XAttribute)typeNode.Attribute("{http://schema.omg.org/spec/XMI/2.1}idref")).Value;
                if (typeNodeType == null) attr = (XAttribute)typeNode.Attribute("{http://www.omg.org/spec/XMI/20131001}idref");

                attributeType = model.getBasicType(typeNodeType.ToLower());
              //  file.WriteLine(attributeType.name); file.Flush();
            }
			
			if(typeNodeType.CompareTo("uml:Class")==0)
			{
				attr = (XAttribute)typeNode.Attribute("idref");
				
				if(attr!=null && _classifiers.ContainsKey(attr.Value))
					attributeType = _classifiers[attr.Value];
				else if (attr!=null){
					attributeType = model.getBasicType(attr.Value.ToLower());
				}
				
			}
			else if(typeNodeType.CompareTo("uml:PrimitiveType")==0)
			{
				string strType="";
					
				attr = (XAttribute)typeNode.Attribute("href");
				if(attr!=null)strType = attr.Value.Substring(attr.Value.IndexOf("#")+1);
				
				attributeType = model.getBasicType(strType.ToLower());
			
				
			}
		}
		else
		{
			
			typeNode = attrNode;
			
			attr = (XAttribute)typeNode.Attribute("type");
			if(attr!=null)typeNodeType = attr.Value;
			
			if(_classifiers.ContainsKey(typeNodeType))
			{
				attributeType = _classifiers[typeNodeType];
			}
			else 
				attributeType = model.getBasicType(typeNodeType.ToLower());
		}
					
		ValueSpecification valueSpec=null;
		if (derived) {
			
			Expression curExpression = new Expression(strVal,attributeType);
			
			valueSpec = (ValueSpecification) curExpression;
		} else if (strVal.CompareTo("")==0) {
			//value = attributeType.createValueFromString(strVal);
		}
	
		Property attrProp = new Property(attrName, cl, attributeType,null, valueSpec,null);
        if (hasStereotype(id))
        {
            attrProp.Stereotype = getStereotype(id);
            MascaretApplication.Instance.VRComponentFactory.Log("DEBUG /// STEREOTYPE : " + attrName + " : " + attrProp.Stereotype);
        }

	
		
		string mulStr = "1";
		
		foreach(XElement child in attrNode.Elements())
		{
			if(child.Name.LocalName.CompareTo("upperValue")==0)
			{
				attr = (XAttribute)child.Attribute("value");
				if(attr!=null && attr.Value.CompareTo("*")==0)
					mulStr = "-1";
				if(attr!=null && attr.Value.CompareTo("")==0)
					mulStr = "1";
				
				break;
			}
			
		}

      //  file.Close();
		
		attrProp.MaximumNumber=int.Parse(mulStr);
		mulStr = "1";
		
		foreach(XElement child in attrNode.Elements())
		{
			if(child.Name.LocalName.CompareTo("lowerValue")==0)
			{
				attr = (XAttribute)child.Attribute("value");
				if(attr!=null && attr.Value.CompareTo("*")==0)
					mulStr = "-1";
				if(attr!=null && attr.Value.CompareTo("")==0)
					mulStr = "0";
				
				break;
			}
			
			
		}
		
		attrProp.MinimumNumber=int.Parse(mulStr);
				
        //derived
		attrProp.IsDerived=derived;
	
		cl.addAttribute(attrProp);
		attrProp.Description=getComment(attrNode);
		attrProp.Summary=getSummary(attrNode);
		attrProp.Tags=getTags(attrNode);
	
		return attrProp;
	
		// Bouml preserved body end 0001FBE7
	}

	//TODO: rajouter multiplicité
	public void addAssociation()
	{
		// Bouml preserved body begin 0001FC67			
		
		//List<string> keys=_idAsso.Keys;
		
		//foreach (KeyValuePair<string,List<KeyValuePair<Class, XElement> > > elmt in _idAsso.convertToDictionary()) 
		foreach(string eKey in _idAsso.Keys)
		{
			
			List<KeyValuePair<Class, XElement> > elmt = _idAsso[eKey];

			
			if (elmt.Count == 1) {
				Property p=null;
				//Debug.Log(elmt.Value[0].Value);
				p = _addAttribute(elmt[0].Value, elmt[0].Key); //xmlNode of attribute , Class
				p.IsNavigable=false;
			} else {
				Property p1=null, p2=null;
				//Debug.Log(elmt.Value[0].Value + " / " + elmt.Value[1].Value);
				p1 = _addAttribute(elmt[0].Value, elmt[0].Key); //xmlNode of attribute , Class
				p2 = _addAttribute(elmt[1].Value, elmt[1].Key); //xmlNode of attribute , class
				p1.IsNavigable=true;
				p2.IsNavigable=true;
				p1.Opposite=p2;
				p2.Opposite=p1;
			}
		}
		
		// Bouml preserved body end 0001FC67
	}
	
	public void addCallOperations()
	{
		foreach( KeyValuePair<CallOperationAction, string> elt in _callOperations)
		{
			string opid = elt.Value;
			CallOperationAction act = elt.Key;
			if (_idOperations.ContainsKey(opid))
				act.Operation = _idOperations[opid];
			//else
				//Debug.Log("Erreur pas d'action : " + opid);
		}
	}

	public void addCallBehaviors()
	{
		foreach (KeyValuePair<CallBehaviorAction, string> it in _callBehaviors) 
		{
			string opid = it.Value;
			CallBehaviorAction cb = it.Key;
			if (_idBehaviors.ContainsKey(opid))
				cb.Behavior= _idBehaviors[opid];
			else
			{
				//Debug.Log("[ModelLoader2 Warning] Behavior: " + opid + " not found for ActionNode " + cb.name);
			}
		}
	}

	public void addPins(ActionNode an,XElement node)
	{
		
		foreach (XElement pins in node.Elements())
	    {
	        if(pins.Name.LocalName=="argument")
	        {
	            if(pins.Attribute("type").Value=="uml:ValuePin")
				{
	                ValuePin valuePin = new ValuePin();
	                string id = pins.Attribute("id").Value;
					string name = pins.Attribute("name").Value;
					valuePin.Id = id;
					valuePin.name = name;
	                Classifier ressourceType = getObjectNodeType(pins);
	                if(ressourceType != null)
	                	valuePin.ResourceType = ressourceType;
	                an.Action.ValuePins.Add(valuePin);
	                _objectNodes.Add(valuePin.Id, valuePin);
	            }
	            else
	            {
	                InputPin inputPin  = new InputPin();
	                inputPin.Id = pins.Attribute("{http://schema.omg.org/spec/XMI/2.1}id").Value;
                    if (inputPin.Id == null) inputPin.Id = pins.Attribute("{http://www.omg.org/spec/XMI/20131001}id").Value;

	                inputPin.name = pins.Attribute("name").Value;
	                Classifier ressourceType = getObjectNodeType(pins);
	                if(ressourceType != null)
	                	inputPin.ResourceType = ressourceType;
	
					//Debug.Log(" AN : " + an);
					
	                an.Action.InputPins.Add(inputPin);
	                _objectNodes.Add(inputPin.Id,inputPin);
				}
			}
	        else if(pins.Name.LocalName=="result")
	        {
	            OutputPin outputPin = new OutputPin();
	            outputPin.Id = pins.Attribute("id").Value;
	            outputPin.name = pins.Attribute("name").Value;
	            Classifier ressourceType = getObjectNodeType(pins);
	            if(ressourceType != null) 
					outputPin.ResourceType = ressourceType;
	            an.Action.OutputPins.Add(outputPin);
	            _objectNodes.Add(outputPin.Id, outputPin);
	        }
		}
	}

	


	public void _addParameters(Operation op, XElement opNode)
	{
		// Bouml preserved body begin 0001FCE7
		foreach(XElement child in opNode.Elements())
		{
			if (child.Name.LocalName.CompareTo("ownedParameter")==0) {
				string paramName="", paramKind="", id="";

				XAttribute attr = (XAttribute)child.Attribute("name");
				if(attr!=null) paramName = attr.Value;
			
				attr = (XAttribute)child.Attribute("direction");
				if(attr!=null) paramKind = attr.Value;

                attr = (XAttribute)child.Attribute("{http://schema.omg.org/spec/XMI/2.1}id");
                if (attr == null) attr = (XAttribute)child.Attribute("{http://www.omg.org/spec/XMI/20131001}id");

				if(attr!=null) id = attr.Value;
			
				Parameter parameter = new Parameter(paramName, paramKind,null);

                attr = (XAttribute)child.Attribute("type");
                if (attr != null )
                { 
					_parameters.Add(id,parameter);
                    _paramToType.Add(id, attr.Value); //set type after
                    op.addParameter(parameter);
                }
                else 
                {
				foreach(XElement gChild in child.Elements())
				{

					if(gChild.Name.LocalName.CompareTo("type")==0)
					{

                        attr = (XAttribute)gChild.Attribute("{http://schema.omg.org/spec/XMI/2.1}type");
                        if (attr == null) attr = (XAttribute)child.Attribute("{http://www.omg.org/spec/XMI/20131001}type");

						if(attr!=null && attr.Value.CompareTo("uml:Class")==0)
						{
							_parameters.Add(id,parameter);
							XAttribute attr2 = (XAttribute)gChild.Attribute("idref");
							if(attr2!=null)_paramToType.Add(id, attr2.Value); //set type after
						}
						else if (attr!=null && attr.Value.CompareTo("uml:PrimitiveType")==0)
						{
							Classifier type=null;
							string strType="";
							
							XAttribute attr2 = (XAttribute)gChild.Attribute("href");
							if(attr2!=null)strType = attr2.Value.Substring(attr2.Value.IndexOf("#")+1);
							
							type = model.getBasicType(strType.ToLower());
							parameter.Type=type;
							
						} else if (attr==null) {
							_parameters.Add(id,parameter);
							
							XAttribute attr2 = (XAttribute)gChild.Attribute("{http://schema.omg.org/spec/XMI/2.1}type");
                            if (attr2 == null) attr2 = (XAttribute)child.Attribute("{http://www.omg.org/spec/XMI/20131001}type");

							if(attr2!=null)_paramToType.Add(id,attr2.Value);
						}

						op.addParameter(parameter);
								
						break;
					}
				}
                }
				
			}
	
		}
		// Bouml preserved body end 0001FCE7
	}

	public void _setParametersType()
	{
        foreach(KeyValuePair<string, string> el in _paramToType)
        {
            if (_parameters.ContainsKey(el.Key))
            {
                Parameter parameter = _parameters[el.Key];
                if (_classifiers.ContainsKey(el.Value))
                {
                    Classifier type = _classifiers[el.Value];
                    parameter.Type = type;
                }
                else { }
            }
            else { }
        }
	}

	public void addActivity(XNode activityNode, Package pkg)
	{
		
	}
	
	public void addActivityToClass(XElement activityNode, Class cl)
	{	
		string name = activityNode.Attribute("name").Value;
		
		//Debug.Log(" ###### Adding Activity to class : " + cl.getFullName() + " : " + name);

		bool hasSpecification = false;
		string idoperation = "";
		if (activityNode.Attribute("specification") != null) 
		{
			idoperation = activityNode.Attribute("specification").Value;
			hasSpecification = true;
		}

		Activity activity = new Activity(name);
		
		foreach(XElement child in activityNode.Elements())
		{
			if (child.Name.LocalName.CompareTo("group")==0)
				addActivityGroup(child, activity);
		}
		foreach(XElement child in activityNode.Elements())
		{	
			if (child.Name.LocalName.CompareTo("node")==0)
				addActivityNode(child, activity);
			else if (child.Name.LocalName.CompareTo("ownedParameter")==0)
				addActivityParameter(child, activity);
		}
		foreach(XElement child in activityNode.Elements())
		{	
			if (child.Name.LocalName.CompareTo("edge")==0)
				addActivityEdge(child, activity);
		}
		
		if (hasSpecification) 
		{
			if (_idOperations.ContainsKey(idoperation))
				_idOperations[idoperation].Method = activity;
			//else
				//Debug.Log("[ModelLoader2 Warning] Operation : " + idoperation
				//+ " not found for activity " + name );
		} else 
		{
			if (cl.hasOperation(name)) 
			{
				Operation op = cl.Operations[name];
				op.Method = activity;
			}
		}
		//	pkg->addActivity(activity);
		activity.Description = getComment(activityNode);
		activity.Summary = getSummary(activityNode);
		activity.Tags = getTags(activityNode);
	}
	
	public Activity addActivity(XElement activityNode)
	{
		/* Add Activity pour Structure Organisation */
		string name = activityNode.Attribute("name").Value;
		
		//Debug.Log("Parsing Activity : " + name);
	
		Activity activity = new Activity(name);
		foreach(XElement child in activityNode.Elements())
		{
			if (child.Name.LocalName == "group")
				addActivityGroup(child, activity);
		}

		foreach(XElement child in activityNode.Elements())
		{
			if (child.Name.LocalName == "node") //actionNode
				addActivityNode(child, activity);
			else if (child.Name.LocalName == "ownedParameter")
				addActivityParameter(child, activity);
		}

		foreach(XElement child in activityNode.Elements())
		{
			if (child.Name.LocalName == "edge")
				addActivityEdge(child, activity);
		}
		
		activity.Description = getComment(activityNode);
		activity.Summary = getSummary(activityNode);
		activity.Tags = getTags(activityNode);
		
		return activity;
	}

	//TODO ajout après le reste
	public void addStateMachine(XNode smNode, Package pkg)
	{
		
	}
	
	public void addStateMachineToClass(XElement smNode, Class classe)
	{
      //StreamWriter file = MascaretApplication.Instance.logfile;

		//Debug.Log ("********* [ModelLoader2 Info] addStateMachineToClass (SM: ");

		string name = smNode.Attribute("name").Value;

		//Debug.Log("RQ : " + name + " Class: " + classe.getFullName());
      // file.WriteLine("StateMachine : " + name); file.Flush();
		StateMachine machine = new StateMachine(name);

		machine.Description = getComment(smNode);
		machine.Summary = getSummary(smNode);
		machine.Tags = getTags(smNode);

		
		// SubMachine ?
		//if (smNode->hasProperty("submachineState")) {
		//	string subMachineRef = smNode->getProperty("id");
		//	_subStateMachines[subMachineRef] = machine;
			//cerr << "SUB STATE MACHINE" << endl;
		//} else {
			classe.addOwnedBehavior(machine);
			machine.Owner = classe;
		//}

          
		
		Dictionary<string, Vertex > vertices = new Dictionary<string, Vertex>();

        foreach (XElement child in smNode.Elements())
        {
            if (child.Name.LocalName.CompareTo("packagedElement") == 0 || child.Name.LocalName.CompareTo("ownedMember") == 0)
            {
                string childType = "";
              //  file.WriteLine("Child : " + child.Name.LocalName);
              //  file.Flush();

                XAttribute attr = (XAttribute)child.Attribute("{http://schema.omg.org/spec/XMI/2.1}type");
                if (attr == null) attr = (XAttribute)child.Attribute("{http://www.omg.org/spec/XMI/20131001}type");

                if (attr != null) childType = attr.Value;

                if (childType == "uml:Trigger")
                {
                    string id = "";
               //   file.WriteLine("Trigger");
                    XAttribute attr3 = (XAttribute)child.Attribute("{http://schema.omg.org/spec/XMI/2.1}id");
                    if (attr3 == null) attr3 = (XAttribute)child.Attribute("{http://www.omg.org/spec/XMI/20131001}type");

                    if (attr3 != null) id = attr3.Value;

                    foreach (XElement child2 in child.Elements())
                    { 
                        if (child2.Name.LocalName.CompareTo("event") == 0 )
                        {
                          XAttribute attr2 = (XAttribute)child2.Attribute("{http://schema.omg.org/spec/XMI/2.1}type");
                          if (attr2 == null) attr2 = (XAttribute)child.Attribute("{http://www.omg.org/spec/XMI/20131001}type");

                            if (attr2 != null) childType = attr2.Value;
                            if (childType.CompareTo("uml:SignalEvent") == 0)
                            {
                                XElement cSignal = child2.Element("signal");
                                XAttribute idSignal = (XAttribute)cSignal.Attribute("{http://schema.omg.org/spec/XMI/2.1}idref");
                                if (idSignal == null) idSignal = (XAttribute)cSignal.Attribute("{http://www.omg.org/spec/XMI/20131001}type");

                                string idref = idSignal.Value;
                                Signal signal = _signals[idref];
                             //   file.WriteLine("Signal name : " + signal.name); file.Flush();
                                SignalEvent signalEvent = new SignalEvent(signal.name);
                                signalEvent.SignalClass = signal;

                                _events.Add(id, signalEvent);
                            }
                        }
                    }
                }
            }
        }

		foreach (XElement child in smNode.Elements()) 
		{
			if (child.Name.LocalName == "region") 
			{
				//Debug.Log(" ####### add region .....");
				Region region = new Region();
				machine.Region.Add(region);
				addMachineRegion(child, machine, region, vertices);
			} 
			else if (child.Name.LocalName == "transition")
			{
				//Debug.Log(" ####### add transition.....");
				addMachineTransition(child, machine, vertices);
			}
			
		}
    //    file.Close();
		
	}

	public void addGeneralizations()
	{
		//Debug.Log(" ##### GENERALISATION : " + _toGeneralize.Count);
		foreach (KeyValuePair<string,string> elemt in _toGeneralize)
		{
			_idClass[elemt.Value].addChild(_idClass[elemt.Key]);
		}
	}

	public void addDatatype(XNode dtNode)
	{
		
	}

	public void addConditions()
	{
		/*
		foreach (ConstraintT constraint in _constraints) {
			
			if (_stereoPreconditions.Contains(constraint.id)) {
				Operation operation;
				
				if(_idOperations.ContainsKey(constraint.constrainedElementID))
				{
					operation = _idOperations.getMap(constraint.constrainedElementID);
					Expression expression = (Expression)ScriptableObject.CreateInstance("Expression");
					expression.instantiate(constraint.specification, model.getBasicType("boolean"));
				
					Constraint pre=(Constraint)ScriptableObject.CreateInstance("Constraint");
					pre.instantiate();
					pre.Specification=expression;
					operation.addPrecondition(pre);
					Debug.Log (" PRECONDITION : " + constraint.specification+ " FOR : " + operation.getFullName());
				}

			}
			
			if (_stereoPostconditions.Contains(constraint.id)) {
				Operation operation;
				if(_idOperations.ContainsKey(constraint.constrainedElementID))
				{
					operation = _idOperations.getMap(constraint.constrainedElementID);
					Expression expression = (Expression)ScriptableObject.CreateInstance("Expression");
					expression.instantiate(constraint.specification, model.getBasicType("boolean"));
				
					Constraint pre=(Constraint)ScriptableObject.CreateInstance("Constraint");
					pre.instantiate();
					pre.Specification=expression;
					operation.addPostcondition(pre);
					Debug.Log (" POSTCONDITION : " + constraint.specification+ " FOR : " + operation.getFullName());
				}
			}
		}
		*/
	}	

	public void addStereotypes()
	{
     //   MascaretApplication.Instance.logfile.WriteLine("Adding stereotypes"); MascaretApplication.Instance.logfile.Flush();
		foreach(XElement child in  _xmi.Elements())
		{
			string classBase = null;
			string elementBase = null;
			string interfaceBase = null;
			string constraintBase = null;
			
			XAttribute attr = (XAttribute)child.Attribute("base_Class");
			
			if(attr!=null)
				classBase = attr.Value;
			
			attr = (XAttribute)child.Attribute("base_Element");
			
			if(attr!=null)
				elementBase = attr.Value;
			
			attr = (XAttribute)child.Attribute("base_Interface");
			
			if(attr!=null)
				interfaceBase = attr.Value;
			
			attr =(XAttribute) child.Attribute("base_Constraint");
			if(attr!=null)
				constraintBase = attr.Value;

			if (child.Name.LocalName.Contains("Agent")) {
				_stereoAgents.Add(elementBase);
			} else if (child.Name.LocalName.Contains("VirtualHuman")) {
				if (classBase!=null)
				{
					_stereoVirtualHumans.Add(classBase);
				}
				else
				{
					_stereoVirtualHumans.Add(elementBase);
				}
			} else if (child.Name.LocalName.Contains("Entity")) {
				if (classBase!=null)
					_stereoEntities.Add(classBase);
				else
					_stereoEntities.Add(elementBase);
			} else if (child.Name.LocalName.Contains("Role")) {
				if (interfaceBase!=null)
					_stereoRoles.Add(interfaceBase);
				else
					_stereoRoles.Add(elementBase);
			} else if (child.Name.LocalName.Contains("BuiltIn"))
				_stereoBuiltIn.Add(elementBase);
			else if (child.Name.LocalName.Contains("NonInterupt"))
				_stereoNonInterupt.Add(elementBase);
			else if (child.Name.LocalName.Contains("precondition"))
				_stereoPreconditions.Add(constraintBase);
			else if (child.Name.LocalName.Contains("postcondition"))
				_stereoPostconditions.Add(constraintBase);
			else if (child.Name.LocalName.Contains("Block"))
				_stereoBlocks.Add(classBase);
            else
            {
                if (_otherStereo.ContainsKey(child.Name.LocalName))
                {
                    MascaretApplication.Instance.VRComponentFactory.Log("EXIST Prop");
                    XAttribute attrS =(XAttribute) child.Attribute("base_Property");
                    if (attrS != null)
                        _otherStereo[child.Name.LocalName].Add(attrS.Value);
                }
                else
                {
                    MascaretApplication.Instance.VRComponentFactory.Log("NEW Prop");

                    XAttribute attrS =(XAttribute) child.Attribute("base_Property");
                    if (attrS != null)
                    {
                        MascaretApplication.Instance.VRComponentFactory.Log("---> " + child.Name.LocalName+ attrS.Value);
                        _otherStereo.Add(child.Name.LocalName, new List<string>());
                        _otherStereo[child.Name.LocalName].Add(attrS.Value);
                    }
                }
            }
		}
	}

    public bool hasStereotype(string id)
    {
        bool found = false;
        foreach (KeyValuePair<string, List<String>> stereos in _otherStereo)
        {
            if (stereos.Value.Contains(id)) found = true;
        }
        return found;
    }

    public string getStereotype(string id)
    {
        foreach (KeyValuePair<string, List<String>> stereos in _otherStereo)
        {
            if (stereos.Value.Contains(id)) return stereos.Key;
        }
        return "";
    }

	//FinalNode, Merge et Decision devraent �tre des activityNode !!

	public void addActivityNode( XElement node, Activity activity)
	{
        MascaretApplication.Instance.VRComponentFactory.Log("New activity node");
        string type = "";
        string id = "";
        if (node.Attribute("{http://schema.omg.org/spec/XMI/2.1}type") != null)
            type = node.Attribute("{http://schema.omg.org/spec/XMI/2.1}type").Value;
        else
            type = node.Attribute("{http://www.omg.org/spec/XMI/20131001}type").Value;

        MascaretApplication.Instance.VRComponentFactory.Log(" Type : " + type);

        if (node.Attribute("{http://schema.omg.org/spec/XMI/2.1}id") != null)
		    id = node.Attribute("{http://schema.omg.org/spec/XMI/2.1}id").Value;
        else 
            id = node.Attribute("{http://www.omg.org/spec/XMI/20131001}id").Value;
        MascaretApplication.Instance.VRComponentFactory.Log(" ID : " + id);

        string name = "";
        if (node.Attribute("name") != null)
            name = node.Attribute("name").Value;
        else name = id;
        MascaretApplication.Instance.VRComponentFactory.Log(" Name : " + name);


		string idPartition = "";
		if (node.Attribute("inPartition") != null)
			idPartition = node.Attribute("inPartition").Value;

        MascaretApplication.Instance.VRComponentFactory.Log(id); 
        MascaretApplication.Instance.VRComponentFactory.Log(type); 
        MascaretApplication.Instance.VRComponentFactory.Log(name); 
        MascaretApplication.Instance.VRComponentFactory.Log(idPartition); 

		ActivityNode actNode = null;
		

		//Debug.Log(" TYPE NODE : " + type);
		
		if (type == "uml:InitialNode")
		{
			actNode = new InitialNode();
			activity.Initial = actNode;
			actNode.name = "Initial";
		} else if (type == "uml:FinalNode" || type == "uml:ActivityFinalNode") 
		{
			actNode = new FinalNode();
		} else if (type == "uml:LoopNode") 
		{
			addActivityGroup(node, activity);
		} else if (type == "uml:ForkNode") 
		{
			actNode = new ForkNode();
		} else if (type == "uml:JoinNode") 
		{
			actNode = new JoinNode();
		} else if (type == "uml:MergeNode") 
		{
			actNode = new MergeNode();
		} 
		/*else if (type == "uml:DecisionNode") 
		{
			actNode = new DecisionNode();
		}*/ 
		else if (type == "uml:ObjectNode" || type == "uml:CentralBufferNode") 
		{
			
			ObjectNode objNode = new ObjectNode(name);
			Classifier ressourceType = getObjectNodeType(node);
			if(ressourceType != null)
				objNode.ResourceType = ressourceType;
			actNode = objNode;
            string type2 = node.Attribute("{http://schema.omg.org/spec/XMI/2.1}type").Value;
			if (_idClass.ContainsKey(type2))
				((ObjectNode)actNode).ResourceType = _idClass[type2];
		}
		else if (type == "uml:ValueSpecificationAction") 
		{
			/*
			shared_ptr<XmlNode> valueNode = node->getChildByName("value");
			string expressionValue = valueNode->getProperty("value");
			shared_ptr<Expression> expression = make_shared<Expression>(expressionValue, _model->getBasicType("boolean"));
			_activityExpressions[node->getProperty("id")] = expression;
			*/
		}
		else if (type == "uml:CallBehaviorAction")
		{
			
			ActionNode an = new ActionNode(name,"action");
			CallBehaviorAction cb = new CallBehaviorAction();
            cb.Description = getComment(node);
            cb.name = name;
            cb.Owner = activity;
			XElement beNode = node.Element("behavior");
			
			string opid;

            if (beNode != null) opid = beNode.Attribute("idref").Value;
            else opid = node.Attribute("behavior").Value;

            if (_idBehaviors.ContainsKey(opid))
            {
                cb.Behavior = _idBehaviors[opid];
            }
            else
            {
               // Debug.Log("[ModelLoader2 Info] Behavior " + opid
               //           + " not yet found for ActionNode " + name + ". Postbone...");
                _callBehaviors.Add(cb, opid);
            }

            an.Action = cb;
            addPins(an, node);
            actNode = an;

		}
		else if (type == "uml:CallOperationAction")
		{
			
			ActionNode an = new ActionNode(name,"action");
			//Debug.Log("Action Node : " +name);
			CallOperationAction act = new CallOperationAction();
            act.Description = getComment(node);
            an.Fml = getFML(node);

            act.name = name;

            act.Owner = activity;

			XElement opNode = node.Element("operation");
			if (opNode != null) 
			{
				string opid;
				opid = opNode.Attribute("idref").Value;
                //MascaretApplication.Instance.logfile.WriteLine("Operation ID : " + opid); MascaretApplication.Instance.logfile.Flush();

				if (_idOperations.ContainsKey(opid))
					act.Operation = _idOperations[opid];
				else
				{
					//cerr << "ERREUR PAS OPERATION... " << opid<< endl;
					_callOperations.Add(act,opid);
				}
			} else 
			{
				string opid;
				opid = node.Attribute("operation").Value;
               // MascaretApplication.Instance.logfile.WriteLine("Operation ID : " + opid); MascaretApplication.Instance.logfile.Flush();

                if (_idOperations.ContainsKey(opid))
                {
                    act.Operation = _idOperations[opid];
                   // MascaretApplication.Instance.logfile.WriteLine("Operation Name : " + act.Operation); MascaretApplication.Instance.logfile.Flush();

                }
                else
                    _callOperations.Add(act, opid);
			}
			an.Action = act;
			addPins(an,node);
			actNode = an;
		} 
		else if (type == "uml:SendSignalAction") 
		{
            MascaretApplication.Instance.VRComponentFactory.Log("SendSignalAction");
			ActionNode an = new ActionNode(name,"action");
			SendSignalAction act = new SendSignalAction();
			XElement sigNode = node.Element("signal");
			if (sigNode != null) 
			{
				string signame = sigNode.Attribute("name").Value;
				if (!_signals.ContainsKey(signame))
				{
					Signal ns = new Signal(signame);
					_signals.Add(signame, ns);
				}
				act.SignalClass = _signals[signame];
			} else 
			{
				string signame = "";
				if (node.Attribute("signal") != null)
					signame = node.Attribute("signal").Value;
				if (!_signals.ContainsKey(signame))
				{
					Signal ns = new Signal(signame);
					_signals.Add(signame, ns);
				}
				
				act.SignalClass = _signals[signame];
			}

			XElement targetNode = node.Element("argument");
            if (targetNode != null)
            {
                string targetName = targetNode.Attribute("name").Value;
                XElement typeNode = targetNode.Element("type");
                string classid = "";
                if (typeNode != null)
                    classid = typeNode.Attribute("idref").Value;
                else
                    classid = targetNode.Attribute("type").Value;
                act.Target = new SendSignalTarget();
                act.Target.targetName = targetName;
                act.Target.targetClass = _idClass[classid];
            }
            else
            {
                targetNode = node.Element("target");
                if (targetNode != null)
                {
                    MascaretApplication.Instance.VRComponentFactory.Log("SendSignal Target");
                    string targetName = targetNode.Attribute("name").Value;
                    string classid = targetNode.Attribute("type").Value;
                    MascaretApplication.Instance.VRComponentFactory.Log("name : " + targetName);
                    MascaretApplication.Instance.VRComponentFactory.Log("name : " + targetName + " : " + _idClass[classid]);

                    act.Target = new SendSignalTarget();
                    act.Target.targetName = targetName;
                    act.Target.targetClass = _idClass[classid];

                }
                else MascaretApplication.Instance.VRComponentFactory.Log("No target");
            }
			an.Action = act;
			addPins(an,node);
			actNode = an;
		} 
		else if (type == "uml:AddStructuralFeatureValueAction") 
		{
			//cerr << " ############# Found a AddStructuralFeatureValueAction : " << endl;
			/*
			shared_ptr<ActionNode> an = make_shared<ActionNode>(name);
			shared_ptr<Classifier> type;
			shared_ptr<XmlNode> pinNode = node->getChildByName("object");

			string attrName = pinNode->getProperty("name");

			cerr << " Name == " << attrName << endl;

			shared_ptr<XmlNode> typeNode = pinNode->getChildByName("type");
			if (typeNode && typeNode->getProperty("type") == "uml:PrimitiveType") 
			{
				string href = typeNode->getProperty("href");
				string strType = href.substr(href.rfind('#') + 1);
				boost::to_lower(strType);
				type = _model->getBasicType(strType);
			}

			shared_ptr<XmlNode> valueNode = pinNode->getChildByName("value");
			if (valueNode) 
			{
				string value = valueNode->getProperty("value");
				cerr << "    VALUE : " << value << endl;
				shared_ptr<Expression> expression = make_shared<Expression>(value, type);
				shared_ptr<AddStructuralFeatureValueAction> act =
					make_shared<AddStructuralFeatureValueAction>();
				act->setValue(expression);
				act->setProperty(attrName);
				an->setAction(act);
			}
			actNode = an;
			*/

		} else if (type == "uml:AcceptEventAction") 
		{
            MascaretApplication.Instance.VRComponentFactory.Log("New uml:AcceptEventAction");
			ActionNode an = new ActionNode(name,"action");
			AcceptEventAction act= new AcceptEventAction();

			XElement triggerNode = node.Element("trigger");
			if (triggerNode != null) 
			{
                MascaretApplication.Instance.VRComponentFactory.Log("trigger node found");
                
				Trigger trigger = new Trigger (name);
                MascaretApplication.Instance.VRComponentFactory.Log("Debug : " + name);
                string idT = "";
                if (triggerNode.Attribute("{http://schema.omg.org/spec/XMI/2.1}id") != null)
                    idT = triggerNode.Attribute("{http://schema.omg.org/spec/XMI/2.1}id").Value;
                else
                    idT = triggerNode.Attribute("{http://www.omg.org/spec/XMI/20131001}id").Value;

				trigger.Id = idT;
                MascaretApplication.Instance.VRComponentFactory.Log(trigger.Id);

				if (triggerNode.Attribute("event") != null) 
				{
					string idEvent = triggerNode.Attribute("event").Value;

					if (_events.ContainsKey(idEvent)) 
					{
						trigger.MEvent = _events[idEvent];
						act.setTrigger(trigger);
					} else
						MascaretApplication.Instance.VRComponentFactory.Log( "Could not find event : " + idEvent);
				}
			}
			an.Action = act;
			addPins(an,node);
			actNode = an;

            MascaretApplication.Instance.VRComponentFactory.Log(" Fin AcceptEventAction");
			
		}
		// ...
		else 
		{
			//Debug.Log("Unknown state: " + type);
		}

		if (actNode != null) 
		{
			_activityNodes.Add(id,actNode);
			activity.addNode(actNode);
			//cerr << "searching _partitions for '" << idPartition << "'" << endl;
			if (_partitions.ContainsKey(idPartition))
			{
				actNode.Partitions.Add(_partitions[idPartition]);
				_partitions[idPartition].Node.Add(actNode);
			}
			actNode.Summary = getSummary(node);
            actNode.Description = getComment(node);
			actNode.Tags = getTags(node);
		}
	}
	
	public void addActivityNode( XNode node, LoopNode loopNode, Activity activity)
	{
		
	}

	//TODO
	public void addActivityParameter( XElement paramNode, Activity activity)
	{
		string type, attrName;
		attrName = paramNode.Attribute("name").Value;

		XElement typeNode = paramNode.Element("type");

		Classifier attributeType = null;
		if (typeNode != null && typeNode.Attribute("{http://schema.omg.org/spec/XMI/2.1}type").Value == "uml:Class") 
		{
			string typeId = typeNode.Attribute("idref").Value;
			if (_classifiers.ContainsKey(typeId))
			{
				attributeType = _classifiers[typeId];
			}
			else
				attributeType = model.getBasicType(typeId);
		} 
		else if (typeNode != null && typeNode.Attribute("{http://schema.omg.org/spec/XMI/2.1}type").Value == "uml:PrimitiveType") 
		{
			string href = typeNode.Attribute("href").Value;
			string strType = href.Substring(href.LastIndexOf('#') + 1);
			attributeType = model.getBasicType(strType);
		} 
		else if (typeNode == null) 
		{
			typeNode = paramNode;
			string typeId = typeNode.Attribute("type").Value;
			if (_classifiers.ContainsKey(typeId))
			{
				attributeType = _classifiers[typeId];
			}
			else 
			{
				if (typeId != "") 
				{
					attributeType = model.getBasicType(typeId);
				}
			}
	}

	//cerr << " Adding Parameter : " << attrName << " : " << attributeType->getName() << " dans " << activity->getName() << endl;
	ActivityParameterNode parameterNode = new ActivityParameterNode(attrName);
	Parameter parameter = new Parameter(attrName, "read",attributeType);
	parameterNode.Parameter = parameter;
	activity.addNode(parameterNode);
		
	}

	//TODO: containedEdge avec des ObjectFlow
	//Gerer diff entre ObjectFlow et ControlFlow
	public void addActivityEdge( XElement node, Activity activity)
	{

        string sourceid = node.Attribute("source").Value;
        string targetid = node.Attribute("target").Value;

        //MascaretApplication.Instance.logfile.WriteLine("sourceid : " + sourceid); MascaretApplication.Instance.logfile.Flush();
        //MascaretApplication.Instance.logfile.WriteLine("targetid : " + targetid); MascaretApplication.Instance.logfile.Flush();

		string type = "";
        if (node.Attribute("{http://schema.omg.org/spec/XMI/2.1}type") != null) 
            type = node.Attribute("{http://schema.omg.org/spec/XMI/2.1}type").Value;// "uml:ObjectFlow" "uml:ControlFlow"
        else 
            type = node.Attribute("{http://www.omg.org/spec/XMI/20131001}type").Value;
		
        string id = "";
        if (node.Attribute("{http://schema.omg.org/spec/XMI/2.1}id") != null)
            id = node.Attribute("{http://schema.omg.org/spec/XMI/2.1}id").Value;
        else
            id = node.Attribute("{http://www.omg.org/spec/XMI/20131001}id").Value;
        string name = "";
        if (node.Attribute("name") != null)
            name = node.Attribute("name").Value;
		//Debug.Log(" ????????????????????????????????? " + type + " "+id);

		
		ActivityNode source = null;
		ActivityNode target = null;



		if (!_activityNodes.ContainsKey(sourceid))
		{
			if (!_objectNodes.ContainsKey(sourceid))
			{
                //MascaretApplication.Instance.logfile.WriteLine("erreur source"); MascaretApplication.Instance.logfile.Flush();

				//Debug.Log("edge source not found : " + sourceid);
				return;
			}
			else
				source = _objectNodes[sourceid];
		}
		else 
			source = _activityNodes[sourceid];
		
		
		if (!_activityNodes.ContainsKey(targetid)) {
			if (!_objectNodes.ContainsKey(targetid))
			{
                //MascaretApplication.Instance.logfile.WriteLine("erreur cible"); MascaretApplication.Instance.logfile.Flush();

				//Debug.Log("edge Target not found: " + targetid);
				return;
			}
			else
				target = _objectNodes[targetid];
		}
		else 
			target = _activityNodes[targetid];
	
		
		ActivityEdge edge = null;
		if (type == "uml:ControlFlow") edge = new ControlFlow(name);
		else if (type == "uml:ObjectFlow") edge = new ObjectFlow (name);
		else edge = new ActivityEdge(name);
		
		source.Outgoing.Add(edge);
		target.Incoming.Add(edge);
		edge.Source = source;
		edge.Target = target;
		
		//Debug.Log("Source : " + source.name + " / Target : " + target.name);
		
		
		activity.addEdge(edge);
		
		edge.Description = getComment(node);
		edge.Summary = getSummary(node);
		edge.Tags = getTags(node);
		
	}
	
	public void addActivityEdge( XNode node, LoopNode activity)
	{
		
	}

	//TODO
	public void addActivityGroup( XElement node, Activity activity)
	{
        string type = "";
        if (node.Attribute("{http://schema.omg.org/spec/XMI/2.1}type") != null)
            type = node.Attribute("{http://schema.omg.org/spec/XMI/2.1}type").Value;
        else
            type = node.Attribute("{http://www.omg.org/spec/XMI/20131001}type").Value;

        

		if (type == "uml:ActivityPartition")
		{
			string name = node.Attribute("name").Value;
			ActivityPartition partition = new ActivityPartition(name);
			//Debug.Log("   ---> Partition : " + name);
            string id = "";
            if (node.Attribute("{http://schema.omg.org/spec/XMI/2.1}id") != null)
                id = node.Attribute("{http://schema.omg.org/spec/XMI/2.1}id").Value;
            else
                id = node.Attribute("{http://www.omg.org/spec/XMI/20131001}id").Value;

			_partitions.Add(id,partition);
			activity.addPartition(partition);
		}
		else if (type == "uml:LoopNode")
		{
			LoopNode loopNode = new LoopNode(node.Attribute("name").Value,"Loop");
			loopNode.Description = getComment(node);

			activity.addNode(loopNode);
			
			string idPartition = node.Attribute("inPartition").Value;
			if (_partitions.ContainsKey(idPartition))
			{
				loopNode.Partitions.Add(_partitions[idPartition]);
				_partitions[idPartition].Node.Add(loopNode);
			}
			
			/*
        	string isTestedFirst = node.Attribute("isTestedFirst").Value;
        	if (isTestedFirst == "true")
        	{
        		loopNode.setTestedFirst(true);
        	}
        	else
        	{
        		loopNode->setTestedFirst(false);
       		}*/

			_activityNodes.Add(node.Attribute("{http://schema.omg.org/spec/XMI/2.1}id").Value,loopNode);
			//string testId = node.Attribute("test").Value;
			/*
			map<string, shared_ptr<Expression> >::iterator it =	_activityExpressions.find(testId);
			if (it != _activityExpressions.end()) 
			{
				loopNode->setCondition(it->second);
			} 
			else
				cerr << "Test for LoopNode : " << loopNode->getName()
					<< " not found" << endl;
			*/
			
			foreach(XElement child in  node.Elements())
			{
				if (child.Name.LocalName == "node") 
				{
					string inPart = node.Attribute("inPartition").Value;
					//child.->setProperty(string("inPartition"), inPart);
					//cerr << "././././././././././. transferring property: " << inPart << endl;
					addActivityNode(child, loopNode, activity);
				}
			}
			foreach(XElement child in  node.Elements())
			{
				if (child.Name.LocalName == "edge") 
					addActivityEdge(child,loopNode);
			}
		}
		
	}

	public bool isStereotypedEntity( XElement node) {
		// Bouml preserved body begin 0001FFE7
	
		XAttribute attr = (XAttribute)node.Attribute("{http://schema.omg.org/spec/XMI/2.1}id");
        if (attr == null) attr = (XAttribute)node.Attribute("{http://www.omg.org/spec/XMI/20131001}id");

		if(attr!=null) return (_stereoEntities.Contains(attr.Value));
		else return false;
		
		// Bouml preserved body end 0001FFE7
	}

	public bool isStereotypedAgent( XElement node) {
		// Bouml preserved body begin 00020067
		XAttribute attr= (XAttribute)node.Attribute("{http://schema.omg.org/spec/XMI/2.1}id");
        if (attr == null) attr = (XAttribute)node.Attribute("{http://www.omg.org/spec/XMI/20131001}id");

		if(attr!=null) return (_stereoAgents.Contains(attr.Value));
		else return false;
		// Bouml preserved body end 00020067
	}
	public bool isStereotypedVirtualHuman( XElement node) {
		XAttribute attr= (XAttribute)node.Attribute("{http://schema.omg.org/spec/XMI/2.1}id");
        if (attr == null) attr = (XAttribute)node.Attribute("{http://www.omg.org/spec/XMI/20131001}id");

		if(attr!=null) return (_stereoVirtualHumans.Contains(attr.Value));
		else return false;

	}
	public bool isStereotypedRole( XElement node) {
		// Bouml preserved body begin 000200E7
		
		XAttribute attr= (XAttribute)node.Attribute("{http://schema.omg.org/spec/XMI/2.1}id");
        if (attr == null) attr = (XAttribute)node.Attribute("{http://www.omg.org/spec/XMI/20131001}id");

		if(attr!=null) return (_stereoRoles.Contains(attr.Value));
		else return false;
		// Bouml preserved body end 000200E7
	}
	
	public bool isStereotypedBuiltIn( XElement node) {
		// Bouml preserved body begin 000219E7
		
		XAttribute attr= (XAttribute)node.Attribute("{http://schema.omg.org/spec/XMI/2.1}id");
        if (attr == null) attr = (XAttribute)node.Attribute("{http://www.omg.org/spec/XMI/20131001}id");

		if(attr!=null) return (_stereoBuiltIn.Contains(attr.Value));
		else return false;
		
		// Bouml preserved body end 000219E7
	}
	public bool isStereotypedNonInterupt( XElement node) {
		// Bouml preserved body begin 000219E7
		
		XAttribute attr= (XAttribute)node.Attribute("{http://schema.omg.org/spec/XMI/2.1}id");
        if (attr == null) attr = (XAttribute)node.Attribute("{http://www.omg.org/spec/XMI/20131001}id");

		if(attr!=null) return (_stereoNonInterupt.Contains(attr.Value));
		else return false;
		
		// Bouml preserved body end 000219E7
	}
	public bool isStereotypedBlock( XElement node) {
		
		XAttribute attr= (XAttribute)node.Attribute("{http://schema.omg.org/spec/XMI/2.1}id");
        if (attr == null) attr = (XAttribute)node.Attribute("{http://www.omg.org/spec/XMI/20131001}id");

		if(attr!=null) return (_stereoBlocks.Contains(attr.Value));
		else return false;
		
	}
	public string getComment( XElement node)
	{
		string comment ="";
        

        foreach (XElement commentNode in node.Elements("ownedComment"))
        {
            XElement body = commentNode.Element("body");
            if (body != null)
            {
                if (!commentNode.Value.StartsWith("<FML>"))
                    comment = commentNode.Value;
            }
        }
        
		return comment;	
	}

    public string getFML(XElement node)
    {
        string FML = "";

        foreach (XElement commentNode in node.Elements("ownedComment"))
        {
            XElement body = commentNode.Element("body");
            if (body != null)
            {
                if (commentNode.Value.StartsWith("<FML>"))
                    FML = commentNode.Value;
            }
        }

        return FML;
    }
	
	public string getSummary( XElement node)
	{
		return null;
	}
	
	public Dictionary<String,String> getTags( XElement node)
	{
		return null;
	}


	public void addMachineRegion( XElement node, StateMachine machine, Region region, Dictionary<string, Vertex> vertices)
	{

     //   StreamWriter file = new StreamWriter("region.txt");


		foreach (XElement child in node.Elements()) 
		{
			
			if (child.Name.LocalName == "subvertex") 
			{
				string type = "";
                if (child.Attribute("{http://schema.omg.org/spec/XMI/2.1}type") != null)
                    type = child.Attribute("{http://schema.omg.org/spec/XMI/2.1}type").Value;
                else if (child.Attribute("{http://www.omg.org/spec/XMI/20131001}type") != null)
                    type = child.Attribute("{http://www.omg.org/spec/XMI/20131001}type").Value;

				
				string name = child.Attribute("name").Value;

				string childId = "";
				if(child.Attribute("{http://schema.omg.org/spec/XMI/2.1}id") != null)
					childId = child.Attribute("{http://schema.omg.org/spec/XMI/2.1}id").Value;
                else if (child.Attribute("{http://www.omg.org/spec/XMI/20131001}id") != null)
                    childId = child.Attribute("{http://www.omg.org/spec/XMI/20131001}id").Value;

               // MascaretApplication.Instance.logfile.WriteLine("Type : " + type); MascaretApplication.Instance.logfile.Flush();
				if (type == "uml:State") 
				{
                 //   file.WriteLine("State : " + name); file.Flush();
					//cerr << "State : " << name << endl;
                   // MascaretApplication.Instance.logfile.WriteLine("Name : " + name); MascaretApplication.Instance.logfile.Flush();

					State state = new State(name,"State");
					state.Description = getComment(child);
					state.Summary = getSummary(child);
					state.Tags = getTags(child);
					region.Vertices.Add(state);
					
					vertices[childId] = state;
                    
					foreach (XElement vertexChild in child.Elements()) 
					{
						
						if (vertexChild.Name.LocalName == "region") 
						{
							Region subregion = new Region();
							state.Regions.Add(subregion);
							addMachineRegion(vertexChild, machine, subregion,vertices);
							//cerr << "HAS SUBREGION....." << endl;
						} 
						else if (vertexChild.Name.LocalName == "Entry") 
						{
							string opEntryName = "";
							foreach(XElement childV in vertexChild.Elements()) 
								if (childV.Name.LocalName == "body") opEntryName = childV.Value; //InnerTExt
							Class cl = (Class)machine.Owner;
							if (cl.hasOperation(opEntryName))
							{
								Operation op = cl.Operations[opEntryName];	
								state.EntryBehavior= op;
							} else 
							{
								//Debug.Log ("can't found " + opEntryName + " for entry behavior of state " + name );
							}
						}
						else if (vertexChild.Name.LocalName == "Exit") 
						{
							string opExitName = "";
							foreach(XElement childV in vertexChild.Elements()) 
								if (childV.Name == "body") opExitName = childV.Value;
							Class cl = (Class)machine.Owner;
							if (cl.hasOperation(opExitName))
							{
								Operation op = cl.Operations[opExitName];	
								state.ExitBehavior= op;
							} else 
							{
								//Debug.Log ("can't found " + opExitName + " for entry behavior of state " + name );
							}
						} else if (vertexChild.Name.LocalName == "Do" || vertexChild.Name.LocalName == "doBehavior" || vertexChild.Name.LocalName == "doActivity") 
						{
                         //   file.WriteLine(" ... has Do Activity");
                            
							string opDoName;
                            XElement bodyVertexChild = null;
							foreach(XElement childV in vertexChild.Elements()) 
							{

								//Debug.Log(childV.Name.LocalName);
								if (childV.Name.LocalName == "body") bodyVertexChild = childV;
                                else if (childV.Name.LocalName == "method") { bodyVertexChild = childV; }
							}
                            
							Class cl = (Class)machine.Owner;
							if (bodyVertexChild != null) 
							{
                                /*
						   		opDoName = bodyVertexChild.Value;
						   		if(cl.hasOperation(opDoName)) 
								{
									Operation op = cl.Operations[opDoName];	
									state.DoBehavior = op;
									//Debug.Log ("Do behavior : " + op.getFullName());
						   		} else 
								{
									//Debug.Log ("can't found " + opDoName + " for do behavior of state " + name);
						   		}
                                */
                                opDoName = bodyVertexChild.Attribute("{http://schema.omg.org/spec/XMI/2.1}idref").Value;
                                if (_idOperations.ContainsKey(opDoName))
                                {
                                    Operation op = _idOperations[opDoName];
                                    state.DoBehavior = op;
                                 //   file.WriteLine("Do : " + op.getFullName()); file.Flush();
                                }

                           
							}
							else 
							{
						   		string opID = vertexChild.Attribute("specification").Value;
								
								if(_idOperations.ContainsKey(opID))
						   		{
									Operation op = _idOperations[opID]; 
									state.DoBehavior =op;
                                   // MascaretApplication.Instance.logfile.WriteLine(op.getFullName()); MascaretApplication.Instance.logfile.Flush();
									//Debug.Log ("Do behavior : " + op.getFullName());
						   		}
								//else 
									//Debug.Log ("can't found " + opID + " for do behavior of state " + name);
							}
                            
                     
						}
					}
					
				} 
				else if (type == "uml:Pseudostate") 
				{
					
					PseudoState ps = new PseudoState(name);
					ps.Description = getComment(child);
					ps.Summary = getSummary(child);
					ps.Tags = getTags(child);
						
					region.Vertices.Add(ps);
					machine.ConnectionPoint.Add(ps);
					
					vertices[childId] = ps;
					string kind; 
					if (child.Attribute("kind")==null)
						kind = child.Attribute("name").Value;
               	    else 
						kind = child.Attribute("kind").Value;

                   // MascaretApplication.Instance.logfile.WriteLine("PseudoState : " + name + " Kind : " + kind); MascaretApplication.Instance.logfile.Flush();


					if (kind == "initial" || kind == "Initial" || kind == "Initial State")
						ps.kind=PseudoStateKind.INITIAL;
					else if (kind == "join")
						ps.kind=PseudoStateKind.JOIN;
					else if (kind == "fork")
						ps.kind=PseudoStateKind.FORK;
					else if (kind == "deepHistory")
						ps.kind=PseudoStateKind.DEEPHISTORY;
					else if (kind == "shallowHistory")
						ps.kind=PseudoStateKind.SHALLOWHISTORY;
					else if (kind == "choice")
						ps.kind=PseudoStateKind.CHOICE;
					else if (kind == "junction")
						ps.kind=PseudoStateKind.JUNCTION;
					else if (kind == "terminate")
						ps.kind=PseudoStateKind.TERMINATE;
					else if (kind == "exitPoint")
						ps.kind=PseudoStateKind.EXITPOINT;
					else if (kind == "entryPoint")
						ps.kind=PseudoStateKind.ENTRYPOINT;
					
				} 
				else if (type == "uml:Finalstate" || type == "uml:FinalState") 
				{
					FinalState fs = new FinalState(name);
					fs.Description = getComment(child);
					fs.Summary = getSummary(child);
					fs.Tags = getTags(child);
					region.Vertices.Add(fs);
					//machine->addConnectionPoint(fs);
					
					vertices[childId] = fs;
				}
			}
		}

        foreach (XElement child in node.Elements())
        {
            if (child.Name == "transition")
                addMachineTransition(child, machine, vertices);
        }

      //  file.Close();
	}
	
	//non gestion des triggers
	//Attention ! Les Opaque expressions ne sont pas impl�ment� mais sont utilis� pour le moment dans les modelLoader
	//(par contre les expressions existe - c'est du ocl)
	public void addMachineTransition( XElement node, StateMachine machine, Dictionary<string, Vertex>  vertices)
	{
			//shared_ptr<Operation> op;

    //    StreamWriter fileT = MascaretApplication.Instance.logfile;
		Transition t = new Transition();
		string transitionKind = "";
			
		if (node.Attribute("kind") != null)
			transitionKind = node.Attribute("kind").Value;
		t.Description = getComment(node);
		t.Summary = getSummary(node);
		t.Tags = getTags(node);
		if (transitionKind != "internal") {
			//cerr << "Not internal" << endl;
			Region region = machine.Region[machine.Region.Count -1];
			region.Transitions.Add(t);
		}
     //   fileT.WriteLine("-------------------------------------------");
     //   fileT.WriteLine("Transition : " + transitionKind); fileT.Flush();
		
		// Trigger
        
		XElement triggerNode;
		triggerNode = node.Element("trigger");
		if (triggerNode != null) 
		{
            if (triggerNode.Attribute("{http://schema.omg.org/spec/XMI/2.1}idref") != null)
            {
                string idRef = triggerNode.Attribute("{http://schema.omg.org/spec/XMI/2.1}idref").Value;
                MascaretEvent evt = this._events[idRef];
                if (evt != null)
                {
                //    fileT.WriteLine("Trigger : " + evt.name);
                    Trigger trigger = new Trigger(evt.name);
                    trigger.MEvent = evt;
                    t.Trigger.Add(trigger);
                }
            }
            else
            {
			    string triggerName = triggerNode.Attribute("name").Value;

			    if (triggerNode.Attribute("event") != null) 
			    {
				    //cerr << "has prop event" << endl;
				    string eventID = triggerNode.Attribute("event").Value;
				    MascaretEvent evt = this._events[eventID];
				    if (evt != null) 
				    {
                    //    fileT.WriteLine("Trigger : " + evt.name);
                     //   if (evt.GetType().ToString() == "Mascaret.SignalEvent") fileT.WriteLine("Trigger Class : " + ((SignalEvent)evt).SignalClass.name);
                     //   fileT.Flush();
					    Trigger trigger = new Trigger(evt.name);
					    trigger.MEvent = evt;
					    t.Trigger.Add(trigger);
				    }// else
				    	//Debug.Log ("Error : Event not found for trigger : " + triggerName
					    //	+ " in machine " + machine.getFullName() );
			    } 
            }
		}
		
		
		
		
		// Effect
		XElement effectNode;
		effectNode = node.Element("effect");
		if (effectNode != null) 
		{
            string type = "";
            if (effectNode.Attribute("{http://schema.omg.org/spec/XMI/2.1}type") != null)
                type = effectNode.Attribute("{http://schema.omg.org/spec/XMI/2.1}type").Value;
            else
                type = effectNode.Attribute("{http://www.omg.org/spec/XMI/20131001}type").Value;

			if (type == "uml:OpaqueBehavior") 
			{
				if (effectNode.Attribute("specification") != null) 
				{
					string opName = effectNode.Attribute("specification").Value;
					Operation op = _idOperations[opName];

					if (transitionKind != "internal" && op != null) 
					{
						CallOperationAction action = new CallOperationAction();
						t.Effect = action ;
						action.Operation = op;
					}
				
				} else 
				{
					string sigName = effectNode.Attribute("name").Value;
					Signal sig;
					sig = _signals[sigName];
					SendSignalAction action = new SendSignalAction();
					t.Effect = action;
					action.SignalClass = sig;
				}
			}
		}

		
		if (transitionKind != "internal") 
		{
			// Source
			string sourceID = node.Attribute("source").Value;
            string targetID = node.Attribute("target").Value;

//            fileT.WriteLine("Source : " + sourceID);
  //          fileT.WriteLine("Target : " + targetID); 
            foreach(KeyValuePair<string, Vertex> v in vertices)
            {
            //    fileT.WriteLine("       Vertex : " + v.Key + " = " + v.Value.name);
            }


			Vertex vertex = null;


			
			if (!vertices.TryGetValue(sourceID, out vertex)) {
				 }
			else 
			{
              //  fileT.WriteLine("Source : " + vertex.name);
				t.Source = vertex;
				vertex.Outgoing.Add(t);
			}
			// target

            if (!vertices.TryGetValue(targetID, out vertex))
            {
                
            }
            else
            {
             //   fileT.WriteLine("Target : " + vertex.name);

                t.Target = vertex;
                vertex.Incoming.Add(t);
            }
		}
        
	}
	
	public void addActivitiesAndStateMachines()
	{
		
	}
	
	public Classifier getObjectNodeType(XElement node)
	{
		XElement type = node.Element("type");
		if(type != null)
		{
			string pinType = type.Attribute("type").Value;
			if(pinType == "uml:PrimitiveType")
			{
				/*
				string href = type->getProperty("href");
				string strType = href.substr(href.rfind('#') + 1);
				boost::to_lower(strType);
				return _model->getBasicType(strType);
				*/
			}
		}
		else
		{
			string idClass = "";
			if (node.Attribute("type") != null)
				idClass = node.Attribute("type").Value;
			else if (node.Attribute("{http://schema.omg.org/spec/XMI/2.1}type") != null)
				idClass = node.Attribute("{http://schema.omg.org/spec/XMI/2.1}type").Value;
			
			if (this._classifiers.ContainsKey(idClass))
				return _classifiers[idClass];
			/*
			map<string, shared_ptr<Classifier> >::iterator it = _classifiers.find(idClass);
			if (it != _classifiers.end())
				return _classifiers[idClass];
			else {
				
				if (idClass != "") {
					boost::to_lower(idClass);
					return _model->getBasicType(idClass);
				}
				
			} */
		}

		return null;
	}

}
}
