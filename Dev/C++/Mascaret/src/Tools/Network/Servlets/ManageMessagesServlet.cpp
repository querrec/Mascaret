#include "Tools/Network/Servlets/ManageMessagesServlet.h"
#include "VEHA/Kernel/Class.h"
#include "VEHA/Kernel/Environment.h"
#include "VEHA/Kernel/Model.h"
#include "MascaretApplication.h"
using namespace VEHA;

void ManageMessagesServlet::manageRequest(HttpRequest* req)
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

	//if (true)
	if (MascaretApplication::getInstance()->agent)
	{
		req->getResponse()->write("<HR>");
		req->getResponse()->write("<ul>");
		req->getResponse()->write("<P>");
		req->getResponse()->write(" <a href=\"messagesRecus\"");
		req->getResponse()->write(" target = \"Body\">");
		req->getResponse()->write("Messages Recus");
		req->getResponse()->write("</a>");
		req->getResponse()->write("</P>");
		req->getResponse()->write("<P>");
		req->getResponse()->write(" <a href=\"messagesEnvoyes\"");
		req->getResponse()->write(" target = \"Body\">");
		req->getResponse()->write("Messages Envoyes");
		req->getResponse()->write("</a>");
		req->getResponse()->write("</P>");
		req->getResponse()->write("<P>");
		req->getResponse()->write(" <a href=\"createUserMessage\"");
		req->getResponse()->write(" target = \"Body\">");
		req->getResponse()->write("Envoyer un message");
		req->getResponse()->write("</a>");
		req->getResponse()->write("</P>");
		req->getResponse()->write("</ul>");
	}
	req->getResponse()->write("</body>");
	req->getResponse()->write("</html>");
}
extern "C" HttpServlet* createManageMessagesServlet()
		{
	return new ManageMessagesServlet();
		}
extern "C" void destroyManageMessagesServlet(HttpServlet* s)
{
	delete s;
}
