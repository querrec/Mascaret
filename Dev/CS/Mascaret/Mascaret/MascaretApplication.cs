
using System;
using System.IO;
using System.Xml.Linq;
using System.Collections;
using System.Collections.Generic;


public class MascaretApplication 
{
	public HttpCommunicationManager httpCommunicationManager;

	private static MascaretApplication instance=null;
	
	public static MascaretApplication Instance
	{
		get{
			if(instance==null)
			{	
				instance = new MascaretApplication();
			}
			return instance;
		}
	}

	
	public void parse(string applicationFileName)
	{
	
		BaseDir = ".";
		XDocument parser;
		parser = XDocument.Load(applicationFileName);
		XElement root = parser.Root;
		parse(root);
	}
	
	public void parse(XElement root)
	{
		XElement appliNode = root.Element("ApplicationParameters");
		XElement actnode = root.Element("Actors");
		XElement orgNode = root.Element("Organisations");
		
  		{
    		// Parametres par defaut de l'applie ....
    		string ressourceDir = "HTTPServerBaseDir";
    		agentPlateform = new AgentPlateform("localhost", 8080, ressourceDir,false);
    		agent = new Agent(agentPlateform, "HTTPManager", null,""); 
    		agent.addBehavior("SimpleCommunicationBehavior",0.2,true);
    		agentPlateform.addAgent(agent);
 		 }

		foreach (XElement child in root.Elements()) 
		{
			
			if (child.Name.LocalName == "Environment")
			{
				string envUrl = child.Attribute("url").Value;
				Environment env = parseEnvironment(envUrl, actnode, orgNode);
			}
		}
	}

	Model parseModel(string url)
	{
  		ModelLoader2 modelLoader =new ModelLoader2(url,true);
  		Model model = modelLoader.Model;
  		model.ModelLoader = modelLoader;

  		return model;
	}

	Environment parseEnvironment(string url, XElement actNode, XElement orgNode)
	{
  		Environment env;

  		XDocument parser = XDocument.Load(url);
  		XElement root = parser.Root;

  		Model model;
  		XElement modelNode = root.Element("Model");
  		if (modelNode != null)
  		{
    		string urlModel = modelNode.Attribute("url").Value;

    		model = parseModel(urlModel);

    		//map<string, shared_ptr<Environment> >::iterator it = model->getEnvironments().find(url);
    		if (model.Environments.ContainsKey(url))
   			{
     			 env = model.Environments[url];
   			}
    		else
   			{
      			env = new Environment(model);
      			env.Url =url;
     			model.addEnvironment(env);

      			InstanceLoader instanceLoader = new InstanceLoader(agentPlateform, env, url,true);

      			System.Console.WriteLine("SIZE of env map " + model.Environments.Count + " of model :" + model.name);

      			if (actNode != null)
       			 instanceLoader.parseInstances(agentPlateform,env,actNode.Attribute("url").Value,true);

     			if (orgNode != null)
       			 instanceLoader.parseInstances(agentPlateform,env,orgNode.Attribute("url").Value,true);
    		}
  		}
  		else
    		env = null;
  		return env;
	}


	
	
	private Model model = null;
	public Model Model
	{
		get{return model;}
		set{model = value;}
	}
	
	protected Dictionary<string, Model> models;
	public Dictionary<string, Model> Models
	{
		get{return models;}
		set{models = value;}
	}
	
	private AgentPlateform agentPlateform=null;
	public AgentPlateform AgentPlateform
	{
		get{return agentPlateform;}
		set{agentPlateform=value;}
	}
	
	private Agent agent=null;
	public Agent Agent
	{
		get{return agent;}
		set{agent = value;}
	}
	
	public Environment getEnvironment()
	{
		if(Model.Environments!=null && Model.Environments.Keys.Count>0)
		{
			IEnumerator<KeyValuePair<string,Environment>> iter = Model.Environments.GetEnumerator();
			iter.MoveNext();
			return (Environment)(iter.Current.Value);
		}
		else 
		{
			Environment env = new Environment(Model);
			env.Url="default";
			Model.addEnvironment(env);
			return env;
		}
	}
	
	protected string baseDir="";
	public string BaseDir
	{
		get{return baseDir;}
		set{baseDir = value;}
	}
	
	public bool hasModel(string modelName)
	{
		if(model!=null && model.name == modelName)
			return true;
		
		if(models!=null)
		{
			foreach(Model currentModel in models.Values)
			{
				if(currentModel.name == modelName)
					return true;
			}
		}
		return false;
	}
	
	public Model getModel(string modelName)
	{
		if(model !=null && model.name == modelName)
			return model;
		
		if(models!=null)
		{
			foreach(Model currentModel in models.Values)
			{
				if(currentModel.name == modelName)
					return currentModel;
			}
		}
		return null;
	}
	
	public void addModel(Model model)
	{
		if(models==null)
		{
			models = new Dictionary<string,Model>();
		}
		
		models.Add(model.name, model);
		if(this.model==null)
			this.model = model;
	}

	private MascaretApplication()
	{
		instance = this;		
	}
	
	public void init()
	{
		this.httpCommunicationManager = new HttpCommunicationManager(8080,"",false);
		this.httpCommunicationManager.httpServer.Start();
	}
	
	public void run()
	{
 	 	while (step())
  		{
  		}
	}

	public bool step()
	{
  		BehaviorScheduler.Instance.Update();
		this.httpCommunicationManager.httpServer.Update();

  		return true;
	}

}
