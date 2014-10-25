#include "Tools/Network/Servlets/ManageClassServlet.h"
#include "VEHA/Kernel/Model.h"
#include "VEHA/Kernel/Property.h"
#include "VEHA/Kernel/Operation.h"
#include "VEHA/Kernel/Parameter.h"
#include "HAVE/VirtualHuman.h"
#include "MascaretApplication.h"

using namespace VEHA;
using namespace HAVE;

void ManageClassServlet::manageRequest(HttpRequest* req)
{
	shared_ptr<Class> classe;

	string id = req->getParameter("agent");

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
        }
        shared_ptr<VirtualHuman> human;
        if (entity) human = shared_dynamic_cast<VirtualHuman>(entity);

	if (entity) 
	{
		shared_ptr<Environment> env = human->getKnowledgeBase()->getEnvironment();
		if (env) classe = _findClasse(env->getModel()->getPackage(),req->getParameter("alias"));
	}
	else
		classe = _findClasse(MascaretApplication::getInstance()->getModel()->getPackage(),req->getParameter("alias"));

	req->getResponse()->write("<html>");
	req->getResponse()->write("<META HTTP-EQUIV=\"Content-Type\" content=\"text/html; charset=UTF-8\">");
	req->getResponse()->write("<body>");
	req->getResponse()->write("<link href=\"doxygen.css\" rel=\"stylesheet\" type=\"text/css\">");
	req->getResponse()->write("<link href=\"tabs.css\" rel=\"stylesheet\" type=\"text/css\">");

	req->getResponse()->write("<H2>Description</H2>");
	req->getResponse()->write("<ul>");
	req->getResponse()->write("<li>");
	req->getResponse()->write(classe->getName());
	req->getResponse()->write("</li>");
	req->getResponse()->write("<li>");
	req->getResponse()->write(classe->getFullName());
	req->getResponse()->write("</li>");
	req->getResponse()->write("<li>");
	req->getResponse()->write(classe->getSummary());
	req->getResponse()->write("</li>");
	req->getResponse()->write("<li>");
	req->getResponse()->write(classe->getDescription());
	req->getResponse()->write("</li>");
	req->getResponse()->write("</ul>");

	req->getResponse()->write("<HR>");
	req->getResponse()->write("<H2>Hierarchie</H2>");

	//   vector<Generalization * > generalizations = classe->getGeneralization();

	//   writeString(request,"<H3>Specialisation</H3>");
	//
	//   writeString(request,"<ul>");
	//   for (unsigned int iC = 0; iC < generalizations.size(); iC++)
	//   {
	//      if (generalizations[iC]->getGeneral()->getId() == classe->getId())
	//      {
	//         shared_ptr<Class> children = _findClasse(world->getModel()->getPackages_at(0),generalizations[iC]->getSpecific()->getId());
	//         writeString(request,"<li><a href=\"Class?alias=");
	//         writeString(request,children->getId());
	//         writeString(request,"\" target = \"Body\">");
	//         writeString(request,children->getName());
	//         writeString(request,"</a></li>");
	//      }
	//   }
	//   writeString(request,"</ul>");

	req->getResponse()->write("<H3>Parent</H3>");
	req->getResponse()->write("<ul>");
	for (unsigned int iC = 0; iC < classe->getParents().size(); iC++)
	{

		//      if (generalizations[iC]->getSpecific()->getId() == classe->getId())
		//      {
		//= _findClasse(world->getModel()->getPackages_at(0),generalizations[iC]->getGeneral()->getId());
		shared_ptr<Classifier> parent=classe->getParents()[iC];
		req->getResponse()->write("<li><a href=\"Class?alias=");
		req->getResponse()->write(parent->getName());
		req->getResponse()->write("\" target = \"Body\">");
		req->getResponse()->write(parent->getName());
		req->getResponse()->write("</a></li>");
		//      }
	}
	req->getResponse()->write("</ul>");

	req->getResponse()->write("</ul>");
	req->getResponse()->write("<HR>");
	req->getResponse()->write("<H2>Attributs</H2>");
	req->getResponse()->write("<ul>");
	const map <string,shared_ptr<Property> >& attributes = classe->getAttributes();
	map <string,shared_ptr<Property> >::const_iterator itA;
	for(itA=attributes.begin();itA!=attributes.end();itA++)
	{
		req->getResponse()->write("<li>");
		req->getResponse()->write(itA->first);
		req->getResponse()->write(" : ");
		req->getResponse()->write(itA->second->getType()->getName());
		req->getResponse()->write("</li>");
	}
	req->getResponse()->write("</ul>");

	req->getResponse()->write("<HR>");
	/*
	      req->getResponse()->write("<H2>Associations</H2>");
	      req->getResponse()->write("<ul>");

	      vector<Property* > assos = classe->getAssociations();
	      for (unsigned int iAss = 0; iAss < assos.size() ; iAss ++)
	      {
	         Property * attr = assos[iAss];
	         req->getResponse()->write("<li>");
	         req->getResponse()->write(attr->getName());
	         req->getResponse()->write(" : <a href=\"Class?alias=");
	         req->getResponse()->write(attr->getType()->getId());
	         req->getResponse()->write("\" target = \"Body\">");
	         req->getResponse()->write(attr->getType()->getName());
	         req->getResponse()->write("</a>");
	      }
	      req->getResponse()->write("</ul>");
	 */

	req->getResponse()->write("<HR>");
	req->getResponse()->write("<H2>Operations</H2>");
	const map<string, shared_ptr<Operation> >& operations = classe->getOperations();
	map<string, shared_ptr<Operation> >::const_iterator itO;
	for (itO=operations.begin();itO!=operations.end();itO++)
	{
		//     Operation * oper = operations[iOper];
		req->getResponse()->write("<li>");
		req->getResponse()->write(itO->first);
		req->getResponse()->write("( ");
		const vector <shared_ptr<Parameter> >& paramsT = itO->second->getParameter();

		for (unsigned int iP = 0; iP < paramsT.size(); iP++)
		{
			shared_ptr<Parameter> attr = paramsT[iP];
			req->getResponse()->write(attr->getName());
			req->getResponse()->write(" : <a href=\"Class?alias=");
			req->getResponse()->write(attr->getType()->getName());
			req->getResponse()->write("\" target = \"Body\">");
			req->getResponse()->write(attr->getType()->getName());
			req->getResponse()->write("</a>");
			if (iP !=  paramsT.size()-1)
				req->getResponse()->write(", ");
		}
		req->getResponse()->write(" )");
		req->getResponse()->write(" : ");
		if (itO->second->getType() )
			req->getResponse()->write(itO->second->getType()->getName());
		else
			req->getResponse()->write("undefined");
		req->getResponse()->write("</li>");
	}

	req->getResponse()->write("<HR>");
	req->getResponse()->write("<H2>Instances</H2>");
	map<string,shared_ptr<InstanceSpecification> > instances = classe->getInstances();
	map<string,shared_ptr<InstanceSpecification> >::const_iterator itI;
	for(itI=instances.begin();itI!=instances.end();itI++)
		//  for (unsigned int iI = 0; iI < instances.size(); iI++)
	{
		req->getResponse()->write("<li>");
		req->getResponse()->write("<a href=\"Entity?alias=");
		req->getResponse()->write(itI->first);
		req->getResponse()->write("\" target = \"Body\">");
		req->getResponse()->write(itI->first);
		req->getResponse()->write("</a>");

	}

	/*
	      connection->writeString("<HR>");
	      connection->writeString("<H2>Etats</H2>");

	 */

	req->getResponse()->write("<HR>");
	req->getResponse()->write("<H2>Creer une nouvelle Entite</H2>");
	req->getResponse()->write("<FORM METHOD=GET action=\"CreateEntity?class=");
	req->getResponse()->write(classe->getName());
	req->getResponse()->write("\">");
	req->getResponse()->write("Nom : ");
	req->getResponse()->write("<INPUT NAME=name>");
	req->getResponse()->write("<INPUT NAME=class VALUE=");
	req->getResponse()->write(classe->getName());
	req->getResponse()->write(">");

	req->getResponse()->write("<INPUT TYPE=\"submit\" VALUE=\"Creer\">");
	req->getResponse()->write("</FORM>");

	req->getResponse()->write("<H2>Mettre a jour</H2>");
	req->getResponse()->write("<li><a href=\"ReloadClass?alias=");
	req->getResponse()->write(classe->getName());
	req->getResponse()->write("\" target = \"Body\">");
	req->getResponse()->write("Relire la classe du fichier modele");
	req->getResponse()->write("</a></li>");

	req->getResponse()->write("</body>");
	req->getResponse()->write("</html>");
}


shared_ptr<Class> ManageClassServlet::_findClasse(shared_ptr<Package> package, const string & id)
{
	const vector<shared_ptr<Class> >& classes = package->getClasses();
	unsigned int i = 0;
	bool found = false;

	while (!found && i < classes.size())
	{
		if (classes[i]->getName() == id) found = true;
		i++;
	}
	if (found) return classes[i-1];

	const vector<shared_ptr<Package> > packages = package->getPackages();
	i = 0;
	shared_ptr<Class> c ;
	while (!c &&  i< packages.size())
	{
		c = _findClasse(packages[i], id);
		i++;
	}
	return c;
}
extern "C" HttpServlet* createManageClassServlet()
		{
	return new ManageClassServlet();
		}
extern "C" void destroyManageClassServlet(HttpServlet* s)
{
	delete s;
}
