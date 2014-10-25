#include "Tools/Network/Servlets/ManageXOCLServlet.h"


void ManageXOCLServlet::manageRequest(HttpRequest* req)
{
	req->getResponse()->write("<html>");
	req->getResponse()->write("<META HTTP-EQUIV=\"Content-Type\" content=\"text/html; charset=UTF-8\">");
	req->getResponse()->write("<body>");
	req->getResponse()->write("<link href=\"doxygen.css\" rel=\"stylesheet\" type=\"text/css\">");
	req->getResponse()->write("<link href=\"styles.css\" rel=\"stylesheet\" type=\"text/css\">");
	req->getResponse()->write("<link href=\"tabs.css\" rel=\"stylesheet\" type=\"text/css\">");
	req->getResponse()->write("<div class=\"tabs\">");
	req->getResponse()->write("<ul>");
	req->getResponse()->write("<li class=\"current\"><a href=\"Model\"><span>Model</span></a></li>");
	req->getResponse()->write("<li><a href=\"Instances\"><span>Entites</span></a></li>");
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
	req->getResponse()->write("</ul>");
	req->getResponse()->write("<HR>");
	req->getResponse()->write("<H2>Query</H2>");
	req->getResponse()->write("<ul>");
	req->getResponse()->write("<li><a href=\"Query\" target=\"Body\">Execute a query</a></li>");
	req->getResponse()->write("</ul>");
	req->getResponse()->write("</body>");
	req->getResponse()->write("</html>");
}
extern "C" HttpServlet* createManageXOCLServlet()
		{
	return new ManageXOCLServlet();
		}
extern "C" void destroyManageXOCLServlet(HttpServlet* s)
{
	delete s;
}
