#include "Tools/Network/Servlets/ManageModelServlet.h"
#include "VEHA/Kernel/Model.h"
#include "VEHA/Kernel/Class.h"
#include "MascaretApplication.h"
#include "HAVE/VirtualHuman.h"

using namespace VEHA;
using namespace HAVE;

using BEHAVE::KnowledgeBase;

void ManageModelServlet::manageRequest(HttpRequest* req)
{

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


	req->getResponse()->write("<html>");
	req->getResponse()->write("<META HTTP-EQUIV=\"Content-Type\" content=\"text/html; charset=UTF-8\">");
	req->getResponse()->write("<body>");
	req->getResponse()->write("<link href=\"doxygen.css\" rel=\"stylesheet\" type=\"text/css\">");
	req->getResponse()->write("<link href=\"styles.css\" rel=\"stylesheet\" type=\"text/css\">");
	req->getResponse()->write("<link href=\"tabs.css\" rel=\"stylesheet\" type=\"text/css\">");

	req->getResponse()->write("<div class=\"tabs\">");
	req->getResponse()->write("<ul>");
	if (!human) 
		req->getResponse()->write("<li class=\"current\"><a href=\"Model\"><span>Model</span></a></li>");
	else
	{
		req->getResponse()->write("<li><a href=\"Model?agent=");
		req->getResponse()->write(human->getName());
		req->getResponse()->write("\"><span>Model</span></a></li>");
	}
	if (!human) 
		req->getResponse()->write("<li><a href=\"Instances\"><span>Entites</span></a></li>");
	else
	{
		req->getResponse()->write("<li><a href=\"Instances?agent=");
		req->getResponse()->write(human->getName());
		req->getResponse()->write("\"><span>Entites</span></a></li>");
	}
	req->getResponse()->write("<li><a href=\"Agents\"><span>Humains Virtuels</span></a></li>");
	req->getResponse()->write("<li><a href=\"Organisations\"><span>Organisations</span></a></li>");
	req->getResponse()->write("<li><a href=\"Scenario\"><span>Scenario</span></a></li>");
	req->getResponse()->write("<li><a href=\"Application\"><span>Application</span></a></li>");
	req->getResponse()->write("<li><a href=\"FIPASL\"><span>FIPA SL</span></a></li>");
	req->getResponse()->write("<li><a href=\"XOCL\"><span>XOCL</span></a></li>");
	req->getResponse()->write("<li><a href=\"messages\"><span>Mes messages</span></a></li>");

	req->getResponse()->write("</ul>");
	req->getResponse()->write("</div>");

	req->getResponse()->write("<HR>");

	req->getResponse()->write("<ul>");

	shared_ptr<Model> model;
	if (human)
	{
		shared_ptr<KnowledgeBase> kb = human->getKnowledgeBase();
		shared_ptr<Environment> env = kb->getEnvironment();
		if (env) 
			_printPackage(0, req, env->getModel()->getPackage(),human);
			
	}
	else
		_printPackage(0, req, MascaretApplication::getInstance()->getModel()->getPackage(), shared_ptr<VirtualHuman>());
	
	req->getResponse()->write("</ul>");

	req->getResponse()->write("</body>");
	req->getResponse()->write("</html>");
}
void ManageModelServlet::_printPackage(int level, HttpRequest * req, shared_ptr<Package> package, shared_ptr<VirtualHuman> human)
{
	if (!package) return;
	req->getResponse()->write("<div class=\"node\">");

	req->getResponse()->write("<img src = em_package.png>");
	req->getResponse()->write(" ");
	req->getResponse()->write(package->getName());
	level++;
	req->getResponse()->write("<ul>");
	vector<shared_ptr<Class> > classes = package->getClasses();
	for (unsigned int i = 0; i < classes.size(); i++)
	{
		req->getResponse()->write("<P><img src =EndL.png>");
		req->getResponse()->write("<img src =em_class.png>");
		req->getResponse()->write(" <a href=\"Class?alias=");
		req->getResponse()->write(classes[i]->getName());
		if (human)
		{
			req->getResponse()->write("&agent=");
			req->getResponse()->write(human->getName());
		}
		req->getResponse()->write("\" target = \"Body\">");
		req->getResponse()->write(classes[i]->getName());
		req->getResponse()->write("</a>");
		req->getResponse()->write("</P>");
	}
	vector<shared_ptr<Package> > packages = package->getPackages();
	for (unsigned int i = 0; i < packages.size(); i++)
		_printPackage(level, req, packages[i],human);
	req->getResponse()->write("</ul>");
	req->getResponse()->write("</div>");
}
extern "C" HttpServlet* createManageModelServlet()
		{
	return new ManageModelServlet();
		}
extern "C" void destroyManageModelServlet(HttpServlet* s)
{
	delete s;
}
