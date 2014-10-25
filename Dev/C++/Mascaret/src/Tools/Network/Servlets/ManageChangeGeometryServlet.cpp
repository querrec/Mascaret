//#include "MascaretApplication.h"
#include "Tools/Network/Servlets/ManageChangeGeometryServlet.h"
#include "MascaretApplication.h"
#include "VEHA/Kernel/Environment.h"
#include "VEHA/Kernel/InstanceSpecification.h"
#include "VEHA/Entity/Entity.h"
#include "VEHA/Entity/Vector3.h"

using namespace VEHA;

string _ftoa(double val);

void ManageChangeGeometryServlet::manageRequest(HttpRequest* req)
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
    }

    req->getResponse()->write("<html>");
    req->getResponse()->write("<body>");
    req->getResponse()->write("<META HTTP-EQUIV=\"Refresh\" CONTENT=\"30\">");
    req->getResponse()->write("<META HTTP-EQUIV=\"Content-Type\" content=\"text/html; charset=UTF-8\">");
    req->getResponse()->write("<link href=\"doxygen.css\" rel=\"stylesheet\" type=\"text/css\">");
    req->getResponse()->write("<link href=\"tabs.css\" rel=\"stylesheet\" type=\"text/css\">");

    Vector3 position, orientation ;
    position = (shared_dynamic_cast<Entity>(entity))->getLocalPosition();
    orientation = (shared_dynamic_cast<Entity>(entity))->getLocalOrientation();

    cerr << id << endl;

    req->getResponse()->write("<FORM METHOD=GET action=\"changePosition");
    req->getResponse()->write("\">");
    req->getResponse()->write("<TABLE BORDER=1>");
    req->getResponse()->write("<TR>");
    req->getResponse()->write("<TD>Position</TD>");
    req->getResponse()->write("<TD>");
    req->getResponse()->write("<INPUT NAME=alias TYPE=text VALUE=");
    req->getResponse()->write(id);
    req->getResponse()->write(">");
    req->getResponse()->write("</TD>");
    req->getResponse()->write("<TD>");
    req->getResponse()->write("<INPUT NAME=x VALUE=");
    req->getResponse()->write(_ftoa(position.x));
    req->getResponse()->write(">");
    req->getResponse()->write("</TD>");
    req->getResponse()->write("<TD>");
    req->getResponse()->write("<INPUT NAME=y VALUE=");
    req->getResponse()->write(_ftoa(position.y));
    req->getResponse()->write(">");
    req->getResponse()->write("</TD>");
    req->getResponse()->write("<TD>");
    req->getResponse()->write("<INPUT NAME=z VALUE=");
    req->getResponse()->write(_ftoa(position.z));
    req->getResponse()->write(">");
    req->getResponse()->write("</TD>");
    req->getResponse()->write("<TD>");
    req->getResponse()->write("<INPUT TYPE=\"submit\" VALUE=\"Modifier\">");
    req->getResponse()->write("</TD>");
    req->getResponse()->write("</TR>");
    req->getResponse()->write("</TABLE>");
    req->getResponse()->write("</FORM>");
    req->getResponse()->write("<FORM METHOD=GET action=\"changeOrientation");
    req->getResponse()->write("\">");
    req->getResponse()->write("<TABLE BORDER=1>");
    req->getResponse()->write("<TR>");
    req->getResponse()->write("<TD>Orientation</TD>");
    req->getResponse()->write("<TD>");
    req->getResponse()->write("<INPUT NAME=alias TYPE=text VALUE=");
    req->getResponse()->write(id);
    req->getResponse()->write(">");
    req->getResponse()->write("</TD>");
    req->getResponse()->write("<TD>");
    req->getResponse()->write("<INPUT NAME=x VALUE=");
    req->getResponse()->write(_ftoa(orientation.x));
    req->getResponse()->write(">");
    req->getResponse()->write("</TD>");
    req->getResponse()->write("<TD>");
    req->getResponse()->write("<INPUT NAME=y VALUE=");
    req->getResponse()->write(_ftoa(orientation.y));
    req->getResponse()->write(">");
    req->getResponse()->write("</TD>");
    req->getResponse()->write("<TD>");
    req->getResponse()->write("<INPUT NAME=z VALUE=");
    req->getResponse()->write(_ftoa(orientation.z));
    req->getResponse()->write(">");
    req->getResponse()->write("</TD>");
    req->getResponse()->write("<TD>");
    req->getResponse()->write("<INPUT TYPE=\"submit\" VALUE=\"Modifier\">");
    req->getResponse()->write("</TD>");
    req->getResponse()->write("</TR>");
    req->getResponse()->write("</TABLE>");
    req->getResponse()->write("</FORM>");
    req->getResponse()->write("<FORM METHOD=GET action=\"changeShape");
    req->getResponse()->write("\">");
    req->getResponse()->write("<INPUT NAME=alias TYPE=text VALUE=");
    req->getResponse()->write(id);
    req->getResponse()->write(">");
    req->getResponse()->write("Geometrie, URL : ");
    req->getResponse()->write("<INPUT TYPE=file NAME=url/>");
    req->getResponse()->write("<INPUT TYPE=\"submit\" VALUE=\"Modifier\">");
    req->getResponse()->write("</FORM>");
    req->getResponse()->write("</body>");
    req->getResponse()->write("</html>");
}
extern "C" HttpServlet* createManageChangeGeometryServlet()
{
	return new ManageChangeGeometryServlet();
}
extern "C" void destroyManageChangeGeometryServlet(HttpServlet* s)
{
	delete s;
}
