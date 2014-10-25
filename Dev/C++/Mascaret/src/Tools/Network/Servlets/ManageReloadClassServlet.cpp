#include "Tools/Network/Servlets/ManageReloadClassServlet.h"
#include "Tools/Loaders/ModelLoader2.h"
#include "VEHA/Kernel/Model.h"
#include "VEHA/Kernel/Property.h"
#include "VEHA/Kernel/Operation.h"
#include "VEHA/Kernel/Parameter.h"
#include "MascaretApplication.h"

using namespace VEHA;

void ManageReloadClassServlet::manageRequest(HttpRequest* req)
{
	shared_ptr<Model> model = MascaretApplication::getInstance()->getModel();
	shared_ptr<ModelLoader2> modelLoader = model->getModelLoader();
cerr << modelLoader << endl;
	shared_ptr<Class> classe;
        string className = req->getParameter("alias");
	classe = _findClasse(model->getPackage(),req->getParameter("alias"));
 
        //cerr << " URL : " << world->getModel()->getUrl()<< endl;

        modelLoader->reload(classe);

	req->getResponse()->write("<html>");
	req->getResponse()->write("<META HTTP-EQUIV=\"Content-Type\" content=\"text/html; charset=UTF-8\">");
	req->getResponse()->write("<body>");
	req->getResponse()->write("<link href=\"doxygen.css\" rel=\"stylesheet\" type=\"text/css\">");
	req->getResponse()->write("<link href=\"tabs.css\" rel=\"stylesheet\" type=\"text/css\">");

        req->getResponse()->write("<H2>");
        req->getResponse()->write(className);
        req->getResponse()->write(" rechargée");
        req->getResponse()->write("</H2>");

	req->getResponse()->write("</body>");
	req->getResponse()->write("</html>");
}

shared_ptr<Class> ManageReloadClassServlet::_findClasse(shared_ptr<Package> package, const string & id)
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

extern "C" HttpServlet* createManageReloadClassServlet()
		{
	return new ManageReloadClassServlet();
		}
extern "C" void destroyManageReloadClassServlet(HttpServlet* s)
{
	delete s;
}
