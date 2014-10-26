using System.Collections;
using System.Collections.Generic;

namespace Mascaret
{
    public class ManageOrganisationalEntityServlet : HttpServlet
    {

        public override void manageRequest(HttpRequest req)
        {
            string orgName = req.parameters["alias"];

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

            req.response.write("<H2>Description</H2>");
            req.response.write(org.name);

            req.response.write("<H2>Affectations</H2>");
            List<RoleAssignement> assignements = org.RoleAssignement;
            req.response.write("<ul>");
            for (int iA = 0; iA < assignements.Count; iA++)
            {
                string agentAid = assignements[iA].Agent.toString();
                Role role = assignements[iA].Role;
                req.response.write("<li>");
                req.response.write(agentAid);
                req.response.write(" / ");
                req.response.write(role.name);
                req.response.write(" : <a href=\"RoleClass?alias=");
                req.response.write(struc.name);
                req.response.write("&role=");
                req.response.write(role.name);
                req.response.write("\" target = \"Body\">");
                req.response.write(role.RoleClass.name);
                req.response.write("</a></li>");
            }
            req.response.write("</ul>");

            req.response.write("<H2>Procedures</H2>");
            List<Procedure> procs = struc.Procedures;
            req.response.write("<ul>");
            for (int iP = 0; iP < procs.Count; iP++)
            {
                req.response.write("<li>");
                req.response.write("<a href=\"Procedure?alias=");
                req.response.write(procs[iP].name);
                req.response.write("&org=");
                req.response.write(org.name);
                req.response.write("\" target = \"Body\">");
                req.response.write(procs[iP].name);
                req.response.write("</a></li>");
            }
            req.response.write("</ul>");

            req.response.write("<H2>Structure</H2>");
            req.response.write("<a href=\"OrgStruct?alias=");
            req.response.write(struc.name);
            req.response.write("\" target = \"Body\">");
            req.response.write(struc.name);
            req.response.write("</a>");

            req.response.write("</body>");
            req.response.write("</html>");
        }

    }
}
