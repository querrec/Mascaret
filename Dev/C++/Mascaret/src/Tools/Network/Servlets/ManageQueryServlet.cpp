#include "Tools/Network/Servlets/ManageQueryServlet.h"


void ManageQueryServlet::manageRequest(HttpRequest* req)
{
	req->getResponse()->write("<html>");
	req->getResponse()->write("<META HTTP-EQUIV=\"Content-Type\" content=\"text/html; charset=UTF-8\">");
	req->getResponse()->write("<body>");
	req->getResponse()->write("<link href=\"doxygen.css\" rel=\"stylesheet\" type=\"text/css\">");
	req->getResponse()->write("<link href=\"tabs.css\" rel=\"stylesheet\" type=\"text/css\">");
	req->getResponse()->write("<H2>Enter a query</H2>");
	req->getResponse()->write("<FORM METHOD=GET action=\"ExecuteQuery");
	req->getResponse()->write("\">");
	req->getResponse()->write("Context (self) or *");
	req->getResponse()->write("<br>");
	req->getResponse()->write("<INPUT NAME=self");
	req->getResponse()->write(">");
	req->getResponse()->write("<br>");
	req->getResponse()->write("Expression : ");
	req->getResponse()->write("<br>");
	//req->getResponse()->write("<INPUT NAME=query");
	//req->getResponse()->write(">");
	//<TEXTAREA Name="comments" rows="4" cols="20"></TEXTAREA>
	req->getResponse()->write("<TEXTAREA  NAME=query rows=\"20\" cols=\"60\" ");
	req->getResponse()->write("></TEXTAREA>");
	req->getResponse()->write("<br>");
	req->getResponse()->write("<INPUT TYPE=\"submit\" VALUE=\"Submit\">");
	req->getResponse()->write("</FORM>");

	req->getResponse()->write("</body>");
	req->getResponse()->write("</html>");
}
extern "C" HttpServlet* createManageQueryServlet()
		{
	return new ManageQueryServlet();
		}
extern "C" void destroyManageQueryServlet(HttpServlet* s)
{
	delete s;
}
