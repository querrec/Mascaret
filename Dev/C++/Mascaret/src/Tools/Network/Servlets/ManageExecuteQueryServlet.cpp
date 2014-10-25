#include "Tools/Network/Servlets/ManageExecuteQueryServlet.h"
#include "VEHA/Kernel/Class.h"
#include "VEHA/Kernel/Environment.h"
#include "VEHA/Kernel/InstanceSpecification.h"
#include "VEHA/Kernel/expressionParser.h"
#include "MascaretApplication.h"

using namespace VEHA;
using namespace VEHA::OclParser;

void ManageExecuteQueryServlet::manageRequest(HttpRequest* req)
{
	string self, query;
	self = req->getParameter("self");
	query = req->getParameter("query");

	req->getResponse()->write("<html>");
	req->getResponse()->write("<META HTTP-EQUIV=\"Content-Type\" content=\"text/html; charset=UTF-8\">");
	req->getResponse()->write("<body>");
	req->getResponse()->write("<link href=\"doxygen.css\" rel=\"stylesheet\" type=\"text/css\">");
	req->getResponse()->write("<link href=\"tabs.css\" rel=\"stylesheet\" type=\"text/css\">");
	req->getResponse()->write("<H2>Entered Query</H2>");
	req->getResponse()->write("Self : " + self + "<br>");
	req->getResponse()->write( query);
	req->getResponse()->write("<H2>Query execution result</H2>");

	shared_ptr<Environment> env = MascaretApplication::getInstance()->getEnvironment();
	const map<string,shared_ptr<InstanceSpecification> >& ents = env->getInstances();

	if(self == "*")
	{
		Context ctx;

		map<string,shared_ptr<InstanceSpecification> >::const_iterator itEntities=ents.begin();
		while(itEntities != ents.end())
		{
			ctx[itEntities->first]=itEntities->second;
			//ctx["self"]=itEntities->second; // pour qu'il y est un self
			itEntities++;
		}
		ExpressionTree et= parseExpression(query) ;

		req->getResponse()->write(string(evalExpression(et,ctx)));
	}
	else
	{
		map<string,shared_ptr<InstanceSpecification> >::const_iterator itSelf=ents.find(self);
		if(itSelf != ents.end())
		{
			shared_ptr<InstanceSpecification> self_instance = itSelf->second;
			Context ctx;
			ctx["self"]=self_instance;

			ExpressionTree et= parseExpression(query) ;

			req->getResponse()->write(string(evalExpression(et,ctx)));
		}
		else
		{
			req->getResponse()->write("Instance "+self+" non trouvé");
		}
	}

	/*req->getResponse()->write("<br> <br><H2>Some help</H2>");
	Interface3D::get()->initHelp();
	string constraint_name = Interface3D::get()->getConstraintProperty("name");

	map<string,string> m = Interface3D::get()->_mapSpecialHelp[constraint_name];
	map<string,string>::iterator it = m.begin();
	for (size_t i=0; i< m.size(); i++) {
		req->getResponse()->write(" <a href=\"XOCLHelper?help=");
		req->getResponse()->write( boost::lexical_cast<string>(i));
		req->getResponse()->write("\" target = \"Body\">");
		req->getResponse()->write( it->first);
		req->getResponse()->write("</a><br>");
		it++;
	}*/

	req->getResponse()->write("</html>");
}
extern "C" HttpServlet* createManageExecuteQueryServlet()
		{
	return new ManageExecuteQueryServlet();
		}
extern "C" void destroyManageExecuteQueryServlet(HttpServlet* s)
{
	delete s;
}
