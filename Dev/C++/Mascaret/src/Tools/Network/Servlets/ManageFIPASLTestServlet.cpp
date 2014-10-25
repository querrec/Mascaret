#include "Tools/Network/Servlets/ManageFIPASLTestServlet.h"
#include "VEHA/Kernel/Class.h"
#include "VEHA/Kernel/Environment.h"
#include "BEHAVE/ACLMessage.h"
#include "BEHAVE/AID.h"
#include "BEHAVE/fipaSLParser.h"
#include "Tools/Loaders/XmlParser.h"

using namespace VEHA;
using namespace BEHAVE;
using std::string;
void ManageFIPASLTestServlet::manageRequest(HttpRequest* req)
{
	req->getResponse()->write("<html>");
	req->getResponse()->write("<META HTTP-EQUIV=\"Content-Type\" content=\"text/html; charset=UTF-8\">");
	req->getResponse()->write("<body>");
	req->getResponse()->write("<link href=\"doxygen.css\" rel=\"stylesheet\" type=\"text/css\">");
	req->getResponse()->write("<link href=\"tabs.css\" rel=\"stylesheet\" type=\"text/css\">");
	req->getResponse()->write("<H2>Entrez une expression:</H2>");
	req->getResponse()->write("<form method=\"get\" action=\"fipaSLTest\">" );
	req->getResponse()->write("Expression : ");
	req->getResponse()->write("<br/>");
	req->getResponse()->write("<input type=\"text\" name=\"expression\" size=\"60\" value=\"");
	if(req->hasParameter("expression"))
		req->getResponse()->write(req->getParameter("expression"));
	req->getResponse()->write("\"/>");
	req->getResponse()->write("<br/>");
	req->getResponse()->write("<input type=\"submit\" value=\"Executer\" />");
	req->getResponse()->write("</form>");
	if(req->hasParameter("expression"))
	{
		req->getResponse()->write("<br/>");
		string expression=req->getParameter("expression");
		FIPASLParserResult& r=parseFipaSLExpression(expression);
		req->getResponse()->write("success:");
		req->getResponse()->write(r.success);
		req->getResponse()->write("<br/>");
		if(r.success)
		{
			req->getResponse()->write("isAction: ");
			req->getResponse()->write(r.isAction);
			req->getResponse()->write("<br/>");
			req->getResponse()->write("isDone: ");
			req->getResponse()->write(r.isDone);
			req->getResponse()->write("<br/>");
			req->getResponse()->write("action: ");
			req->getResponse()->write(r.action.actionName);
			req->getResponse()->write("<br/>");
			req->getResponse()->write("performer: ");
			req->getResponse()->write(r.action.performer);
			req->getResponse()->write("<br/>");
			for(size_t i=0;i<r.action.paramValue.size();i++)
			{
				req->getResponse()->write("param");
				req->getResponse()->write((int)i);
				req->getResponse()->write(": ");
				req->getResponse()->write(r.action.paramValue[i]);
				req->getResponse()->write("<br/>");
			}
			for(size_t i=0;i<r.action.paramName.size();i++)
			{
				req->getResponse()->write("paramName");
				req->getResponse()->write((int)i);
				req->getResponse()->write(": ");
				req->getResponse()->write(r.action.paramName[i]);
				req->getResponse()->write("<br/>");

			}

		}
	}
}
extern "C" HttpServlet* createManageFIPASLTestServlet()
{
	return new ManageFIPASLTestServlet();
}
extern "C" void destroyManageFIPASLTestServlet(HttpServlet* s)
{
	delete s;
}
