#include "Tools/Network/Servlets/ManageMessagesEnvoyesServlet.h"
#include "MascaretApplication.h"
#include "BEHAVE/Agent.h"
#include "BEHAVE/AgentPlateform.h"

using BEHAVE::ACLMessage;
using BEHAVE::AID;

void ManageMessagesEnvoyesServlet::manageRequest(HttpRequest* req)
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
		req->getResponse()->write("<H3>Messages envoyes</H3>");
		vector<shared_ptr<ACLMessage> > e = MascaretApplication::getInstance()->agent->getSent();
		req->getResponse()->write("<TABLE BORDER=1>");
		req->getResponse()->write("<TR>");
		req->getResponse()->write("<TD>A </TD>");
		req->getResponse()->write("<TD>Performative </TD>");
		req->getResponse()->write("<TD>Contenu </TD>");
		req->getResponse()->write("</TR>");
		for (unsigned int i = 0; i < e.size(); i++)
		{
			shared_ptr<ACLMessage> msg = e[i];
			req->getResponse()->write("<TR>");
			req->getResponse()->write("<TD>");
			vector< shared_ptr<AID> > receivers = msg->getReceivers();
			for (unsigned int i = 0; i < receivers.size() ; i++)
			{
				req->getResponse()->write(receivers[i]->toString());
			}
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
extern "C" HttpServlet* createManageMessagesEnvoyesServlet()
		{
	return new ManageMessagesEnvoyesServlet();
		}
extern "C" void destroyManageMessagesEnvoyesServlet(HttpServlet* s)
{
	delete s;
}
