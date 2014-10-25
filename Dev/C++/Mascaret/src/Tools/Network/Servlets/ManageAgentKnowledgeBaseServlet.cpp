#include "Tools/Network/Servlets/ManageAgentKnowledgeBaseServlet.h"
#include "MascaretApplication.h"
#include "VEHA/Kernel/InstanceSpecification.h"
#include "VEHA/Kernel/Environment.h"
#include "HAVE/VirtualHuman.h"


using namespace VEHA;
using namespace HAVE;


void ManageAgentKnowledgeBaseServlet::manageRequest(HttpRequest* req)
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
	req->getResponse()->write("<head>");
	req->getResponse()->write("<META HTTP-EQUIV=\"Content-Type\" content=\"text/html; charset=UTF-8\">");
	req->getResponse()->write("<title>");
	req->getResponse()->write(human->getName());
	req->getResponse()->write("</title>");
	req->getResponse()->write("</head>");
	req->getResponse()->write("<link href=\"doxygen.css\" rel=\"stylesheet\" type=\"text/css\">");
	req->getResponse()->write("<frameset rows=\"175,*\">");
	req->getResponse()->write("<frame src=\"HeadAgentKB?alias=");
	req->getResponse()->write(human->getName());
	req->getResponse()->write("\" name=\"Head\">");
	req->getResponse()->write("<frameset cols=\"250,*\">");
	req->getResponse()->write("<frame src=\"Model?agent=");
	req->getResponse()->write(human->getName());
	req->getResponse()->write("\" name=\"Model\">");
	req->getResponse()->write("<frame src=\"Body\" name=\"Body\">");
	req->getResponse()->write("</frameset>");
	req->getResponse()->write("</frameset>");
	req->getResponse()->write("</html>");
}
extern "C" HttpServlet* createManageAgentKnowledgeBaseServlet()
{
	return new ManageAgentKnowledgeBaseServlet();
}
extern "C" void destroyManageAgentKnowledgeBaseervlet(HttpServlet* s)
{
	delete s;
}
