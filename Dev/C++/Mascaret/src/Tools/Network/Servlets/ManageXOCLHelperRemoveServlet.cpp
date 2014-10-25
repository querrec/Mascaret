#include "Tools/Network/Servlets/ManageXOCLHelperRemoveServlet.h"

#include <iostream>
#include <map>
using namespace std;


void ManageXOCLHelperRemoveServlet::manageRequest(HttpRequest* request)
{
    request->getResponse()->write("<html>");
    request->getResponse()->write("<META HTTP-EQUIV=\"Content-Type\" content=\"text/html; charset=UTF-8\">");
    request->getResponse()->write("<body>");
    request->getResponse()->write("<link href=\"doxygen.css\" rel=\"stylesheet\" type=\"text/css\">");
    request->getResponse()->write("<link href=\"tabs.css\" rel=\"stylesheet\" type=\"text/css\">");

    	string id_help = request->getParameter("id");
	Interface3D::get()->removeHelp(boost::lexical_cast<int>(id_help));
  	request->getResponse()->write( "<h2>It is removed!</h2>");	
    request->getResponse()->write("</html>");
}
extern "C" HttpServlet* createManageXOCLHelperRemoveServlet()
		{
	return new ManageXOCLHelperRemoveServlet();
		}
extern "C" void destroyManageXOCLHelperRemoveServlet(HttpServlet* s)
{
	delete s;
}
