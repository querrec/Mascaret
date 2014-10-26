using System;
using System.Collections.Generic;
using Antlr.Runtime;
using Antlr.Runtime.Misc;

namespace Mascaret
{ 
public struct FIPAAction
{
	public string actionName;
	public List<string> paramName;
	public List<string> paramValue;
}

public struct FIPASLParserResult
{
	public bool success;
	public bool isAction;
	public FIPAAction action;
};

[Serializable]
public class SimpleCommunicationBehavior:CyclicBehaviorExecution
{	
	public SimpleCommunicationBehavior(Behavior specif, InstanceSpecification host, Dictionary<String,ValueSpecification> p) : base(specif, host,p)
	{
		
	}
	
	public override void action()
	{
		Agent agt = (Agent)(this.Host);
		ACLMessage msg = agt.receive();

		if (msg != null)
		{
			if (msg.Performative == ACLPerformative.REQUEST)
			{
				manageRequest(msg);
			}
			else if (msg.Performative == ACLPerformative.INFORM)
			{
				manageInform(msg);
			}
			else if (msg.Performative == ACLPerformative.QUERY_REF)
			{
				manageQueryRef(msg);
			}
		}
	}
	
	protected void manageRequest(ACLMessage msg)
	{
		Agent agt = (Agent)(this.Host);
	
		string content = msg.Content;
		
		System.Console.WriteLine(content);

		if (content == "Model") sendModel(msg);
		else if (content == "Organisation") sendOrganisation(msg);
		else if (content == "Actions") sendActions(msg);
		else if (content == "Entities") sendEntities(msg);  // Renvoi les attributs et etats de chaque entités appartenant  a la procedure et abonne aux messages de modifications d'attributs
		else
		{
			string[] words = content.Split(' ');
           	if (words[0] == "STOP") 
	   		{
				//string procName = words[1];
				//ProceduralBehavior be=(ProceduralBehavior)(agt.getBehaviorExecutingByName("ProceduralBehavior"));
                /*if (procName =="ALL")
		   			if (be) be.stopstopAll();
				else
	  	   			if (be) be.stop(procName);
	  	   		*/
	   		}
	   		else manageRequestAction(msg);
        }

	}
	
	protected void manageInform(ACLMessage msg)
	{
	}
	
	protected void manageQueryRef(ACLMessage msg)
	{
	}
	
	protected void manageRequestAction(ACLMessage msg)
	{
		Agent agt = (Agent)(this.Host);
	
		string content = msg.Content;

		FIPASLParserResult result = parseFipaSLExpression(content);

		if (result.success)
		{
			if (result.isAction)
			{
				bool done = false;
				AgentBehavior behavior = agt.getBehaviorByBame(result.action.actionName);
		
				if (behavior != null)
				{
					Dictionary<string,ValueSpecification> parameters = new Dictionary<string,ValueSpecification>();
					if(result.action.paramName.Count > 0)
					{
						for(int i=0;i<result.action.paramName.Count;i++)
						{
							LiteralString stringValue = new LiteralString(result.action.paramValue[i]);
							parameters.Add(result.action.paramName[i],(ValueSpecification)(stringValue));
						}
					}
					else
					{
						for(int i=0;i<result.action.paramName.Count;i++)
						{
							LiteralString stringValue = new LiteralString(result.action.paramValue[i]);
							parameters.Add("param"+i+'0',(ValueSpecification)(stringValue));
						}
					}
		
					BehaviorScheduler.Instance.executeBehavior(behavior,agt,parameters,false);
					ACLMessage reponse = new ACLMessage(ACLPerformative.AGREE);
					reponse.Content = msg.Content;
					reponse.Receivers.Add(msg.Sender);
					agt.send(reponse);
					done=true;
				}
		
				Class classifier = agt.Classifier;
		
				Operation operation = null;
				
				if (classifier != null  && classifier.Operations.ContainsKey(result.action.actionName))
				{
					operation = classifier.Operations[result.action.actionName];
				}
		
				if (operation != null)
				{
					Dictionary<string,ValueSpecification> param = new Dictionary<string,ValueSpecification>();
					List<Parameter> parameters = operation.Parameters;
					List<string> paramValue = result.action.paramValue;
					List<string> paramName = result.action.paramName;
			
					if (parameters.Count == paramValue.Count)
					{
						for (int i=0; i < parameters.Count;i++)
						{
							// Pour tous les parametres issus de l'operation
							string parameterName = parameters[i].name;

							// Cherche l'indice de ce parameter dans paramsName
							int indice = 0; bool found=false;
							while (!found && indice<paramName.Count)
							{
								if (paramName[indice] == parameterName)
								{
									found = true;
								}
								else
								{
									indice++;
								}
							}
					
							if (found)
							{
								string strVal=paramValue[indice];
								param.Add(parameters[i].name,parameters[i].Type.createValueFromString(strVal));
							}
						}
                        BehaviorScheduler.Instance.executeBehavior(operation.Method,agt,param,false);

					/*
						List<ActionNode> possibleTranslation = _translateOperationToActions(operation);
					
						shared_ptr<AgentBehaviorExecution> abe = agt->getBehaviorExecutionByName("ProceduralBehavior"); //hasslich... ja...
						shared_ptr<ProceduralBehavior> pbe  = shared_dynamic_cast<ProceduralBehavior> (abe);
					
						if (possibleTranslation.size() && pbe)
						{
							//add actionNode in ProceduralBehavior
						
							pbe->pushActionToDo(possibleTranslation[0]);
						}
						else
						{
							//call method
						
							shared_ptr<BehaviorExecution> be = BehaviorScheduler::getInstance()->executeBehavior(operation->getMethod(),agt,param);
				
							if (be)
							{
								be->addCallbackOnBehaviorStop(bind(&SimpleCommunicationBehavior::_onBehaviorStop,this,_1));
							
								_requestedAction[be->getSpecification()->getName()].push_back(msg->getSender());
							}
						}
						
					*/
					
						ACLMessage reponse = new ACLMessage(ACLPerformative.AGREE);
						reponse.Content = msg.Content;
						reponse.Receivers.Add(msg.Sender);
						agt.send(reponse);
					}
					else
					{
						ACLMessage reponse = new ACLMessage(ACLPerformative.NOT_UNDERSTOOD);
						string contentRep = "";
						contentRep += content;
						reponse.Content = contentRep;
						reponse.Receivers.Add(msg.Sender);
						agt.send(reponse);
					}
				}
				else
				{
					string procName = result.action.actionName;
					bool found = false;
					OrganisationalEntity askedOrg = null;
					Procedure askedProc = null;
					Role askedRole = null;
					List<OrganisationalEntity> orgs = agt.Plateform.Organisations;
								
					for (int iOrg = 0; iOrg < orgs.Count; iOrg++)
					{
						List<RoleAssignement> assigns = orgs[iOrg].RoleAssignement;
				
						for (int iAss = 0; iAss < assigns.Count;iAss++)
						{
							if (assigns[iAss].Agent.toString() == agt.Aid.toString())
							{
								OrganisationalStructure os = orgs[iOrg].Structure;
								List<Procedure> procs = os.Procedures;
						
								for (int iP = 0; iP < procs.Count; iP++)
								{
									System.Console.WriteLine(procName + " / " + procs[iP].name);
									if (procs[iP].name == procName)
									{
										askedProc = procs[iP];
										askedOrg = orgs[iOrg];
										askedRole = assigns[iAss].Role;
										found = true;
									}
								}
							}
						}
					}
					
					if (found)
					{
						//ACLMessage reponse = new ACLMessage();
						//reponse.instantiate(ACLPerformative.AGREE);
						//reponse.Content = msg.Content;
						//reponse.Receivers.Add(msg.Sender);
						//agt.send(reponse);

						// Recherche du comportement procedural
						// UGGLY
						System.Console.WriteLine("1");
						AgentBehaviorExecution pbehavior = agt.getBehaviorExecutingByName("ProceduralBehavior");

						//AgentBehaviorExecution> behavior2 = agt->getBehaviorExecutionByName("ActionListenerBehavior");
						System.Console.WriteLine("2");
						if (pbehavior != null)
						{
							ProceduralBehavior procBehave  = (ProceduralBehavior) (pbehavior);
						
							Dictionary<string,ValueSpecification> procParams = new Dictionary<string,ValueSpecification>();
							for (int i=0;i<result.action.paramName.Count;i++)
							{
								LiteralString stringValue = new LiteralString(result.action.paramValue[i]);
								procParams.Add(result.action.paramName[i],stringValue);
							}
						
							System.Console.WriteLine("3");
							Activity act = askedProc.Activity;
							//List<ActivityNode>  nodes  = act.Nodes;
					
							procBehave.pushProcedureToDo(askedProc, askedOrg, askedRole, procParams);
							//procBehave.restart();

							ACLMessage reponse = new ACLMessage(ACLPerformative.AGREE);
							reponse.Content = msg.Content;
							reponse.Receivers.Add(msg.Sender);
							agt.send(reponse);
						}
						/*
						else if (behavior2)
						{
							shared_ptr<ActionListenerBehavior> procBehave  = shared_dynamic_cast<ActionListenerBehavior> (behavior2);

							procBehave->pushProcedureToDo(askedProc, askedOrg, askedRole, Parameters());
							procBehave->restart();

							shared_ptr<ACLMessage> reponse = make_shared<ACLMessage>(AGREE);
							reponse->setContent(msg->getContent());
							reponse->addReceiver(msg->getSender());
							agt->send(reponse);
						}*/
						else
						{
							if (!done)
							{
								ACLMessage reponse = new ACLMessage(ACLPerformative.NOT_UNDERSTOOD);
								string contentRep = "";
								contentRep += result.action.actionName;
								reponse.Content = contentRep;
								reponse.Receivers.Add(msg.Sender);
								agt.send(reponse);
							}
						}
					}
					else
					{
						if (!done)
						{
							ACLMessage reponse = new ACLMessage(ACLPerformative.NOT_UNDERSTOOD);
							string contentRep = "";
							contentRep += result.action.actionName;
							reponse.Content = contentRep;
							reponse.Receivers.Add(msg.Sender);
							agt.send(reponse);
						}
					}
				}
			}
			else
			{
				// Proposition Non encore traite....
			}
		}
		else
		{
			System.Console.WriteLine("[SimpleCommunicationBehavior Warning] Parsing error. Message content parsing error: " + content);
		
			ACLMessage reponse = new ACLMessage(ACLPerformative.NOT_UNDERSTOOD);
			string contentRep  = "";
			contentRep += content;
			reponse.Content = contentRep;
			reponse.Receivers.Add(msg.Sender);
			agt.send(reponse);
		}
	}
	
	protected void sendModel(ACLMessage msg)
	{
	}
	
	protected void sendOrganisation(ACLMessage msg)
	{
	}
	
	protected void sendActions(ACLMessage msg)
	{
	}
	
	protected void sendEntities(ACLMessage msg)
	{
	}
	
	protected FIPASLParserResult parseFipaSLExpression(string content)
	{
		FIPASLParserResult result = new FIPASLParserResult();
		
		FipaSLLexer lex = new FipaSLLexer(new ANTLRStringStream(content));	
	    CommonTokenStream tokens = new CommonTokenStream(lex);
			
	    FipaSLParser parser = new FipaSLParser(tokens);
        parser.parse();
			
		result.success = true;
		result.isAction = parser.isAction;
		FIPAAction action = new FIPAAction();
		action.actionName = parser.actionName;
		action.paramName = new List<string>();
		action.paramValue = new List<string>();
		
		action.paramName = parser.paramName;
		action.paramValue = parser.paramValue;
		
		result.action = action;
		System.Console.WriteLine("MSG : " + action.actionName);
		return result;
	}
	
}
}

