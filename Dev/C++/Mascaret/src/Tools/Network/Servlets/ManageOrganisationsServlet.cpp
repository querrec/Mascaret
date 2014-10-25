#include "Tools/Network/Servlets/ManageOrganisationsServlet.h"
#include "BEHAVE/OrganisationalEntity.h"
#include "BEHAVE/OrganisationalStructure.h"
#include "MascaretApplication.h"
#include "BEHAVE/Agent.h"
#include "BEHAVE/AgentPlateform.h"

using namespace VEHA;

using BEHAVE::OrganisationalEntity;
using BEHAVE::OrganisationalStructure;

void ManageOrganisationsServlet::manageRequest(HttpRequest* req)
{
	req->getResponse()->write("<html>");
	req->getResponse()->write("<META HTTP-EQUIV=\"Content-Type\" content=\"text/html; charset=UTF-8\">");
	req->getResponse()->write("<body>");
	req->getResponse()->write("<link href=\"doxygen.css\" rel=\"stylesheet\" type=\"text/css\">");
	req->getResponse()->write("<link href=\"tabs.css\" rel=\"stylesheet\" type=\"text/css\">");
	req->getResponse()->write("<div class=\"tabs\">");
	req->getResponse()->write("<ul>");
	req->getResponse()->write("<li><a href=\"Model\"><span>Model</span></a></li>");
	req->getResponse()->write("<li class=\"current\"><a href=\"Instances\"><span>Entites</span></a></li>");
	req->getResponse()->write("<li><a href=\"Agents\"><span>Humains Virtuels</span></a></li>");
	req->getResponse()->write("<li><a href=\"Organisations\"><span>Organisations</span></a></li>");
	req->getResponse()->write("<li><a href=\"Scenario\"><span>Scenario</span></a></li>");
	req->getResponse()->write("<li><a href=\"Application\"><span>Application</span></a></li>");
    req->getResponse()->write("<li><a href=\"FIPASL\"><span>FIPA SL</span></a></li>");
	req->getResponse()->write("<li><a href=\"XOCL\"><span>XOCL</span></a></li>");
	req->getResponse()->write("<li><a href=\"messages\"><span>Mes messages</span></a></li>");

	req->getResponse()->write("</ul>");
	req->getResponse()->write("</div>");
	req->getResponse()->write("<HR>");

	vector< shared_ptr<OrganisationalStructure> > structs = MascaretApplication::getInstance()->getAgentPlateform()->getOrganisationalStructure();
	vector< shared_ptr<OrganisationalEntity> > orgs = MascaretApplication::getInstance()->getAgentPlateform()->getOrganisationalEntity();

	req->getResponse()->write("<H2>Structures</H2>");
	req->getResponse()->write("<ul>");
	for (unsigned int i = 0; i < structs.size(); i++)
	{
		req->getResponse()->write("<li><a href=\"OrgStruct?alias=");
		req->getResponse()->write(structs[i]->getName());
		req->getResponse()->write("\" target = \"Body\">");
		req->getResponse()->write(structs[i]->getName());
		req->getResponse()->write("</a></li>");
	}
	req->getResponse()->write("</ul>");

	req->getResponse()->write("<H2>Entites</H2>");
	req->getResponse()->write("<ul>");
	for (unsigned int i = 0; i < orgs.size(); i++)
	{
		req->getResponse()->write("<li><a href=\"OrgEntity?alias=");
		req->getResponse()->write(orgs[i]->getName());
		req->getResponse()->write("\" target = \"Body\">");
		req->getResponse()->write(orgs[i]->getName());
		req->getResponse()->write("</a></li>");
	}
	req->getResponse()->write("</ul>");

	req->getResponse()->write("</body>");
	req->getResponse()->write("</html>");
}

extern "C" HttpServlet* createManageOrganisationsServlet()
{
	return new ManageOrganisationsServlet();
}
extern "C" void destroyManageOrganisationsServlet(HttpServlet* s)
{
	delete s;
}
