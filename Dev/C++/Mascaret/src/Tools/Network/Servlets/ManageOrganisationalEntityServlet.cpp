#include "Tools/Network/Servlets/ManageOrganisationalEntityServlet.h"
#include "MascaretApplication.h"
#include "BEHAVE/OrganisationalEntity.h"
#include "BEHAVE/RoleAssignement.h"
#include "BEHAVE/Role.h"
#include "BEHAVE/Procedure.h"
#include "BEHAVE/RoleClass.h"
#include "BEHAVE/OrganisationalStructure.h"
#include "BEHAVE/Agent.h"
#include "BEHAVE/AgentPlateform.h"
using std::cerr;
using std::endl;
using std::string;

using BEHAVE::OrganisationalEntity;
using BEHAVE::OrganisationalStructure;
using BEHAVE::Procedure;
using BEHAVE::Role;
using BEHAVE::RoleAssignement;

void ManageOrganisationalEntityServlet::manageRequest(HttpRequest* req)
{
	string orgName = req->getParameter("alias");
	cerr << "ManageOrganisationalEntity (" << orgName << endl;
	req->getResponse()->write("<html>");
	req->getResponse()->write("<META HTTP-EQUIV=\"Content-Type\" content=\"text/html; charset=UTF-8\">");
	req->getResponse()->write("<body>");

	shared_ptr<OrganisationalEntity> org;
	vector <shared_ptr<OrganisationalEntity> > orgs = MascaretApplication::getInstance()->getAgentPlateform()->getOrganisationalEntity();
	bool found = false;
	int i = 0;
	while (!found && (unsigned)i < orgs.size())
	{
		if (orgs[i]->getName() == orgName)
			found = true;
		else i++;
	}
	if (found) org = orgs[i];
	else cerr << "Erreur..." << endl;

	req->getResponse()->write("<H2>Description</H2>");
	req->getResponse()->write(org->getName());

	req->getResponse()->write("<H2>Affectations</H2>");
	vector< shared_ptr<RoleAssignement> > assignements = org->getRoleAssignements();
	req->getResponse()->write("<ul>");
	for (size_t i = 0; i < assignements.size(); i++)
	{
		string agentAid = assignements[i]->getAgent()->toString();
		shared_ptr<Role> role = assignements[i]->getRole();
		shared_ptr<OrganisationalStructure> struc = org->getStructure();
		req->getResponse()->write("<li>");
		req->getResponse()->write(agentAid);
		req->getResponse()->write(" / ");
		req->getResponse()->write(role->getName());
		req->getResponse()->write(" : <a href=\"RoleClass?alias=");
		req->getResponse()->write(struc->getName());
		req->getResponse()->write("&role=");
		req->getResponse()->write(role->getName());
		req->getResponse()->write("\" target = \"Body\">");
		req->getResponse()->write(role->getRoleClass()->getName());
		req->getResponse()->write("</a></li>");
	}
	req->getResponse()->write("</ul>");

	req->getResponse()->write("<H2>Procedures</H2>");
	shared_ptr<OrganisationalStructure> struc = org->getStructure();
	vector < shared_ptr<Procedure> > procs = struc->getProcedures();
	req->getResponse()->write("<ul>");
	for (size_t i = 0; i < procs.size(); i++)
	{
		req->getResponse()->write("<li>");
		req->getResponse()->write("<a href=\"Procedure?alias=");
		req->getResponse()->write(procs[i]->getName());
		req->getResponse()->write("&org=");
		req->getResponse()->write(org->getName());
		req->getResponse()->write("\" target = \"Body\">");
		req->getResponse()->write(procs[i]->getName());
		req->getResponse()->write("</a></li>");
	}
	req->getResponse()->write("</ul>");

	req->getResponse()->write("<H2>Structure</H2>");
	req->getResponse()->write("<a href=\"OrgStruct?alias=");
	req->getResponse()->write(struc->getName());
	req->getResponse()->write("\" target = \"Body\">");
	req->getResponse()->write(struc->getName());
	req->getResponse()->write("</a>");

	req->getResponse()->write("</body>");
	req->getResponse()->write("</html>");
}
extern "C" HttpServlet* createManageOrganisationalEntityServlet()
		{
	return new ManageOrganisationalEntityServlet();
		}
extern "C" void destroyManageOrganisationalEntityServlet(HttpServlet* s)
{
	delete s;
}
