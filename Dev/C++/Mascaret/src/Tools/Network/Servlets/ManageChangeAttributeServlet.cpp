#include "Tools/Network/Servlets/ManageChangeAttributeServlet.h"
#include <boost/foreach.hpp>
#include "VEHA/Kernel/Environment.h"
#include "MascaretApplication.h"
#include "VEHA/Kernel/InstanceSpecification.h"

using std::cerr;
using std::endl;
using std::pair;

using VEHA::InstanceSpecification;
using VEHA::Slot;
using VEHA::ValueSpecification;

void ManageChangeAttributeServlet::manageRequest(HttpRequest* req)
{
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
    req->getResponse()->write("<META HTTP-EQUIV=\"Refresh\" CONTENT=\"30\">");
    req->getResponse()->write("<META HTTP-EQUIV=\"Content-Type\" content=\"text/html; charset=UTF-8\">");
    req->getResponse()->write("<link href=\"doxygen.css\" rel=\"stylesheet\" type=\"text/css\">");
    req->getResponse()->write("<link href=\"tabs.css\" rel=\"stylesheet\" type=\"text/css\">");

    cerr << id << endl;

    const map<string,shared_ptr<Slot> >& attributes = entity->getSlots();
    map<string,shared_ptr<Slot> >::const_iterator it;
    for (it=attributes.begin();it!=attributes.end();it++)
    //   for (unsigned int i = 0; i < attributes.size(); i++)
    {
	//      Slot * attr = attributes[i];

	pair<string, shared_ptr<ValueSpecification> > pV;

        req->getResponse()->write("<FORM METHOD=GET action=\"setAttributeValue");
        req->getResponse()->write("\">");
        req->getResponse()->write("<TABLE width=\"900px\" BORDER=1>");
        map<string, shared_ptr<ValueSpecification> > currentSlotValues = it->second->getValues();
        map<string, shared_ptr<ValueSpecification> >::iterator itValue;

	for(itValue = currentSlotValues.begin();itValue!=currentSlotValues.end();itValue++)//BOOST_FOREACH(pV,it->second->getValues())
	{     
             string value;

             value="'"+itValue->first+"' ";
             req->getResponse()->write("<TR>");
             req->getResponse()->write("<TD width=\"200px\">");
             req->getResponse()->write(it->first);
             req->getResponse()->write("</TD>");
             req->getResponse()->write("<TD>");
             req->getResponse()->write("<INPUT NAME=alias TYPE=text VALUE=");
             req->getResponse()->write(id);
             req->getResponse()->write(">");
             req->getResponse()->write("</TD>");      
             req->getResponse()->write("<TD width=\"250px\">");

             if(itValue->second->CLASSTYPE=="Instance")
             {
            	 //find every instance of the same class
            	 map<string, shared_ptr<InstanceSpecification> > instances = env->getInstances();
            	 map<string, shared_ptr<InstanceSpecification> >::iterator itInstance;

            	 req->getResponse()->write("<select name=");
            	 req->getResponse()->write(it->first);
            	 req->getResponse()->write(" width=\"100%\" >");
            	 req->getResponse()->write("<option>");
            	 req->getResponse()->write(itValue->first);
            	 req->getResponse()->write("</option>");

            	 for(itInstance= instances.begin();itInstance!=instances.end();itInstance++)
            	 {
            		 if(itInstance->second->getClassifier()->getFullName() == itValue->second->getType()->getFullName())
            		 {
            			 if(itInstance->first!=itValue->first)
            			 {
            				 req->getResponse()->write("<option>");
            				 req->getResponse()->write(itInstance->first);
            				 req->getResponse()->write("</option>");
            			 }
		      		 }
            	 }

        		 req->getResponse()->write("</select>");
             }
             else
             {
				 req->getResponse()->write("<INPUT NAME=");
				 req->getResponse()->write(it->first);
				 req->getResponse()->write(" VALUE=");
				 req->getResponse()->write(value);
				 req->getResponse()->write(">");
             }

             req->getResponse()->write("</TD>");
             req->getResponse()->write("<TD>");
             req->getResponse()->write("<center><INPUT TYPE=\"submit\" VALUE=\"Modifier\"></center>");
             req->getResponse()->write("</TD>");
             req->getResponse()->write("</TR>");
	}

        req->getResponse()->write("</TABLE>");
	req->getResponse()->write("</FORM>");
    }

    


    req->getResponse()->write("</body>");
    req->getResponse()->write("</html>");
}
extern "C" HttpServlet* createManageChangeAttributeServlet()
{
	return new ManageChangeAttributeServlet();
}
extern "C" void destroyManageChangeAttributeServlet(HttpServlet* s)
{
	delete s;
}
