#include "Tools/Network/Servlets/ManageSetAttributeValueServlet.h"
#include <boost/foreach.hpp>
#include "VEHA/Kernel/Environment.h"
#include "VEHA/Kernel/PrimitiveType.h"
#include "MascaretApplication.h"
#include "VEHA/Kernel/ValueSpecification.h"
#include "VEHA/Kernel/InstanceSpecification.h"

using std::cout;
using std::cerr;
using std::endl;

using VEHA::Environment;
using VEHA::InstanceSpecification;
using VEHA::Slot;

void ManageSetAttributeValueServlet::manageRequest(HttpRequest* req)
{

	cout<<"HTTP REQEST URL :"<<req->getRequestURL()<<endl;
    string id = req->getParameter("alias");

    shared_ptr<Environment> env = MascaretApplication::getInstance()->getEnvironment();
    shared_ptr<InstanceSpecification> entity;
    try
    {
        #if GCC_VERSION > 40100
        entity=env->getInstances().at(id);
        #else
        entity=MAP_AT(env->getInstances(),id);
        #endif
    }
    catch (std::out_of_range&)
    {
        cerr <<"Can't find : "<<id<<endl;
    }

    req->getResponse()->write("<html>");
    req->getResponse()->write("<body>");
//    req->getResponse()->write("<META HTTP-EQUIV=\"Refresh\" CONTENT=\"30\">");
    req->getResponse()->write("<META HTTP-EQUIV=\"Content-Type\" content=\"text/html; charset=UTF-8\">");
    req->getResponse()->write("<link href=\"doxygen.css\" rel=\"stylesheet\" type=\"text/css\">");
    req->getResponse()->write("<link href=\"tabs.css\" rel=\"stylesheet\" type=\"text/css\">");

    cerr << id << endl;

    map<std::string,std::string> parameters = req->getParameters();
    map<std::string,std::string>::iterator it;

    const map<string,shared_ptr<Slot> >& attributes = entity->getSlots();
    map<string,shared_ptr<Slot> >::const_iterator itAtr;

    for(it=parameters.begin();it!=parameters.end();it++)
    {
    	if(it->first!="alias")
    	{
    		string attribute = it->first;
			string value = parameters[attribute];

			itAtr = attributes.find(attribute);
			if(itAtr!=attributes.end())
			{
				shared_ptr<Slot> val = itAtr->second;
				val->addValueFromString(value);

				req->getResponse()->write("<h3>This attribute has been updated : </h3>");
				req->getResponse()->write(entity->getName());

				//break;

			}
			else if(it==parameters.end())
			{
				req->getResponse()->write("<h3>This attribute does not exist in ");
				req->getResponse()->write(entity->getName());
				req->getResponse()->write("</h3>");
			}
    	}
    }

    req->getResponse()->write("</body>");
    req->getResponse()->write("</html>");
}
extern "C" HttpServlet* createManageSetAttributeValueServlet()
{
	return new ManageSetAttributeValueServlet();
}
extern "C" void destroyManageSetAttributeValueServlet(HttpServlet* s)
{
	delete s;
}
