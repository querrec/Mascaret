using System.Collections;
using System.Collections.Generic;

namespace Mascaret
{
    public class ManageEntityServlet : HttpServlet
    {

        public override void manageRequest(HttpRequest req)
        {
            string id = "";
            if (req.parameters.ContainsKey("agent"))
                id = req.parameters["agent"];

            Environment env = MascaretApplication.Instance.getEnvironment();
            InstanceSpecification entity = null;
            if (env.InstanceSpecifications.ContainsKey(id))
                entity = env.InstanceSpecifications[id];

            VirtualHuman human = null;
            if (entity != null) human = (VirtualHuman)(entity);
            if (human != null) env = human.KnowledgeBase.Environment;

            id = req.parameters["alias"];
            if (!env.InstanceSpecifications.ContainsKey(id))
            {
                req.response.write("<html>");
                req.response.write("<body>");
                req.response.write("Can't find entity: " + id);
                req.response.write("</body>");
                req.response.write("</html>");
                return;
            }
            entity = env.InstanceSpecifications[id];


            req.response.write("<html>");
            req.response.write("<body>");
            req.response.write("<META HTTP-EQUIV=\"Refresh\" CONTENT=\"30\">");
            req.response.write("<META HTTP-EQUIV=\"Content-Type\" content=\"text/html; charset=UTF-8\">");
            req.response.write("<link href=\"doxygen.css\" rel=\"stylesheet\" type=\"text/css\">");
            req.response.write("<link href=\"tabs.css\" rel=\"stylesheet\" type=\"text/css\">");

            req.response.write("<H2>Description</H2>");
            req.response.write("<ul>");
            req.response.write("<li>");
            req.response.write(entity.name);
            req.response.write("</li>");
            req.response.write("<li>");
            req.response.write(entity.getFullName());
            req.response.write("</li>");
            req.response.write("<li>");
            req.response.write(entity.Description);
            req.response.write("</li>");
            req.response.write("<li>");
            req.response.write(" <a href=\"Class?alias=");
            req.response.write(entity.Classifier.name);
            req.response.write("\" target = \"Body\">");
            req.response.write("</a>");
            req.response.write("</li>");
            req.response.write("</ul>");
            //req.response.flushBuffer();

            Vector3 position = new Vector3(0, 0, 0);
            Vector3 orientation = new Vector3(0, 0, 0);

            Entity entity2 = (Entity)(entity);
            if (entity2 != null)
            {
                //position = entity2.getLocalPosition();
                //orientation = entity2.getLocalOrientation();

                req.response.write("<HR>");
                req.response.write("<H2>Geometrie</H2>");

                req.response.write("<FORM METHOD=GET action=\"changeGeometry\">");
                req.response.write("<input type=\"hidden\" name=\"alias\" value=\"");
                req.response.write(id);
                req.response.write("\" />");
                req.response.write("<TABLE BORDER=1>");
                req.response.write("<TR>");
                req.response.write("<TD>Position locale</TD>");
                req.response.write("<TD>");
                req.response.write("" + position.x);
                req.response.write("</TD>");
                req.response.write("<TD>");
                req.response.write("" + position.y);
                req.response.write("</TD>");
                req.response.write("<TD>");
                req.response.write("" + position.z);
                req.response.write("</TD>");
                req.response.write("</TR>");
                req.response.write("<TR>");
                req.response.write("<TD>Orientation locale</TD>");
                req.response.write("<TD>");
                req.response.write("" + orientation.x);
                req.response.write("</TD>");
                req.response.write("<TD>");
                req.response.write("" + orientation.y);
                req.response.write("</TD>");
                req.response.write("<TD>");
                req.response.write("" + orientation.z);
                req.response.write("</TD>");
                req.response.write("</TR>");
                req.response.write("</TABLE>");

                req.response.write("<INPUT TYPE=\"submit\" VALUE=\"Modifier\">");
                req.response.write("</FORM>");
                //req.response.flushBuffer();
            }


            req.response.write("<HR>");
            req.response.write("<H2>Attributs</H2>");
            req.response.write("<FORM METHOD=GET action=\"changeAttributes\">");
            req.response.write("<input type=\"hidden\" name=\"alias\" value=\"");
            req.response.write(id);
            req.response.write("\" />");
            req.response.write("<ul>");

            Dictionary<string, Slot> attributes = entity.Slots;
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
            Class classifier = entity.Classifier;
            Dictionary<string, Operation> operations = classifier.Operations;
            foreach (KeyValuePair<string, Operation> operation in operations)
            {
                req.response.write("<li>");
                req.response.write(" <a href=\"Operation?alias=");
                req.response.write(entity.name);
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
                                    req.response.write(entity.name);
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
            //req.response.flushBuffer();


            req.response.write("<HR>");
            req.response.write("<H2>Machines d'états</H2>");
            req.response.write("<ul>");
            List<StateMachineBehaviorExecution> stmbes = entity.SmBehaviorExecutions;
            for (int i = 0; i < stmbes.Count; i++)
            {
                Vertex state = stmbes[i].CurrentState;
                string name = stmbes[i].getStateMachine().name;
                string state_name = "";
                if (state == null) state_name = "inconnu";
                else state_name = state.name;
                string tmp = "<li>" + name + " - &Eacute;tat actif: " + state_name + "</li>";
                req.response.write(tmp);
            }
            req.response.write("</ul>");
            req.response.write("<HR>");
            req.response.write("<a href=\"CenterView?alias=" + entity.name + "\">Centrer la vue sur cette entité</a><br/>");
            req.response.write("<a href=\"SetRed?alias=" + entity.name + "\">Mettre en rouge</a>");
            req.response.write("</body>");
            req.response.write("</html>");
            //req.response.flushBuffer();

        }
    }
}
