using System;
using System.Collections.Generic;
using Antlr4.Runtime;
using Antlr4.Runtime.Atn;
using Antlr4.Runtime.Misc;
using DFA = Antlr4.Runtime.Dfa.DFA;

namespace Mascaret
{


    [Serializable]
    public class CommunicationBehavior : CyclicBehaviorExecution
    {
        public CommunicationBehavior(Behavior specif, InstanceSpecification host, Dictionary<String, ValueSpecification> p)
            : base(specif, host, p)
        {

        }

        public override void action()
        {
            Agent agt = (Agent)(this.Host);
            ACLMessage msg = agt.receive();

            if (msg != null)
            {
                MascaretApplication.Instance.VRComponentFactory.Log("Communication Behavior");
                MascaretApplication.Instance.VRComponentFactory.Log("Conversation-ID: " + msg.ConversationID);
                /*if (msg.Performative == ACLPerformative.REQUEST)
                {
                    manageRequest(msg);
                }
                else */
                if (msg.Performative == ACLPerformative.INFORM)
                {
                    manageInform(msg);
                }
                else if (msg.Performative == ACLPerformative.QUERY_REF)
                {
                    manageQueryRef(msg);
                }
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
                string value = result.iota.value;

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
            string content = msg.Content;

            FIPASLParserResult result = parseFipaSLExpression(content);

            Dictionary<string, Agent> agents = VRApplication.Instance.AgentPlateform.Agents;

            if (result.isIota)
            {
                FIPAIota iota = result.iota;
                if (iota.predicate == "slot")
                {
                    string fml;
                    string res = getSlot(iota.paramName);
                    MascaretApplication.Instance.VRComponentFactory.Log("IOTA : " + iota.predicate + " " + iota.result + " == " + res);
                    if (res != "")
                    {
                        // give the intention
                        ACLMessage aclMsg = new ACLMessage(ACLPerformative.INFORM);
                        aclMsg.Sender = ((Agent)Host).Aid;
                        aclMsg.Receivers.Add(msg.Sender);
                        aclMsg.Content = "((= (iota ?" + iota.paramName[0] + " (" + iota.predicate + " ?" + iota.paramName[1] + " ?" + iota.paramName[2] + ")) " + res + "))";
                        aclMsg.ConversationID = msg.ConversationID;
                        ((Agent)Host).send(aclMsg);
                        MascaretApplication.Instance.VRComponentFactory.Log("Inform message: " + aclMsg.Content);
                        //bilal 19-10-15
                        fml = "<FML><Performative>Inform</Performative><Receivers><Receiver>" + msg.Sender + "</Receiver></Receivers><Content>" +/* "the " + iota.paramName[1] + " of " + iota.paramName[2] + "is" +*/ res + "</Content><Emotion>Neutral</Emotion><Ressources><Ressource>" + "" + "</Ressource></Ressources></FML>";
                    }
                    //bilal 19-10-15
                    else
                        fml = "<FML><Performative>Inform</Performative><Receivers><Receiver>" + msg.Sender + "</Receiver></Receivers><Content>" + "I don't know" + /*" the value of the " + iota.paramName[1] + " of " + iota.paramName[2] +*/ "</Content><Emotion>Neutral</Emotion><Ressources><Ressource>" + "" + "</Ressource></Ressources></FML>";
                    if (fml != null) ((EmbodiedAgent)(Host)).addIntention(fml);
                    //bilal 19-10-15
                }
            }
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
