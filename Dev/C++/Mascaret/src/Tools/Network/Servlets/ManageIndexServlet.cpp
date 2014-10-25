#include "Tools/Network/Servlets/ManageIndexServlet.h"


void ManageIndexServlet::manageRequest(HttpRequest* req)
{
	req->getResponse()->write("<html>");
	req->getResponse()->write("<head>");
	req->getResponse()->write("<META HTTP-EQUIV=\"Content-Type\" content=\"text/html; charset=UTF-8\">");
	req->getResponse()->write("<title>");
	req->getResponse()->write("MASCARET : Multi Agents System for Collaborative Adaptive Environment for Traning");
	req->getResponse()->write("</title>");
	req->getResponse()->write("</head>");
	req->getResponse()->write("<link href=\"doxygen.css\" rel=\"stylesheet\" type=\"text/css\">");
	req->getResponse()->write("<frameset rows=\"175,*\">");
	req->getResponse()->write("<frame src=\"Head\" name=\"Head\">");
	req->getResponse()->write("<frameset cols=\"250,*\">");
	req->getResponse()->write("<frame src=\"Model\" name=\"Model\">");
	req->getResponse()->write("<frame src=\"Body\" name=\"Body\">");
	req->getResponse()->write("</frameset>");
	req->getResponse()->write("</frameset>");
	req->getResponse()->write("</html>");
}
extern "C" HttpServlet* createManageIndexServlet()
{
	return new ManageIndexServlet();
}
extern "C" void destroyManageIndexServlet(HttpServlet* s)
{
	delete s;
}
