#include "Tools/Network/Servlets/ManageMessagesRecusServlet.h"
#include "MascaretApplication.h"
#include "BEHAVE/Agent.h"
#include "BEHAVE/AgentPlateform.h"

using BEHAVE::ACLMessage;

void ManageMessagesRecusServlet::manageRequest(HttpRequest* req)
{
	req->getResponse()->write("<html>");
	req->getResponse()->write("<META HTTP-EQUIV=\"Content-Type\" content=\"text/html; charset=UTF-8\">");
	req->getResponse()->write("<body>");
	req->getResponse()->write("<link href=\"doxygen.css\" rel=\"stylesheet\" type=\"text/css\">");
	req->getResponse()->write("<link href=\"styles.css\" rel=\"stylesheet\" type=\"text/css\">");
	req->getResponse()->write("<link href=\"tabs.css\" rel=\"stylesheet\" type=\"text/css\">");

	if (MascaretApplication::getInstance()->agent)
	//if(false)
	{
		while (MascaretApplication::getInstance()->agent->receive()){;}

		req->getResponse()->write("<H3>Messages recus</H3>");
		vector<shared_ptr<ACLMessage> > l = MascaretApplication::getInstance()->agent->getChecked();
		req->getResponse()->write("<TABLE BORDER=1>");
		req->getResponse()->write("<TR>");
		req->getResponse()->write("<TD>De </TD>");
		req->getResponse()->write("<TD>Performative </TD>");
		req->getResponse()->write("<TD>Contenu </TD>");
		req->getResponse()->write("</TR>");
		for (unsigned int i = 0; i < l.size(); i++)
		{
			shared_ptr<ACLMessage> msg = l[i];
			req->getResponse()->write("<TR>");
			req->getResponse()->write("<TD>");
			req->getResponse()->write(msg->getSender()->toString());
			req->getResponse()->write("</TD>");
			req->getResponse()->write("<TD>");
			req->getResponse()->write(msg->getPerformativeText());
			req->getResponse()->write("</TD>");
			req->getResponse()->write("<TD>");
			req->getResponse()->write(msg->getContent());
			req->getResponse()->write("</TD>");
			req->getResponse()->write("</TR>");
		}
		req->getResponse()->write("</TABLE>");
	}

	req->getResponse()->write("</body>");
	req->getResponse()->write("</html>");
}
extern "C" HttpServlet* createManageMessagesRecusServlet()
{
	return new ManageMessagesRecusServlet();
}
extern "C" void destroyManageMessagesRecusServlet(HttpServlet* s)
{
	delete s;
}
