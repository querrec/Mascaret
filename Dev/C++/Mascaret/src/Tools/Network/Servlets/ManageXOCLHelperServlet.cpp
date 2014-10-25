#include "Tools/Network/Servlets/ManageXOCLHelperServlet.h"

#include <iostream>
#include <map>
using namespace std;


void ManageXOCLHelperServlet::manageRequest(HttpRequest* request)
{
    request->getResponse()->write("<html>");
    request->getResponse()->write("<META HTTP-EQUIV=\"Content-Type\" content=\"text/html; charset=UTF-8\">");
    request->getResponse()->write("<body>");
    request->getResponse()->write("<link href=\"doxygen.css\" rel=\"stylesheet\" type=\"text/css\">");
    request->getResponse()->write("<link href=\"tabs.css\" rel=\"stylesheet\" type=\"text/css\">");

    	string help = request->getParameter("help");
	string constraint_name = Interface3D::get()->getConstraintProperty("name");
	map<string, string > m = Interface3D::get()->_mapSpecialHelp[constraint_name];
	map<string, string >::const_iterator it = m.begin();
	long help_id=-1;
	for (unsigned i=0; i< m.size(); i++) {
	  if (boost::lexical_cast<string>(i) == help) {
    		help_id = Interface3D::get()->showHelp(it->second);
		break;
	  }
	  it++;
	}
    request->getResponse()->write("<H2>It is showed!</H2>");    

    request->getResponse()->write(" <a href=\"XOCLHelperRemove?id=");
    request->getResponse()->write( boost::lexical_cast<string>(help_id));
    request->getResponse()->write("\" target = \"Body\">");
    request->getResponse()->write( "Remove it!");
    request->getResponse()->write("</a><br>");

    request->getResponse()->write("</html>");
}
extern "C" HttpServlet* createManageXOCLHelperServlet()
		{
	return new ManageXOCLHelperServlet();
		}
extern "C" void destroyManageXOCLHelperServlet(HttpServlet* s)
{
	delete s;
}
