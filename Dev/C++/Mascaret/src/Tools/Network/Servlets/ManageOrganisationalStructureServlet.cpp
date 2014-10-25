#include "Tools/Network/Servlets/ManageOrganisationalStructureServlet.h"
#include "MascaretApplication.h"
#include "BEHAVE/OrganisationalStructure.h"
#include "BEHAVE/OrganisationalEntity.h"
#include "BEHAVE/Role.h"
#include "BEHAVE/RoleClass.h"
#include "BEHAVE/Agent.h"
#include "BEHAVE/AgentPlateform.h"

using BEHAVE::OrganisationalStructure;
using BEHAVE::Role;

void ManageOrganisationalStructureServlet::manageRequest(HttpRequest* req)
{
	string structName = req->getParameter("alias");

	req->getResponse()->write("<html>");
	req->getResponse()->write("<META HTTP-EQUIV=\"Content-Type\" content=\"text/html; charset=UTF-8\">");
	req->getResponse()->write("<body>");

	shared_ptr<OrganisationalStructure> struc;
	vector <shared_ptr<OrganisationalStructure> > structs = MascaretApplication::getInstance()->getAgentPlateform()->getOrganisationalStructure();
	bool found = false;
	int i = 0;
	while (!found && (unsigned)i < structs.size())
	{
		if (structs[i]->getName() == structName)
			found = true;
		else i++;
	}
	if (found) struc = structs[i];
	else cerr << "Erreur..." << endl;

	req->getResponse()->write("<H2>Description</H2>");
	req->getResponse()->write(struc->getName());

	req->getResponse()->write("<H2>Role</H2>");
	vector< shared_ptr<Role> > roles = struc->getRoles();
	req->getResponse()->write("<ul>");
	for (size_t i = 0; i < roles.size(); i++)
	{
		req->getResponse()->write("<li>");
		req->getResponse()->write(roles[i]->getName());
		req->getResponse()->write(" : <a href=\"RoleClass?alias=");
		req->getResponse()->write(struc->getName());
		req->getResponse()->write("&role=");
		req->getResponse()->write(roles[i]->getName());
		req->getResponse()->write("\" target = \"Body\">");
		req->getResponse()->write(roles[i]->getRoleClass()->getName());
		req->getResponse()->write("</a></li>");
	}
	req->getResponse()->write("</ul>");

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

	req->getResponse()->write("</body>");
	req->getResponse()->write("</html>");
}
extern "C" HttpServlet* createManageOrganisationalStructureServlet()
		{
	return new ManageOrganisationalStructureServlet();
		}
extern "C" void destroyManageOrganisationalStructureServlet(HttpServlet* s)
{
	delete s;
}
