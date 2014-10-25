#include "Tools/Network/Servlets/ManageCenterViewServlet.h"
#include "HAVE/VirtualHuman.h"
#include "HAVE/Body.h"
#include "VEHA/Kernel/Environment.h"
#include "VEHA/Entity/Entity.h"
#include "VEHA/Entity/ShapeSpecification.h"
#include "MascaretApplication.h"

using std::string;
using namespace HAVE;

void ManageCenterViewServlet::manageRequest(HttpRequest* req)
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
	shared_ptr<Entity> entity2=shared_dynamic_cast<Entity>(entity);
	if(entity2)
	{
		Vector3 pos=entity2->getLocalPosition();
		//Interface3D::get()->setCameraPosition(pos.x-10,pos.y,pos.z+1);
		//Interface3D::get()->setCameraOrientation(0,0,0);
		req->getResponse()->write("<html><body>done</body></html>");
	}
	else
	{
		shared_ptr<VirtualHuman> vh=shared_dynamic_cast<VirtualHuman>(entity);
		if(vh)
		{
			Vector3 pos=vh->getBody()->getLocalPosition();
			//Interface3D::get()->setCameraPosition(pos.x-5,pos.y,pos.z+1);
			//Interface3D::get()->setCameraOrientation(0,0,0);
			req->getResponse()->write("<html><body>done</body></html>");
		}
	}
}
extern "C" HttpServlet* createManageCenterViewServlet()
{
	return new ManageCenterViewServlet();
}
extern "C" void destroyManageCenterViewServlet(HttpServlet* s)
{
	delete s;
}
