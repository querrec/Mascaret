#include "Tools/Network/Servlets/ManageOperationServlet.h"
#include "VEHA/Kernel/InstanceSpecification.h"
#include "VEHA/Kernel/Environment.h"
#include "VEHA/Kernel/Operation.h"
#include "VEHA/Kernel/Parameter.h"
#include "MascaretApplication.h"

using std::vector;

using VEHA::Classifier;
using VEHA::InstanceSpecification;
using VEHA::Class;
using VEHA::Operation;
using VEHA::Parameter;

bool _isBaseType(shared_ptr<Classifier> type)
{
    if (type->getName() == "integer") return true;
    if (type->getName() == "real") return true;
    if (type->getName() == "string") return true;
    else return false;
}

void ManageOperationServlet::manageRequest(HttpRequest* req)
{
	string id = req->getParameter("alias");
	string oper = req->getParameter("oper");

	shared_ptr<Environment> env = MascaretApplication::getInstance()->getEnvironment();
#if GCC_VERSION > 40100
	shared_ptr<InstanceSpecification> entity=env->getInstances().at(id);
#else
	shared_ptr<InstanceSpecification> entity=MAP_AT(env->getInstances(),id);
#endif
	const map<string, shared_ptr< InstanceSpecification> >& ents = env->getInstances();
	//   InstanceSpecification * entity = (InstanceSpecification*)NULL;
	//   for (unsigned int i = 0; i < ents.size(); i++)
	//   {
	//      if (ents[i]->getId() == id)
	//      {
	//         entity = ents[i];
	//         break;
	//      }
	//   }

	req->getResponse()->write("<html>");
	req->getResponse()->write("<body>");
	req->getResponse()->write("<META HTTP-EQUIV=\"Refresh\" CONTENT=\"30\">");
	req->getResponse()->write("<META HTTP-EQUIV=\"Content-Type\" content=\"text/html; charset=UTF-8\">");
	req->getResponse()->write("<link href=\"doxygen.css\" rel=\"stylesheet\" type=\"text/css\">");

	shared_ptr<Class> classifier = entity->getClassifier();
#if GCC_VERSION > 40100
	shared_ptr<Operation> operation=classifier->getOperations().at(oper);
#else
	shared_ptr<Operation> operation=MAP_AT(classifier->getOperations(),oper);
#endif
	/*
	       vector<Operation *> operations = classifier->getOperations();
	       Operation * operation = (Operation *) NULL;
	       for (unsigned int i = 0; i < operations.size(); i++)
	       {
	          if (operations[i]->getId() == oper)
	          {
	             operation = operations[i];
	             break;
	          }
	       }
	 */
	req->getResponse()->write("<H2>Description</H2>");
	req->getResponse()->write("<ul>");
	req->getResponse()->write("<li>");
	req->getResponse()->write(operation->getName());
	req->getResponse()->write("</li>");
	req->getResponse()->write("<li>");
	req->getResponse()->write(operation->getDescription());
	req->getResponse()->write("</li>");
	req->getResponse()->write("</ul>");

	vector<shared_ptr<Parameter> > parameters = operation->getParameter();

	req->getResponse()->write("<HR>");
	req->getResponse()->write("<H2>Parametres</H2>");
	req->getResponse()->write("<FORM METHOD=POST action=\"launchOperation\">");
	req->getResponse()->write("<input type=\"hidden\" name=\"alias\" value=\"");
	req->getResponse()->write(id);
	req->getResponse()->write("\"/>");
	req->getResponse()->write("<input type=\"hidden\" name=\"oper\" value=\"");
	req->getResponse()->write(oper);
	req->getResponse()->write("\"/>");
	req->getResponse()->write("<ul>");
	for (unsigned int i = 0; i < parameters.size(); i++)
	{
		shared_ptr<Parameter> param = parameters[i];
		req->getResponse()->write("<li>");
		shared_ptr<Classifier> type = param->getType();
		req->getResponse()->write(type->getName());
		req->getResponse()->write("\t");
		req->getResponse()->write(param->getName());
		if (_isBaseType(type))
		{
			req->getResponse()->write(" = ");
			req->getResponse()->write(" <INPUT name=_");
			req->getResponse()->write(param->getName());
			req->getResponse()->write(">");
			req->getResponse()->write("</li>");
		}
		else
		{
			req->getResponse()->write(" <select name=\"_");
			req->getResponse()->write(param->getName());
			req->getResponse()->write("\">");

			map<string, shared_ptr<InstanceSpecification> >::const_iterator it;
			for (it=ents.begin();it!=ents.end();it++)
				//         for (unsigned int iEnt = 0; iEnt < ents.size(); iEnt ++)
			{
				if (it->second->getClassifier())
				{
					if (it->second->getClassifier()->isA(type))
					{
						req->getResponse()->write(" = <option value=\"");
						req->getResponse()->write(it->first);
						req->getResponse()->write("\">");
						req->getResponse()->write(it->first);
						req->getResponse()->write("</option>");
					}
				}
			}
			req->getResponse()->write("</select>");
		}
	}
	req->getResponse()->write("</ul>");
	req->getResponse()->write("<INPUT TYPE=\"submit\" VALUE=\"Lancer\">");
	req->getResponse()->write("</FORM>");

	req->getResponse()->write("</body>");
	req->getResponse()->write("</html>");
}
extern "C" HttpServlet* createManageOperationServlet()
		{
	return new ManageOperationServlet();
		}
extern "C" void destroyManageOperationServlet(HttpServlet* s)
{
	delete s;
}
