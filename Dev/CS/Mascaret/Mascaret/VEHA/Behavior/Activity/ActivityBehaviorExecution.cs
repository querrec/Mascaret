using System;
using System.Collections.Generic;

[Serializable]
public class ActivityBehaviorExecution : BehaviorExecution
{
	 
	private List<ActivityNode> actions = new List<ActivityNode>();
	public List<ActivityNode> Actions
	{
		get{return actions;}
		set{actions = value;}
	}
	
	 
	private List<ActionNode> execs = new List<ActionNode>();
	public List<ActionNode> Execs
	{
		get{return execs;}
		set{execs = value;}
	}
	
	 
	private Dictionary<string,InstanceSpecification> affectations;
	public Dictionary<string,InstanceSpecification> Affectations
	{
		get{return affectations;}
		set{affectations = value;}
	}
	
	 
	private int stateActivity;
	public int StateActivity
	{
		get{return stateActivity;}
		set{stateActivity = value;}
	}
	
	 
	private Activity activity;
	public Activity Activity
	{
		get{return activity;}
		set{activity = value;}
	}
	
	//default parameters sync=false
	public ActivityBehaviorExecution(Activity activity, InstanceSpecification host, Dictionary<string,ValueSpecification> p, bool sync) :
				base(activity, host, p, sync)

	{
		stateActivity = 0;
		this.activity = activity;
		affectations = new Dictionary<string, InstanceSpecification>();
		
		/* Affectation des parametres */
//		Debug.Log("BUILD AFFECTATIONS ........ "+ activity.name);
		
		foreach(ActivityPartition currentPartition in activity.Partitions)
		{
			if(currentPartition.name=="this")
				affectations.Add("this", host);
			else
			{
				if(p.ContainsKey(currentPartition.name))
				{	
					InstanceValue val = (InstanceValue)p[currentPartition.name];
					affectations.Add(currentPartition.name, val.SpecValue);
				}
				else
				{
					if(host.Slots.ContainsKey(currentPartition.name))
						affectations.Add(currentPartition.name, host.getProperty(currentPartition.name).getValue().valueSpecificationToInstanceSpecification());
					else
						System.Console.WriteLine("[ActivityBehaviorExecution.cpp] Affectation Partition de "+currentPartition.name+" impossible ...");
				}
			}
			
			foreach(ActivityNode currentNode in currentPartition.Node)
			{
				if(currentNode.Kind=="object")
				{
//					Debug.Log(" Parameters size : "+p.KeysList.Count);
					if(p.ContainsKey(currentNode.name))
					{
						InstanceValue val = (InstanceValue)p[currentNode.name];
						affectations.Add(currentNode.name, val.SpecValue);
					}
					else
					{
						if(host.Slots.ContainsKey(currentNode.name))
						{
							try
							{
								affectations.Add(currentNode.name, ((InstanceValue)(host.getProperty(currentNode.name).getValue())).SpecValue);
							}
							catch(Exception)
							{
								System.Console.WriteLine("Multiple relation found, not supported, get the last one");
								foreach(ValueSpecification currentValue in host.getProperty(currentNode.name).Values.Values)
								{
									affectations.Add(currentNode.name, currentValue.valueSpecificationToInstanceSpecification());
									
								}
							}
						}
						else
							System.Console.WriteLine("[ActivityBehaviorExecution.cpp] Affectation Object de "+currentNode.name+ " impossible ...");
					}
					
				}
				else
				{
					if(currentNode.Kind=="action")
					{
						Action action = ((ActionNode)currentNode).Action;
						if(action.Kind=="SendSignal")
						{
							SendSignalAction sendAct = (SendSignalAction)action;
							
							if(sendAct.Target != null && sendAct.Target.target.name!="")
							{
								if(p.ContainsKey(sendAct.Target.target.name))
								{
									InstanceValue val = (InstanceValue) p[sendAct.Target.target.name];
									affectations.Add(sendAct.Target.target.name, val.SpecValue);
								}
								else
								{
									if(host.Slots.ContainsKey(sendAct.Target.target.name))
									{
										affectations.Add (sendAct.Target.target.name, host.getProperty(sendAct.Target.target.name).getValue().valueSpecificationToInstanceSpecification());
										sendAct.Target.target = host.getProperty(sendAct.Target.target.name).getValue().valueSpecificationToInstanceSpecification();
									}
									//else
										//Debug.LogError( "[ActivityBehaviorExecution.cpp] Affectation SendAction de " + sendAct.Target.target.name + " impossible ...");
								}
							}
						}
					}
				}
			}
			
		}
		
		//OCLParser::Context function called
		/* OclParser::Context context;
	     map<string, shared_ptr<InstanceSpecification> >::iterator it;
	     for(it=_affectations.begin();it!=_affectations.end();it++)
	     {
	             context[it->first]=it->second;
	     }
	     _activity->setContext(context);*/
	}
	
	public Activity getActivity()
	{
		return (Activity)this.Specification;
	}
	
	public void fire(ActivityNode activityNode)
	{
		//nothing to do
	}
	
	public override void stop ()
	{
		base.stop ();
		foreach(ActionNode currentNode in execs)
		{
			currentNode.stop();
		}
	}

	public override double execute (double dt)
	{

		if(Sync)
		{
			List<ActionNode> actions = new List<ActionNode>();
			
			if(Activity.Initial!=null)
			{
				actions = Activity.Initial.getOutgoingActionNode();
			}
			else return 0;

			while(actions.Count>0)
			{
				foreach(ActionNode currentNode in actions)
				{
					if(currentNode.Kind=="action")
					{
						currentNode.start(affectations[currentNode.Partitions[0].name], affectations, Sync);
					}
				}
				
				List<ActionNode> next = new List<ActionNode>();
				foreach(ActionNode currentNode in actions)
				{
					next = currentNode.getOutgoingActionNode();
					next.Reverse();
				}
				
				actions = next;
			}
		}
		else
		{
			System.Console.WriteLine(" ####### Running ActivityBehaviorExecution ");

			if(stateActivity == 0)
			{
				if(Activity.Initial!=null)
				{						
//					Debug.Log(Activity.Initial.getOutgoingActionNode().Count);

					foreach(ActionNode currentNode in Activity.Initial.getOutgoingActionNode())
					{
						if(currentNode.Kind=="action")
						{
							System.Console.WriteLine(" Starting Init Action : " + currentNode.getFullName());
							currentNode.start(affectations[currentNode.Partitions[0].name], affectations,false);
						}
						execs.Add(currentNode);
						actions.Add(currentNode);
					}
					stateActivity++;
				}
			}
			if(stateActivity == 1)
			{
				if(execs.Count>0)
				{
					int nbAction = execs.Count;
					foreach(ActionNode currentNode in execs)
					{
						if(!currentNode.isRunning())
							nbAction--;
					}
					if(nbAction==0)
						execs.Clear();
				}
				else
				{
					if(stateActivity==1)
					{
						List<ActionNode> next = new List<ActionNode>();
						foreach(ActionNode currentNode in actions)
						{
							foreach(ActionNode currentChildNode in currentNode.getOutgoingActionNode())
							{
								next.Add(currentChildNode);
							}
						}
						actions.Clear();
						if(next.Count>0)
						{
							foreach(ActionNode currentNode in next)
							{
								System.Console.WriteLine(currentNode.getFullName());
								if(currentNode.Kind=="action")
								{
									currentNode.start (affectations[currentNode.Partitions[0].name], affectations, false);
								}
								execs.Add (currentNode);
								actions.Add(currentNode);
							}
						}
						else
							stateActivity++;
					}
				}
			}
			else
			{
				execs.Clear();
				actions.Clear();
				return 0;
			}
		}		
		return 0.01;
	}
	
}
