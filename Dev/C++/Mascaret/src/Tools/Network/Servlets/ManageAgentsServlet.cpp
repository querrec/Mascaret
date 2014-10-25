#include "Tools/Network/Servlets/ManageAgentsServlet.h"
#include "MascaretApplication.h"
#include "VEHA/Kernel/Environment.h"
#include "VEHA/Kernel/InstanceSpecification.h"

using VEHA::InstanceSpecification;

void ManageAgentsServlet::manageRequest(HttpRequest* req)
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

	req->getResponse()->write("<HR>");
	req->getResponse()->write("<H2>Humains Virtuels</H2>");
	shared_ptr<Environment> env = MascaretApplication::getInstance()->getEnvironment();
	const map<string,shared_ptr<InstanceSpecification> >& ents = env->getInstances();
	map<string,shared_ptr<InstanceSpecification> >::const_iterator it;
	req->getResponse()->write("<ul>");
	for (it=ents.begin();it!=ents.end();it++)
	{
		string type (typeid(*(it->second)).name());
		if (type.find("VirtualHuman")!=string::npos)
		{
			req->getResponse()->write("<li><a href=\"Agent?alias=");
			req->getResponse()->write(it->first);
			req->getResponse()->write("\" target = \"Body\">");
			req->getResponse()->write(it->first);
			req->getResponse()->write("</a></li>");
		}
	}
	req->getResponse()->write("</ul>");

	req->getResponse()->write("</body>");
	req->getResponse()->write("</html>");
}
extern "C" HttpServlet* createManageAgentsServlet()
		{
	return new ManageAgentsServlet();
		}
extern "C" void destroyManageAgentsServlet(HttpServlet* s)
{
	delete s;
}
