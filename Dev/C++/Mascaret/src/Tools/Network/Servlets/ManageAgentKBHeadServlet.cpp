#include "Tools/Network/Servlets/ManageAgentKBHeadServlet.h"
#include "VEHA/Kernel/Package.h"
#include "VEHA/Kernel/Model.h"
#include "MascaretApplication.h"
#include "VEHA/Kernel/InstanceSpecification.h"
#include "VEHA/Kernel/Environment.h"
#include "VEHA/Entity/Entity.h"
#include "HAVE/VirtualHuman.h"

using namespace VEHA;
using namespace HAVE;

void ManageAgentKBHeadServlet::manageRequest(HttpRequest* req)
{

	string id = req->getParameter("alias");

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
                cerr <<"Can't find : "<<id<<endl;
                req->getResponse()->write("<html>");
                req->getResponse()->write("<body>");
                req->getResponse()->write("Can't find agent : " + id);
                req->getResponse()->write("</body>");
                req->getResponse()->write("</html>");
                return;
        }
        shared_ptr<VirtualHuman> human = shared_dynamic_cast<VirtualHuman>(entity);


	req->getResponse()->write("<html>");
	req->getResponse()->write("<link href=\"doxygen.css\" rel=\"stylesheet\" type=\"text/css\">");
	req->getResponse()->write("<META HTTP-EQUIV=\"Content-Type\" content=\"text/html; charset=UTF-8\">");
	req->getResponse()->write("<H1>");
	if (human)
		req->getResponse()->write(human->getName());
	req->getResponse()->write("</H1>");
	req->getResponse()->write("<H1>");
	if (human)
		req->getResponse()->write(human->getSummary());
	req->getResponse()->write("</H1>");
	req->getResponse()->write("<H2>");
	if (human)
		req->getResponse()->write(human->getDescription());
	req->getResponse()->write("</H2>");
	req->getResponse()->write("<img src=\"cerv.png\" align=\"center\">");
	req->getResponse()->write("</html>");
}
extern "C" HttpServlet* createManageAgentKBHeadServlet()
{
	return new ManageAgentKBHeadServlet();
}
extern "C" void destroyManageAgentKBHeadServlet(HttpServlet* s)
{
	delete s;
}
