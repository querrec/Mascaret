#include "Tools/Network/Servlets/ManageHeadServlet.h"
#include "VEHA/Kernel/Package.h"
#include "VEHA/Kernel/Model.h"
#include "MascaretApplication.h"
using namespace VEHA;

void ManageHeadServlet::manageRequest(HttpRequest* req)
{
	shared_ptr<Package> package = MascaretApplication::getInstance()->getModel()->getPackage();

	req->getResponse()->write("<html>");
	req->getResponse()->write("<link href=\"doxygen.css\" rel=\"stylesheet\" type=\"text/css\">");
	req->getResponse()->write("<META HTTP-EQUIV=\"Content-Type\" content=\"text/html; charset=UTF-8\">");
	req->getResponse()->write("<H1>");
	if (package)
		req->getResponse()->write(package->getName());
	req->getResponse()->write("</H1>");
	req->getResponse()->write("<H1>");
	if (package)
		req->getResponse()->write(package->getSummary());
	req->getResponse()->write("</H1>");
	req->getResponse()->write("<H2>");
	if (package)
		req->getResponse()->write(package->getDescription());
	req->getResponse()->write("</H2>");
	req->getResponse()->write("<img src=\"cerv.png\" align=\"center\">");
	req->getResponse()->write("</html>");
}
extern "C" HttpServlet* createManageHeadServlet()
{
	return new ManageHeadServlet();
}
extern "C" void destroyManageHeadServlet(HttpServlet* s)
{
	delete s;
}
