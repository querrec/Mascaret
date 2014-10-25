#include "Tools/Network/Servlets/ManageCreateUserMessageServlet.h"
#include "VEHA/Kernel/Class.h"
#include "VEHA/Kernel/Environment.h"
#include "VEHA/Kernel/Model.h"
#include "BEHAVE/AID.h"
#include "BEHAVE/Agent.h"
#include "VEHA/Entity/Entity.h"
#include "VEHA/Entity/EntityClass.h"
#include "MascaretApplication.h"

using namespace VEHA;
using namespace BEHAVE;
void ManageCreateUserMessageServlet::manageRequest(HttpRequest* req)
{
	shared_ptr<Environment> env = MascaretApplication::getInstance()->getEnvironment();
	shared_ptr<AID> agentAID=MascaretApplication::getInstance()->agent->getAID();
	req->getResponse()->write("<html>");
	req->getResponse()->write("<body>");
	req->getResponse()->write("<META HTTP-EQUIV=\"Content-Type\" content=\"text/html; charset=UTF-8\">");
	req->getResponse()->write("<link href=\"doxygen.css\" rel=\"stylesheet\" type=\"text/css\">");
	req->getResponse()->write("<link href=\"tabs.css\" rel=\"stylesheet\" type=\"text/css\">");
	req->getResponse()->write("<FORM METHOD=GET action=\"sendUserMessage");
	req->getResponse()->write("\">");
	req->getResponse()->write("<TABLE BORDER=1>");
	req->getResponse()->write("<TR>");
	req->getResponse()->write("<TD>De</TD>");
	req->getResponse()->write("<TD>Nom Dest.</TD>");
	req->getResponse()->write("<TD>Plateforme Dest.</TD>");
	req->getResponse()->write("<TD>Port Dest.</TD>");
	req->getResponse()->write("<TD>Performative</TD>");
	req->getResponse()->write("<TD>Contenu</TD>");
	req->getResponse()->write("</TR>");
	req->getResponse()->write("<TR>");
	req->getResponse()->write("<TD>");
	req->getResponse()->write(agentAID->toString());
	req->getResponse()->write("</TD>");
	req->getResponse()->write("<TD>");
	req->getResponse()->write("<INPUT NAME=destName TYPE=text VALUE=");
	req->getResponse()->write(">");
	req->getResponse()->write("</TD>");
	req->getResponse()->write("<TD>");
	req->getResponse()->write("<INPUT NAME=destPlateform TYPE=text VALUE=");
	req->getResponse()->write(">");
	req->getResponse()->write("</TD>");
	req->getResponse()->write("<TD>");
	req->getResponse()->write("<INPUT NAME=destPort TYPE=text VALUE=");
	req->getResponse()->write(">");
	req->getResponse()->write("</TD>");
	req->getResponse()->write("<TD>");
	req->getResponse()->write("<SELECT NAME=perf>");
	req->getResponse()->write("<OPTION VALUE=ACCEPT_PROPOSAL>ACCEPT_PROPOSAL");
	req->getResponse()->write("</OPTION>");
	req->getResponse()->write("<OPTION VALUE=AGREE>AGREE");
	req->getResponse()->write("</OPTION>");
	req->getResponse()->write("<OPTION VALUE=CANCEL>CANCEL");
	req->getResponse()->write("</OPTION>");
	req->getResponse()->write("<OPTION VALUE=CFP>CFP");
	req->getResponse()->write("</OPTION>");
	req->getResponse()->write("<OPTION VALUE=CONFIRM>CONFIRM");
	req->getResponse()->write("</OPTION>");
	req->getResponse()->write("<OPTION VALUE=DISCONFIRM>DISCONFIRM");
	req->getResponse()->write("</OPTION>");
	req->getResponse()->write("<OPTION VALUE=FAILURE>FAILURE");
	req->getResponse()->write("</OPTION>");
	req->getResponse()->write("<OPTION VALUE=FAILURE>FAILURE");
	req->getResponse()->write("</OPTION>");
	req->getResponse()->write("<OPTION VALUE=INFORM>INFORM");
	req->getResponse()->write("</OPTION>");
	req->getResponse()->write("<OPTION VALUE=INFORM_IF>INFORM_IF");
	req->getResponse()->write("</OPTION>");
	req->getResponse()->write("<OPTION VALUE=INFORM_REF>INFORM_REF");
	req->getResponse()->write("</OPTION>");
	req->getResponse()->write("<OPTION VALUE=NOT_UNDERSTOOD>NOT_UNDERSTOOD");
	req->getResponse()->write("</OPTION>");
	req->getResponse()->write("<OPTION VALUE=PROPAGATE>PROPAGATE");
	req->getResponse()->write("</OPTION>");
	req->getResponse()->write("<OPTION VALUE=PROPOSE>PROPOSE");
	req->getResponse()->write("</OPTION>");
	req->getResponse()->write("<OPTION VALUE=PROXY>PROXY");
	req->getResponse()->write("</OPTION>");
	req->getResponse()->write("<OPTION VALUE=QUERY_IF>QUERY_IF");
	req->getResponse()->write("</OPTION>");
	req->getResponse()->write("<OPTION VALUE=QUERY_REF>QUERY_REF");
	req->getResponse()->write("</OPTION>");
	req->getResponse()->write("<OPTION VALUE=REFUSE>REFUSE");
	req->getResponse()->write("</OPTION>");
	req->getResponse()->write("<OPTION VALUE=REJECT_PROPOSAL>REJECT_PROPOSAL");
	req->getResponse()->write("</OPTION>");
	req->getResponse()->write("<OPTION VALUE=REQUEST>REQUEST");
	req->getResponse()->write("</OPTION>");
	req->getResponse()->write("<OPTION VALUE=REQUEST_WHEN>REQUEST_WHEN");
	req->getResponse()->write("</OPTION>");
	req->getResponse()->write("<OPTION VALUE=REQUEST_WHENEVER>REQUEST_WHENEVER");
	req->getResponse()->write("</OPTION>");
	req->getResponse()->write("<OPTION VALUE=SUBSCRIBE>SUBSCRIBE");
	req->getResponse()->write("</OPTION>");
	req->getResponse()->write("<OPTION VALUE=UNKNOWN>UNKNOWN");
	req->getResponse()->write("</OPTION>");
	req->getResponse()->write("</SELECT>");
	req->getResponse()->write("</TD>");
	req->getResponse()->write("<TD>");
	req->getResponse()->write("<INPUT NAME=contenu");
	req->getResponse()->write(">");
	req->getResponse()->write("</TD>");
	req->getResponse()->write("</TR>");
	req->getResponse()->write("</TABLE>");
	req->getResponse()->write("<INPUT TYPE=\"submit\" VALUE=\"Envoyer\">");
	req->getResponse()->write("</FORM>");
	req->getResponse()->write("</body>");
	req->getResponse()->write("</html>");
}
extern "C" HttpServlet* createManageCreateUserMessageServlet()
{
	return new ManageCreateUserMessageServlet();
}
extern "C" void destroyManageCreateUserMessageServlet(HttpServlet* s)
{
	delete s;
}
