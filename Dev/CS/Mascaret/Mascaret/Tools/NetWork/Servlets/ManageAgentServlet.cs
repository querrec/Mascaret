using System;
using System.Collections;
using System.Collections.Generic;

namespace Mascaret
{
    public class ManageAgentServlet : HttpServlet
    {
        public override void manageRequest(HttpRequest req)
        {
            string id = req.parameters["alias"];

            Environment env = MascaretApplication.Instance.getEnvironment();

            if (!env.InstanceSpecifications.ContainsKey(id))
            {
                req.response.write("<html>");
                req.response.write("<body>");
                req.response.write("Can't find entity: " + id);
                req.response.write("</body>");
                req.response.write("</html>");
                return;
            }
            InstanceSpecification entity = env.InstanceSpecifications[id];

            VirtualHuman human = null;
            try
            {
                human = (VirtualHuman)(entity);
            }
            catch (InvalidCastException e)
            {
                req.response.write("<html>");
                req.response.write("<body>");
                req.response.write("Entity: " + id + " is not an agent");
                req.response.write("</body>");
                req.response.write("</html>");
                return;
            }

            req.response.write("<html>");
            req.response.write("<body>");
            req.response.write("<META HTTP-EQUIV=\"Refresh\" CONTENT=\"30\">");
            req.response.write("<META HTTP-EQUIV=\"Content-Type\" content=\"text/html; charset=UTF-8\">");
            req.response.write("<link href=\"doxygen.css\" rel=\"stylesheet\" type=\"text/css\">");
            req.response.write("<link href=\"tabs.css\" rel=\"stylesheet\" type=\"text/css\">");

            req.response.write("<H2>Description</H2>");
            req.response.write("<ul>");
            req.response.write("<li>");
            req.response.write(human.name);
            req.response.write("</li>");
            req.response.write("<li>");
            req.response.write(human.getFullName());
            req.response.write("</li>");
            req.response.write("<li>");
            req.response.write(human.Description);
            req.response.write("</li>");
            req.response.write("<li>");
            req.response.write(" <a href=\"Class?alias=");
            req.response.write(human.Classifier.name);
            req.response.write("\" target = \"Body\">");
            req.response.write("</a>");
            req.response.write("</li>");
            req.response.write("</ul>");

            string setControlled = req.parameters["setControlled"];
            if (setControlled == "true")
            {
                human.ControlledByHuman = true;
            }
            else if (setControlled == "false")
            {
                human.ControlledByHuman = false;
            }


            req.response.write("<HR>");
            req.response.write("<H2>Toggle controlled by human</H2>");

            req.response.write("<FORM METHOD=POST action=\"Agent?alias=" + id + "\">");
            req.response.write("<input type=\"hidden\" name=\"alias\" value=\"" + id + "\" />");
            if (human.ControlledByHuman)
            {
                req.response.write("<font color=\"darkred\">This agent is currently controlled. (will not follow procedure automatically)</font><br />");
                req.response.write("<input type=\"hidden\" name=\"setControlled\" value=\"false\" />");
                req.response.write("<input type=\"submit\" name=\"control\" value=\"Release control of this agent\" />");
            }
            else
            {
                req.response.write("<font color=\"darkgreen\">This agent is currently automatic. (will automatically follow procedures)</font><br />");
                req.response.write("<input type=\"hidden\" name=\"setControlled\" value=\"true\" />");
                req.response.write("<input type=\"submit\" name=\"control\" value=\"Take control of this agent\" />");
            }
            req.response.write("</FORM>");

            req.response.write("<HR>");
            req.response.write("<H2>Knowledge base</H2>");
            KnowledgeBase kb = human.KnowledgeBase;
            if (kb != null)
            {
                req.response.write(" <a href=\"KnowledgeBase?alias=");
                req.response.write(human.name);
                req.response.write("\" target = \"_blank\">");
                req.response.write(kb.name);
                req.response.write("</a>");
            }

            req.response.write("<HR>");
            req.response.write("<H2>Parler</H2>");
            req.response.write("<FORM METHOD=GET action=\"speak\">");
            req.response.write("<input type=\"hidden\" name=\"alias\" value=\"");
            req.response.write(id);
            req.response.write("\" />");
            req.response.write("Texte : \t <INPUT name=\"text\">");
            req.response.write("<INPUT TYPE=\"submit\" VALUE=\"Dire\">");
            req.response.write("</FORM>");

            req.response.write("<HR>");
            req.response.write("<H2>Attributs</H2>");
            req.response.write("<FORM METHOD=GET action=\"changeAttributes\">");
            req.response.write("<input type=\"hidden\" name=\"alias\" value=\"");
            req.response.write(id);
            req.response.write("\" />");
            req.response.write("<ul>");

            Dictionary<string, Slot> attributes = human.Slots;
            foreach (KeyValuePair<string, Slot> attr in attributes)
            {
                req.response.write("<li>");
                req.response.write(attr.Key);
                req.response.write(" = ");
                //string value = it->second->getValue().getStringFromValue();
                string value = "";
                foreach (KeyValuePair<string, ValueSpecification> val in attr.Value.Values)
                {
                    value += "'" + val.Value.getStringFromValue() + "' ";
                }
                req.response.write(value);
                req.response.write("</li>");
            }
            req.response.write("</ul>");
            req.response.write("<INPUT TYPE=\"submit\" VALUE=\"Modifier\">");
            req.response.write("</FORM>");
            //req.response.flushBuffer();

            req.response.write("<HR>");
            req.response.write("<H2>Operations</H2>");
            req.response.write("<ul>");
            Class classifier = human.Classifier;
            Dictionary<string, Operation> operations = classifier.Operations;
            foreach (KeyValuePair<string, Operation> operation in operations)
            {
                req.response.write("<li>");
                req.response.write(" <a href=\"Operation?alias=");
                req.response.write(human.name);
                req.response.write("&oper=");
                req.response.write(operation.Key);
                req.response.write("\" target = \"Body\">");
                req.response.write(operation.Key);
                req.response.write("</a>");
                req.response.write("</li>");
            }
            req.response.write("</ul>");
            //req.response.flushBuffer();




            req.response.write("<HR>");
            req.response.write("<H2>Signaux</H2>");
            req.response.write("<ul>");
            Dictionary<string, Behavior> behaviors = classifier.OwnedBehavior;
            foreach (KeyValuePair<string, Behavior> behavior in behaviors)
            {
                StateMachine stateMachine = (StateMachine)(behavior.Value);
                Region region = stateMachine.Region[0];
                if (region != null)
                {
                    List<Transition> transitions = region.Transitions;
                    for (int iTrans = 0; iTrans < transitions.Count; iTrans++)
                    {
                        List<Trigger> triggers = transitions[iTrans].Trigger;
                        for (int iTrig = 0; iTrig < triggers.Count; iTrig++)
                        {
                            MascaretEvent evt = triggers[iTrig].MEvent;
                            if (evt != null)
                            {
                                if (evt.Type == "SignalEvent")
                                {
                                    SignalEvent signalEvent = (SignalEvent)(evt);

                                    req.response.write("<li>");
                                    req.response.write(" <a href=\"Signal?alias=");
                                    req.response.write(human.name);
                                    req.response.write("&signal=");
                                    req.response.write(((SignalEvent)(evt)).SignalClass.name);
                                    req.response.write("\" target = \"Body\">");
                                    req.response.write(((SignalEvent)(evt)).SignalClass.name);
                                    req.response.write("</a>");
                                    req.response.write("</li>");
                                }
                            }
                        }
                    }
                }
            }
            req.response.write("</ul>");

            req.response.write("</ul>");
            req.response.write("<HR>");
            req.response.write("<H2>Messages</H2>");
            req.response.write(" AID : ");
            req.response.write(human.Aid.toString());
            req.response.write("<H3>");
            req.response.write(" <a href=\"createMessage?alias=");
            req.response.write(human.name);
            req.response.write("\" target = \"Body\">");
            req.response.write("Envoyer un message");
            req.response.write("</a>");
            req.response.write("</H3>");
            req.response.write("<H3>Non lus</H3>");
            List<ACLMessage> nl = human.Mailbox.MessagesQueue;
            req.response.write("<TABLE BORDER=1>");
            req.response.write("<TR>");
            req.response.write("<TD>De </TD>");
            req.response.write("<TD>Performative </TD>");
            req.response.write("<TD>Contenu </TD>");
            req.response.write("</TR>");
            for (int inl = 0; inl < nl.Count; inl++)
            {
                ACLMessage msg = nl[inl];
                //nl.erase(nl.begin());
                req.response.write("<TR>");
                req.response.write("<TD>");
                req.response.write(msg.Sender.toString());
                req.response.write("</TD>");
                req.response.write("<TD>");
                req.response.write(msg.getPerformativeText());
                req.response.write("</TD>");
                req.response.write("<TD>");
                req.response.write(msg.Content);
                req.response.write("</TD>");
                req.response.write("</TR>");
            }
            req.response.write("</TABLE>");

            req.response.write("<H3>Lus</H3>");
            List<ACLMessage> l = human.Mailbox.MessagesChecked;
            req.response.write("<TABLE BORDER=1>");
            req.response.write("<TR>");
            req.response.write("<TD>De </TD>");
            req.response.write("<TD>Performative </TD>");
            req.response.write("<TD>Contenu </TD>");
            req.response.write("</TR>");
            for (int il = 0; il < l.Count; il++)
            {
                ACLMessage msg = l[il];
                req.response.write("<TR>");
                req.response.write("<TD>");
                req.response.write(msg.Sender.toString());
                req.response.write("</TD>");
                req.response.write("<TD>");
                req.response.write(msg.getPerformativeText());
                req.response.write("</TD>");
                req.response.write("<TD>");
                req.response.write(msg.Content);
                req.response.write("</TD>");
                req.response.write("</TR>");
            }
            req.response.write("</TABLE>");

            req.response.write("<H3>Envoyes</H3>");
            List<ACLMessage> me = human.Mailbox.MessagesSent;
            req.response.write("<TABLE BORDER=1>");
            req.response.write("<TR>");
            req.response.write("<TD>A </TD>");
            req.response.write("<TD>Performative </TD>");
            req.response.write("<TD>Contenu </TD>");
            req.response.write("</TR>");
            for (int ie = 0; ie < me.Count; ie++)
            {
                ACLMessage msg = me[ie];
                req.response.write("<TR>");
                req.response.write("<TD>");
                List<AID> receivers = msg.Receivers;
                for (int ir = 0; ir < receivers.Count; ir++)
                {
                    req.response.write(receivers[ir].toString());
                }
                req.response.write("</TD>");
                req.response.write("<TD>");
                req.response.write(msg.getPerformativeText());
                req.response.write("</TD>");
                req.response.write("<TD>");
                req.response.write(msg.Content);
                req.response.write("</TD>");
                req.response.write("</TR>");
            }
        }

    }
}
