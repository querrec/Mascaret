using System.Collections;
using System.Collections.Generic;

namespace Mascaret
{
    public class ManageProcedureServlet : HttpServlet
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

            req.response.write("<HR>");
            req.response.write("<H2>Parametres</H2>");
            req.response.write("<FORM METHOD=GET action=\"launchProcedure\">");
            req.response.write("<INPUT type=\"hidden\" name=\"org\" value=\"");
            req.response.write(orgName);
            req.response.write("\"/>");
            req.response.write("<INPUT type=\"hidden\" name=\"alias\" value=\"");
            req.response.write(procName);
            req.response.write("\"/>");
            req.response.write("<ul>");
            for (int iParam = 0; iParam < parameters.Count; iParam++)
            {
                Parameter param = parameters[iParam];
                req.response.write("<li>");
                Classifier type = param.Type;
                req.response.write(type.name);
                req.response.write("\t");
                req.response.write(param.name);
                if (_isBaseType2(type))
                {
                    req.response.write(" = ");
                    req.response.write(" <INPUT name=_");
                    req.response.write(param.name);
                    req.response.write(">");
                    req.response.write("</li>");
                }
                else
                {
                    req.response.write(" <select name=\"_");
                    req.response.write(param.name);
                    req.response.write("\">");

                    foreach (KeyValuePair<string, InstanceSpecification> instance in env.InstanceSpecifications)
                    {
                        if (instance.Value.Classifier != null)
                        {
                            if (instance.Value.Classifier.isA(type))
                            {
                                req.response.write(" = <option value=\"");
                                req.response.write(instance.Key);
                                req.response.write("\">");
                                req.response.write(instance.Key);
                                req.response.write("</option>");
                            }
                        }
                    }
                    req.response.write("</select>");
                }
            }
            req.response.write("</ul>");
            req.response.write("<INPUT TYPE=\"submit\" VALUE=\"Lancer\">");
            req.response.write("</FORM>");

            req.response.write("</body>");
            req.response.write("</html>");
        }

        bool _isBaseType2(Classifier type)
        {
            if (type.name == "integer") return true;
            if (type.name == "real") return true;
            if (type.name == "string") return true;
            else return false;
        }

    }
}
