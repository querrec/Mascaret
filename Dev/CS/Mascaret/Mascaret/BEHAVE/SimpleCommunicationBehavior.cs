using System;
using System.Collections.Generic;
using Antlr4.Runtime;
using Antlr4.Runtime.Atn;
using Antlr4.Runtime.Misc;
using DFA = Antlr4.Runtime.Dfa.DFA;

namespace Mascaret
{ 
public struct FIPAAction
{
	public string actionName;
    public string performer;
	public List<string> paramName;
	public List<string> paramValue;
}

public struct FIPAIota
{
    public string predicate;
    public string result;
    public List<string> paramName;
    public string value;
}

public struct FIPASLParserResult
{
	public bool success;
	public bool isAction;
    public bool isIota;
    public bool isEqual;
    public bool isDone;
    public bool isStarted;
	public FIPAAction action;
    public FIPAIota iota;
};

[Serializable]
public class SimpleCommunicationBehavior:CyclicBehaviorExecution
{	
	public SimpleCommunicationBehavior(Behavior specif, InstanceSpecification host, Dictionary<String,ValueSpecification> p) : base(specif, host,p)
	{
		
	}

    public SimpleCommunicationBehavior()
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
                    Dictionary<string, ValueSpecification> parameters = new Dictionary<string, ValueSpecification>();

                    if (result.action.paramName.Count > 0)
                    {
                        for (int i = 0; i < result.action.paramName.Count; i++)
                        {
                            LiteralString stringValue = new LiteralString(result.action.paramValue[i]);
                            parameters.Add(result.action.paramName[i], (ValueSpecification)(stringValue));
                        }
                    }
                    else
                    {
                        for (int i = 0; i < result.action.paramName.Count; i++)
                        {
                            LiteralString stringValue = new LiteralString(result.action.paramValue[i]);
                            parameters.Add("param" + i + '0', (ValueSpecification)(stringValue));
                        }
                    }

                    BehaviorScheduler.Instance.executeBehavior(behavior, agt, parameters, false);
                    ACLMessage reponse = new ACLMessage(ACLPerformative.AGREE);
                    reponse.Content = msg.Content;
                    reponse.Receivers.Add(msg.Sender);
                    agt.send(reponse);
                    done = true;
                }

                Class classifier = agt.Classifier;
                Operation operation = null;
                if (classifier != null && classifier.Operations.ContainsKey(result.action.actionName))
                {
                    operation = classifier.Operations[result.action.actionName];
                    //System.Console.WriteLine("Operation:" + operation.getFullName());
                }
                if (operation != null)
                {
                    Dictionary<string, ValueSpecification> param = new Dictionary<string, ValueSpecification>();
                    List<Parameter> parameters = operation.Parameters;
                    List<string> paramValue = result.action.paramValue;
                    List<string> paramName = result.action.paramName;

                    if (parameters.Count == paramValue.Count)
                    {

                        for (int i = 0; i < parameters.Count; i++)
                        {
                            // Pour tous les parametres issus de l'operation
                            string parameterName = parameters[i].name;

                            // Cherche l'indice de ce parameter dans paramsName
                            int indice = 0; bool found = false;
                            while (!found && indice < paramName.Count)
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
                                string strVal = paramValue[indice];

                                System.Console.Write("Type : " + parameters[i].Type.getFullName());
                                System.Console.Write(" - Name: " + parameters[i].name);
                                System.Console.WriteLine(" - Value: " + strVal);

                                string typeName = parameters[i].Type.getFullName();
                                if (typeName == "boolean" || typeName == "integer" || typeName == "real" || typeName == "string")
                                    param.Add(parameters[i].name, parameters[i].Type.createValueFromString(strVal.ToLower()));
                                else
                                {
                                    try
                                    {
                                        InstanceSpecification inst = MascaretApplication.Instance.getEnvironment().getInstance(strVal.ToLower());
                                        param.Add(parameters[i].name, new InstanceValue(inst));
                                    }
                                    catch (NullReferenceException e)
                                    {

                                    }

                                }


                            }
                        }

                        //BehaviorScheduler.Instance.executeBehavior(operation.Method, agt, param, false);
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
						*/
                                BehaviorExecution be = BehaviorScheduler.Instance.executeBehavior(operation.Method, agt, param, false);
				/*
                                if (be != null)
                                {
                                    be.addCallbackOnBehaviorStop(bind(&SimpleCommunicationBehavior::_onBehaviorStop,this,_1));
							
                                    _requestedAction[be->getSpecification()->getName()].push_back(msg->getSender());
                                }*/
                           // }
						
                        

                        ACLMessage reponse = new ACLMessage(ACLPerformative.AGREE);
                        reponse.Content = msg.Content;

                        System.Console.WriteLine("Content-Sent: " + reponse.Content);
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

                        for (int iAss = 0; iAss < assigns.Count; iAss++)
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
                        ACLMessage reponse = new ACLMessage(ACLPerformative.AGREE);
                        reponse.Content = msg.Content;
                        reponse.Receivers.Add(msg.Sender);
                        agt.send(reponse);

                        // Recherche du comportement procedural
                        // UGGLY
                        //System.Console.WriteLine("1");
                        AgentBehaviorExecution pbehavior = agt.getBehaviorExecutingByName("ProceduralBehavior");

                        //AgentBehaviorExecution> behavior2 = agt->getBehaviorExecutionByName("ActionListenerBehavior");
                        //System.Console.WriteLine("2");
                        if (pbehavior != null)
                        {
                            ProceduralBehavior procBehave = (ProceduralBehavior)(pbehavior);

                            Dictionary<string, ValueSpecification> procParams = new Dictionary<string, ValueSpecification>();
                            for (int i = 0; i < result.action.paramName.Count; i++)
                            {
                                LiteralString stringValue = new LiteralString(result.action.paramValue[i]);
                                procParams.Add(result.action.paramName[i], stringValue);
                            }

                            //System.Console.WriteLine("3");
                            Activity act = askedProc.Activity;
                            //List<ActivityNode>  nodes  = act.Nodes;

                            procBehave.pushProcedureToDo(askedProc, askedOrg, askedRole, procParams);
                            //procBehave.restart();

                            ACLMessage reponse2 = new ACLMessage(ACLPerformative.AGREE);
                            reponse2.Content = msg.Content;
                            reponse2.Receivers.Add(msg.Sender);
                            agt.send(reponse2);
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
                                ACLMessage reponse3 = new ACLMessage(ACLPerformative.NOT_UNDERSTOOD);
                                string contentRep = "";
                                contentRep += result.action.actionName;
                                reponse3.Content = contentRep;
                                reponse3.Receivers.Add(msg.Sender);
                                agt.send(reponse3);
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
            string contentRep = "";
            contentRep += content;

            reponse.Content = contentRep;
            reponse.Receivers.Add(msg.Sender);
            agt.send(reponse);
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
        Agent agt = (Agent)(this.Host);
        string content = msg.Content;
        FIPASLParserResult result = parseFipaSLExpression(content);
        if (result.isAction)
        {
            if (result.success)
            {

                AgentBehaviorExecution be = agt.getBehaviorExecutingByName("ProceduralBehavior");
                if (be != null)
                {
                    ProceduralBehavior pbe = (ProceduralBehavior)(be);
                    AID aid = new AID(result.action.performer, agt.Aid.PlateformName, agt.Aid.PlateformPort);
                    if (result.isDone)
                    {

                        aid = new AID(result.action.performer, agt.Aid.PlateformName, agt.Aid.PlateformPort);
                        pbe.onActionDone(aid, result.action.actionName);
                    }
                    else
                    {

                        pbe.onActionRunning(aid, result.action.actionName);
                    }
                }
            }
        }
        else if (result.isEqual)
        {
            string entityName = result.iota.paramName[2];
            string slotName = result.iota.paramName[0];
            string value = result.iota.value ;

            MascaretApplication.Instance.VRComponentFactory.Log(entityName + "." + slotName + "=" + value);

            KnowledgeBase kb = ((Agent)(this.Host)).KnowledgeBase;
            Mascaret.Environment envKB = kb.Environment;
            List<Entity> entities = envKB.getEntities();
            foreach (Entity entity in entities)
            {
                if (entity.name == entityName)
                {
                    MascaretApplication.Instance.VRComponentFactory.Log("ENTITY ...");
                    foreach (KeyValuePair<string, Slot> s in entity.Slots)
                    {
                        if (s.Value.name == slotName)
                        {
                           s.Value.addValueFromString(value);
                        }
                    }
                }

            }
        }
       
	}
	
	protected void manageQueryRef(ACLMessage msg)
	{
        /*
        Dictionary<string, Agent> agents = VRApplication.Instance.AgentPlateform.Agents;
             
        string agentName = msg.Sender.toString();
        //MascaretApplication.Instance.VRComponentFactory.Log("Sender : " + agentName + " - Host: "+this.Host.name);
        if (agents.ContainsKey(agentName))
        {
            Mascaret.Agent agt = agents[agentName];

            ProceduralBehavior pbehavior = (ProceduralBehavior)(agt.getBehaviorExecutingByName("ProceduralBehavior"));
            List<ProcedureExecution> procedures = pbehavior.runningProcedures;
            //MascaretApplication.Instance.VRComponentFactory.Log("M-Procedures : " + procedures.Count);
            foreach (ProcedureExecution proc in procedures)
                MascaretApplication.Instance.VRComponentFactory.Log(proc.procedure.getFullName());

            List<KeyValuePair<ProcedureExecution, ActionNode>> actionNodes = pbehavior.actionsToDo;
            //MascaretApplication.Instance.VRComponentFactory.Log("M-Action Nodes Count: " + actionNodes.Count);

            for (int atd = 0; atd < actionNodes.Count; ++atd)
            {
                if (actionNodes[atd].Value.Action.GetType().ToString() == "Mascaret.CallOperationAction")
                {
                    //MascaretApplication.Instance.VRComponentFactory.Log("Action Node Description: "+actionNodes[atd].Value.Description);
                    string fml=null;
                    if (msg.Content == "action")
                    {
                        fml = "<FML><Performative>Inform</Performative><Receivers><Receiver>" + agentName + "</Receiver></Receivers><Content>" + actionNodes[atd].Value.Description + "</Content><Emotion>Neutral</Emotion><Ressources><Ressource>" + "" + "</Ressource></Ressources></FML>";
                    }
                    else if (msg.Content == "object")
                    {
                        CallOperationAction coa = (CallOperationAction)(actionNodes[atd].Value.Action);
                        List<InputPin> pins = coa.InputPins;
                        if (pins.Count >= 1)
                        {
                            InputPin ip = pins[0];
                            List<ObjectNode> nObjNode = ip.getIncomingObjectNode();

                            if (nObjNode.Count >= 1)
                            {
                                ObjectNode on = nObjNode[0];
                                foreach (ProcedureExecution pe in procedures)
                                {
                                    Dictionary<string, InstanceSpecification> affect = pe.getAffectations();
                                    if (affect.ContainsKey(on.name))
                                    {
                                        InstanceSpecification instance = affect[on.name];
                                        fml = "<FML><Performative>Point</Performative><Receivers><Receiver>" + agentName + "</Receiver></Receivers><Content>" + "" + "</Content><Emotion>Neutral</Emotion><Ressources><Ressource>" + instance.name + "</Ressource></Ressources></FML>";
                                    }
                                }
                            }
                        }
                    }
                    
                    if(fml != null) ((EmbodiedAgent)(Host)).addIntention(fml);
                }
            }
        }*/

        //////////////////////////////////////////////////bilal Sep2015////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        string content = msg.Content;

        FIPASLParserResult result = parseFipaSLExpression(content);
        
        Dictionary<string, Agent> agents = VRApplication.Instance.AgentPlateform.Agents;

        if (result.isIota)
        {
            FIPAIota iota = result.iota;
            if (iota.predicate == "slot")
            {
                string res = getSlot(iota.paramName);
                MascaretApplication.Instance.VRComponentFactory.Log("IOTA : " + iota.predicate + " " + iota.result + " == " + res);
                if (res != "")
                {
                    // give the intention
                    ACLMessage aclMsg = new ACLMessage(ACLPerformative.INFORM);
                    aclMsg.Sender = ((Agent)Host).Aid;
                    aclMsg.Receivers.Add(msg.Sender);
                    aclMsg.Content = "((= (iota ?" + iota.paramName[0] + " (" + iota.predicate + " ?" + iota.paramName[1] + " ?" + iota.paramName[2] + ")) " + res + "))";
                    ((Agent)Host).send(aclMsg);
                    MascaretApplication.Instance.VRComponentFactory.Log("Inform message: "+aclMsg.Content);
                }
            }
        }
        

        /*string agentName = msg.Sender.toString();
        MascaretApplication.Instance.VRComponentFactory.Log("Sender : " + agentName + " - Host: "+this.Host.name);
        if (agents.ContainsKey(agentName))
        {
            Mascaret.Agent agt = agents[agentName];
            KnowledgeBase kb = agt.KnowledgeBase;
            Mascaret.Environment envKB = kb.Environment;
            List<Entity> entitiesKB = envKB.getEntities();
            foreach (Entity entity in entitiesKB)
            {
                MascaretApplication.Instance.VRComponentFactory.Log(entity.getFullName());
                if (entity.Slots.Count > 0)
                {
                    foreach (KeyValuePair<string, Slot> s in entity.Slots)
                    {
                        if (s.Value.getValue() != null)
                            MascaretApplication.Instance.VRComponentFactory.Log(s.Key + " = " + s.Value.getValue().getStringFromValue());
                    }
                }
            }

        }*/
        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
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
       
        FipaSLLexer lex = new FipaSLLexer(new AntlrInputStream(content));
        CommonTokenStream tokens = new CommonTokenStream(lex);
        FipaSLParser parser = new FipaSLParser(tokens);
        parser.content();
        MascaretApplication.Instance.VRComponentFactory.Log("Parsing  : " + content);
        MascaretApplication.Instance.VRComponentFactory.Log("Nb Erreur : " + parser.NumberOfSyntaxErrors);

        if (parser.isAction)
        {

            result.success = true;
            result.isAction = parser.isAction;
            result.isDone = parser.done;
            result.isStarted = parser.started;
            FIPAAction action = new FIPAAction();

            action.actionName = parser.actionName;
            action.performer = parser.performer;
            action.paramName = new List<string>();
            action.paramValue = new List<string>();
            action.paramName = parser.paramName;
            action.paramValue = parser.paramValue;
            MascaretApplication.Instance.VRComponentFactory.Log("Action Name: " + action.actionName);
            MascaretApplication.Instance.VRComponentFactory.Log("Performer: " + action.performer);
            result.action = action;
            MascaretApplication.Instance.VRComponentFactory.Log("MSG : " + action.actionName);

            for (int i = 0; i < action.paramName.Count; i++)
            {
                MascaretApplication.Instance.VRComponentFactory.Log("MSG" + (i + 2) + " : " + action.paramName[i]);
                MascaretApplication.Instance.VRComponentFactory.Log("MSG" + (i + 3) + " : " + action.paramValue[i]);
            }
            //System.Console.WriteLine("MSG4 : " + action.paramName[1]);
            //System.Console.WriteLine("MSG5 : " + action.paramValue[1]);
        }
        else if (parser.isEqual)
        {
            if (parser.isIota)
            {
                result.isEqual = true;
                result.isIota = true;
                result.iota = new FIPAIota();
                result.iota.predicate = parser.predicateSymbol;
                result.iota.result = parser.askedTerm;
                result.iota.paramName = new List<string>();
                result.iota.paramName = parser.paramName;
                result.iota.value = parser.value;
            }
        }
        else if (parser.isIota)
        {
            result.isIota = true;
            result.iota = new FIPAIota();
            result.iota.predicate = parser.predicateSymbol;
            result.iota.result = parser.askedTerm;
            result.iota.paramName = new List<string>();
            result.iota.paramName = parser.paramName;

            MascaretApplication.Instance.VRComponentFactory.Log("IOTA : " + parser.predicateSymbol + " " + parser.askedTerm + " : " + parser.paramName.Count);


        }
		return result;
	}

    protected string getSlot(List<string> p)
    {
        string entityName = p[2];
        string slotName = p[1];

        MascaretApplication.Instance.VRComponentFactory.Log(entityName + "." + slotName);
        string value = "";

        KnowledgeBase kb = ((Agent)(this.Host)).KnowledgeBase;
        Mascaret.Environment envKB = kb.Environment;
        List<Entity> entities = envKB.getEntities();
        foreach (Entity entity in entities)
        {
            if (entity.name == entityName)
            {
                MascaretApplication.Instance.VRComponentFactory.Log("ENTITY ...");
                foreach (KeyValuePair<string, Slot> s in entity.Slots)
                {
                    if (s.Value.name == slotName)
                    {
                        value = s.Value.getValue().getStringFromValue();
                    }
                }
            }
            
        }
        
        return value;
    }
	
}
}

