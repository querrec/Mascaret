using System.Collections;
using System.Collections.Generic;

namespace Mascaret
{
    public class ManageOrganisationalStructureServlet : HttpServlet
    {

        public override void manageRequest(HttpRequest req)
        {
            string structName = req.parameters["alias"];

            req.response.write("<html>");
            req.response.write("<META HTTP-EQUIV=\"Content-Type\" content=\"text/html; charset=UTF-8\">");
            req.response.write("<body>");

            OrganisationalStructure struc = null;
            List<OrganisationalStructure> structs = MascaretApplication.Instance.AgentPlateform.Structures;
            bool found = false;
            int iO = 0;
            while (!found && iO < structs.Count)
            {
                if (structs[iO].name == structName)
                    found = true;
                else iO++;
            }
            if (found) struc = structs[iO];

            req.response.write("<H2>Description</H2>");
            req.response.write(struc.name);

            req.response.write("<H2>Role</H2>");
            List<Role> roles = struc.Roles;
            req.response.write("<ul>");
            for (int i = 0; i < roles.Count; i++)
            {
                req.response.write("<li>");
                req.response.write(roles[i].name);
                req.response.write(" : <a href=\"RoleClass?alias=");
                req.response.write(struc.name);
                req.response.write("&role=");
                req.response.write(roles[i].name);
                req.response.write("\" target = \"Body\">");
                req.response.write(roles[i].RoleClass.name);
                req.response.write("</a></li>");
            }
            req.response.write("</ul>");

            /*
                req->getResponse()->write("<H2>Entites</H2>");
                vector< shared_ptr<OrganisationalEntity> > entites = struc->getEntities();
                req->getResponse()->write("<ul>");
                for (size_t i = 0; i < entites.size(); i++)
                {
                    req->getResponse()->write("<li>");
                    req->getResponse()->write("<a href=\"OrgEntity?alias=");
                    req->getResponse()->write(entites[i]->getName());
                    req->getResponse()->write("\" target = \"Body\">");
                    req->getResponse()->write(entites[i]->getName());
                    req->getResponse()->write("</a></li>");
                }
                req->getResponse()->write("</ul>");
            */

            req.response.write("</body>");
            req.response.write("</html>");

        }

    }
}
