#include "Tools/Network/Servlets/ManageCreateEntityServlet.h"
#include "VEHA/Kernel/Class.h"
#include "VEHA/Kernel/Environment.h"
#include "VEHA/Kernel/Model.h"
#include "VEHA/Entity/Entity.h"
#include "VEHA/Entity/EntityClass.h"
#include "MascaretApplication.h"

using namespace VEHA;

void ManageCreateEntityServlet::manageRequest(HttpRequest* req)
{
	shared_ptr<Class> classe;
	string name;
	classe = _findClasse(MascaretApplication::getInstance()->getModel()->getPackage(),req->getParameter("class"));
	name = req->getParameter("name");
	cerr << req->getParameter("class") << endl;
	shared_ptr<Entity> entity = shared_ptr<Entity>(new Entity(name,shared_dynamic_cast<EntityClass>(classe)));
	shared_ptr<Environment> env = MascaretApplication::getInstance()->getEnvironment();
	env->addInstance(entity);

	req->getResponse()->write("<html>");
	req->getResponse()->write("<META HTTP-EQUIV=\"Content-Type\" content=\"text/html; charset=UTF-8\">");
	req->getResponse()->write("<body>");
	req->getResponse()->write("<link href=\"doxygen.css\" rel=\"stylesheet\" type=\"text/css\">");
	req->getResponse()->write("<link href=\"tabs.css\" rel=\"stylesheet\" type=\"text/css\">");
	req->getResponse()->write("<H2>Creation de l'entite</H2>");
	req->getResponse()->write("<a href=\"Entity?alias=");
	req->getResponse()->write(entity->getName());
	req->getResponse()->write("\" target = \"Body\">");
	req->getResponse()->write(entity->getName());
	req->getResponse()->write("</a>");
	req->getResponse()->write("</body>");
	req->getResponse()->write("</html>");

}
shared_ptr<Class> ManageCreateEntityServlet::_findClasse(shared_ptr<Package> package, const string & id)
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
extern "C" HttpServlet* createManageCreateEntityServlet()
		{
	return new ManageCreateEntityServlet();
		}
extern "C" void destroyManageCreateEntityServlet(HttpServlet* s)
{
	delete s;
}
