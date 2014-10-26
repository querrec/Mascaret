using System.Collections;
using System.Collections.Generic;

namespace Mascaret
{
    public class ManageLaunchProcedureServlet : HttpServlet
    {

        public override void manageRequest(HttpRequest req)
        {
            string procName = req.parameters["alias"];
            string orgName = req.parameters["org"];

            req.response.write("<html>");
            req.response.write("<META HTTP-EQUIV=\"Content-Type\" content=\"text/html; charset=UTF-8\">");
            req.response.write("<body>");

            OrganisationalEntity org = null;
            List<OrganisationalEntity> orgs = MascaretApplication.Instance.AgentPlateform.Organisations;
            bool found = false;
            int i = 0;
            while (!found && i < orgs.Count)
            {
                if (orgs[i].name == orgName)
                    found = true;
                else i++;
            }
            if (found) org = orgs[i];

            OrganisationalStructure struc = org.Structure;

            List<Procedure> procs = struc.Procedures;
            Procedure proc = null;
            found = false;
            i = 0;
            while (!found && i < procs.Count)
            {
                if (procs[i].name == procName)
                    found = true;
                else i++;
            }
            if (found) proc = procs[i];

            Environment env = MascaretApplication.Instance.getEnvironment();

            List<Parameter> parameters = new List<Parameter>();
            List<ActivityNode> nodes = proc.Activity.Nodes;
            for (int iNode = 0; iNode < nodes.Count; iNode++)
            {
                if (nodes[iNode].Kind == "parameter")
                    parameters.Add(((ActivityParameterNode)(nodes[iNode])).Parameter);
            }

            string paramString = "";
            Dictionary<string, ValueSpecification> param = new Dictionary<string, ValueSpecification>();
            for (int iParam = 0; iParam < parameters.Count; iParam++)
            {
                string strVal = req.parameters[parameters[iParam].name];
                strVal.Replace("+", " ");

                //Debug.Log (strVal);
                param.Add(parameters[iParam].name, parameters[iParam].Type.createValueFromString(strVal));
                paramString += " :" + parameters[iParam].name + " " + strVal;
            }

            List<RoleAssignement> assignements = org.RoleAssignement;
            for (int iA = 0; iA < assignements.Count; iA++)
            {
                ACLMessage procMsg = new ACLMessage(ACLPerformative.REQUEST);
                procMsg.Receivers.Add(assignements[iA].Agent);

                AID agentAID = MascaretApplication.Instance.Agent.Aid;
                procMsg.Sender = agentAID;
                string content = "((action ";
                content += assignements[iA].Agent.name;
                content += " ";
                content += "(" + proc.name;
                content += paramString;
                content += ")))";
                procMsg.Content = content;
                MascaretApplication.Instance.AgentPlateform.sendMessage(procMsg);

                if (agentAID.name == assignements[iA].Agent.name)
                {
                    req.response.write("<H2>Vous jouez le role ");
                    req.response.write(assignements[iA].Role.name);
                    req.response.write("</H2>");
                    req.response.write("<H3>Vous pouvez faire les actions suivantes : </H3>");

                    req.response.write("<ul>");
                    Activity act = proc.Activity;
                    List<ActivityPartition> partitions = act.Partitions;
                    for (int iPart = 0; iPart < partitions.Count; iPart++)
                    {
                        if (partitions[iPart].name == assignements[iA].Role.name)
                        {
                            nodes = partitions[iPart].Node;
                            for (int iNode = 0; iNode < nodes.Count; iNode++)
                            {
                                if (nodes[iNode].Kind == "action")
                                {
                                    req.response.write("<li>");
                                    req.response.write("<a href=\"Actions?alias=");
                                    req.response.write(nodes[iNode].name);
                                    req.response.write("&org=");
                                    req.response.write(org.name);
                                    req.response.write("&role=");
                                    req.response.write(assignements[iA].Role.name);
                                    req.response.write("&proc=");
                                    req.response.write(proc.name);
                                    req.response.write("\" target = \"Body\">");
                                    req.response.write(nodes[iNode].name);
                                    req.response.write("</a></li>");
                                }
                            }
                        }
                    }
                    req.response.write("</ul>");
                }
            }

            req.response.write("</body>");
            req.response.write("</html>");
        }
    }
}
