using System;
using System.Collections.Generic;

[Serializable]
public class StateMachineBehaviorExecution : BehaviorExecution
{
	 
	private Vertex currentState;
	public Vertex CurrentState
	{
		get{return currentState;}
		set{currentState = value;}
	}
	
	 
	public bool currentActionDone;

	 
	private bool toStop;
	public bool ToStop
	{
		get{return toStop;}
		set{toStop = value;}
	}
	
	 
	private bool first;
	public bool First
	{
		get{return first;}
		set{first = value;}
	}
	
	 
	private List<Trigger> eventQueue;
	public List<Trigger> EventQueue
	{
		get{return eventQueue;}
		set{eventQueue = value;}
	}
	
	 
	private BehaviorExecution be;
	public BehaviorExecution Be
	{
		get{return be;}
		set{be = value;}
	}
	
	
	//default parameter sync=false
	public StateMachineBehaviorExecution(StateMachine stateMachine, InstanceSpecification host, Dictionary<string,ValueSpecification> p, bool sync) : base(stateMachine,host,p,sync)

	{
		first = true;
		currentActionDone = true;
		eventQueue = new List<Trigger> ();
	}
	
	public void setCurrentState(string name)
	{
		Region region = getStateMachine().Region[0];
		foreach(Vertex currentVertex in region.Vertices)
		{
			if(currentVertex.name == name)
				currentState = currentVertex;
		}
	}
	
	public bool simulateState(Vertex state)
	{
		currentState = state;
		state.simulate(Host);
		return true;
	}
	
	public StateMachine getStateMachine()
	{
		return (StateMachine)Specification;
	}
	
	public override double execute(double dt)
	{
	//Debug.Log (" --->  " + this.Host.name + " stateMachine Behavior " + this.Specification.getFullName());
	if (first)
	{
		// TODO: Attention a gerer plus tard : possibilite de fixe l'etat en cours a l'initialisation de l'instance
		// Recherche de l'etat Initial
		List<PseudoState> ps = getStateMachine().ConnectionPoint;
		StateMachine machine = getStateMachine();
		List <Region> res = machine.Region;
		
		bool foundInitialState = false;
		
		foreach (PseudoState pseudo in ps)
		{
			if (pseudo.kind == PseudoStateKind.INITIAL)
			{
				Dictionary<string, ValueSpecification> p = new Dictionary<string, ValueSpecification>();
				List<Transition> trans = pseudo.Outgoing;
				foreach (Transition transition in trans)
				{
					//if (_currentState) _currentState->desactivate();
					activateState(transition.Target,p);
				}
				foundInitialState = true;
				break;
			}
		}
		if (!foundInitialState) toStop = true;
		//toStop = foundInitialState ? false : true;
		first = false;
	}
	else
	{
		while (eventQueue.Count != 0)
		{
			
			Vertex currentState= this.currentState;
			if (currentState == null )//|| (currentState.Noninterpt && currentState.running)) 
				{break;}
				
			//Debug.Log (" --->  " + this.Host.name + " stateMachine Behavior " + this.Specification.getFullName());
				
			Trigger trigger = eventQueue[0];
			eventQueue.RemoveAt(0);
			List<Transition> trans = currentState.Outgoing;
			for (int iT = 0; iT < trans.Count; iT ++)
			{
				List<Trigger> transTriggers = trans[iT].Trigger;	
				for (int i = 0; i < transTriggers.Count ; i++)
				{
					Trigger transTrigger = transTriggers[i];
					if (transTrigger.equals(trigger))
					{
						Dictionary<string, ValueSpecification> param =  new Dictionary<string, ValueSpecification>();
						//OclParser::Context context;
                                              
                        MascaretEvent evt = trigger.MEvent;
                        if (evt != null && evt.Type == "SignalEvent")
                        {
							SignalEvent sevt = (SignalEvent) evt;
                        	InstanceSpecification signal = sevt.Signal;
                            if (signal != null)
                            {
								/*  // Gestion des parametres
                            	map<string, shared_ptr<Slot> > props = signal->getSlots();
                                map<string, shared_ptr<Slot> >::iterator it;
                                for (it = props.begin(); it != props.end(); it++)
                                {
                                	vector<AnyValue> values;
                                    values.push_back((AnyValue)it->second->getValue()->getStringFromValue());  
                                    context[it->first] = CollectionAnyValue(values);
                                    param[it->first] = it->second->getValue();
                                    cerr << it->first << " == " << it->second->getValue()->getStringFromValue() << endl;
                                }
                                */
                             }
                        }
						//context["self"]=getHost();
      
						if(trans[iT].Guard == null) //|| trans[iT]->getGuard()->eval(context))
						{
							if (currentState != null)
							{
								if (be != null)
								{
									be.stop();
									BehaviorScheduler.Instance.deleteExecutionBehavior(be);
									be=null;
								} 
								currentState.desactivate();
							}
							
							Action effect = trans[iT].Effect;
							if (effect != null)
							{
								BehaviorScheduler.Instance.executeBehavior(effect,this.Host,param,false);
							}

							activateState(trans[iT].Target, param);
						}
					}
				}
			}
		}

	//	if(eventQueue.Count == 0)
	//		pause();
			//toStop = true;

	}

	return 0.01;
    //if (toStop) return 0;
	//else return 0.001;
	}
	
	public void addSignal(InstanceSpecification signal)
	{
//		Debug.Log ("recu signal : " + signal.name +" par " +Host.name);
		Trigger trigger = new Trigger(signal.name);
		SignalEvent se = new SignalEvent(signal.name);
		se.Signal = signal;
		trigger.MEvent = se;
		eventQueue.Add(trigger);
		//restart();
	}
	
	public void simulateAddSignal(InstanceSpecification signal)
	{
		
	}
	
	public void addTime(double time)
	{
		Trigger trigger = new Trigger("time");
		TimeEvent te = new TimeEvent("time");
		LiteralReal lValue = new LiteralReal(time);
		te.When = lValue;
		trigger.MEvent = te;
		eventQueue.Add(trigger);
		restart();
	}
	
	protected bool activateState(Vertex state, Dictionary<string,ValueSpecification> p)
	{
		currentState = state;
				
		if(state as FinalState == null)
			toStop = true;
		
		be = currentState.activate(Host,p);
		System.Console.WriteLine(" Activate State : "+currentState.name);
		
		if(be != null)
		{
			currentState.running = true;
			//callBack function
			currentActionDone = false;
		}
		
		foreach(Transition currentTransition in currentState.Outgoing)
		{
			foreach(Trigger currentTrigger in currentTransition.Trigger)
			{
				if(currentTrigger.MEvent.Type == "ChangeEvent")
				{
					ChangeEvent ce = currentTrigger.MEvent as ChangeEvent;
					Expression exp = (Expression)ce.ChangeExpression.clone();
					//callBack function
					//evalutateExpression function
				}
			}
		}
		
		return true;
	}
	
	protected void onBehaviorStop(BehaviorExecution be)
	{
		currentState.running = false;
		currentActionDone = true;
		
		foreach(Transition currentTransition in currentState.Outgoing)
		{
			if(currentTransition.Trigger.Count == 0)
			{
				//ocl actions
			}
		}
	}
	
	protected void onExpressionCHange(Expression exp)
	{
		//expression and callBacks
	}
}