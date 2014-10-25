#include "Tools/Network/Servlets/ManageInstancesServlet.h"
#include "VEHA/Kernel/Environment.h"
#include "VEHA/Kernel/InstanceSpecification.h"
#include "MascaretApplication.h"

#include "HAVE/VirtualHuman.h"

using namespace VEHA;
using namespace HAVE;


void ManageInstancesServlet::manageRequest(HttpRequest* req)
{

       string id = req->getParameter("agent");

        shared_ptr<Environment> env = MascaretApplication::getInstance()->getEnvironment();
        shared_ptr<InstanceSpecification> entity;
        try
        {
#if GCC_VERSION > 40100
                entity=env->getInstances().at(id);
#else
                entity=MAP_AT(env->getInstances(),id);
#endif
        }
        catch (std::out_of_range&)
        {
        }
        shared_ptr<VirtualHuman> human;
        if (entity) human = shared_dynamic_cast<VirtualHuman>(entity);
	if (human) env = human->getKnowledgeBase()->getEnvironment();


	req->getResponse()->write("<html>");
	req->getResponse()->write("<META HTTP-EQUIV=\"Content-Type\" content=\"text/html; charset=UTF-8\">");
	req->getResponse()->write("<body>");
	req->getResponse()->write("<link href=\"doxygen.css\" rel=\"stylesheet\" type=\"text/css\">");
	req->getResponse()->write("<link href=\"tabs.css\" rel=\"stylesheet\" type=\"text/css\">");
	req->getResponse()->write("<div class=\"tabs\">");
	req->getResponse()->write("<ul>");
	if (!human)
                req->getResponse()->write("<li class=\"current\"><a href=\"Model\"><span>Model</span></a></li>");
        else
        {
                req->getResponse()->write("<li><a href=\"Model?agent=");
                req->getResponse()->write(human->getName());
                req->getResponse()->write("\"><span>Model</span></a></li>");
        }
        if (!human)
                req->getResponse()->write("<li class=\"current\"><a href=\"Instances\"><span>Entites</span></a></li>");
        else
        {
                req->getResponse()->write("<li class=\"current\"><a href=\"Instances?agent=");
                req->getResponse()->write(human->getName());
                req->getResponse()->write("\"><span>Entites</span></a></li>");
        }

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

	/*
	    req->getResponse()->write("<SCRIPT language=\"Javascript\">");
	    req->getResponse()->write("alert(\"Voici un message d alerte!\");");
	    req->getResponse()->write("</SCRIPT>");
	 */

	req->getResponse()->write("<HR>");
	req->getResponse()->write("<H2>Entites</H2>");

	if (env)
	{
		const map<string,shared_ptr<InstanceSpecification> >& ents = env->getInstances();
		map<string,shared_ptr<InstanceSpecification> >::const_iterator it;
		req->getResponse()->write("<ul>");

		//   for (unsigned int iEnt = 0; iEnt<ents.size(); iEnt++)
		for (it=ents.begin();it!=ents.end();it++)
		{
			req->getResponse()->write("<li><a href=\"Entity?alias=");
			req->getResponse()->write(it->first);
			if (human) 
			{
				req->getResponse()->write("&agent=");
				req->getResponse()->write(human->getName());
			}
			req->getResponse()->write("\" target = \"Body\">");
			req->getResponse()->write(it->first);
			req->getResponse()->write("</a></li>");
		}
		req->getResponse()->write("</ul>");
	}

	req->getResponse()->write("</body>");
	req->getResponse()->write("</html>");

}
extern "C" HttpServlet* createManageInstancesServlet()
		{
	return new ManageInstancesServlet();
		}
extern "C" void destroyManageInstancesServlet(HttpServlet* s)
{
	delete s;
}
