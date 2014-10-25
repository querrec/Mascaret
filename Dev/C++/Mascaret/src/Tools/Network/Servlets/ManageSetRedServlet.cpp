#include "Tools/Network/Servlets/ManageSetRedServlet.h"
#include "HAVE/VirtualHuman.h"
#include "HAVE/Body.h"
#include "VEHA/Behavior/BasicActions/SetInstanceInRedAction.h"
#include "VEHA/Behavior/BasicActions/SetInstanceInRedBehaviorExecution.h"
#include "VEHA/Behavior/Common/behaviorScheduler.h"
#include "VEHA/Kernel/Environment.h"
#include "VEHA/Entity/Entity.h"
#include "VEHA/Entity/ShapeSpecification.h"

using std::string;
using namespace HAVE;
using namespace VEHA;

void ManageSetRedServlet::manageRequest(HttpRequest* req)
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
                shared_ptr<SetInstanceInRedAction> action = shared_ptr<SetInstanceInRedAction>(new SetInstanceInRedAction());
                action->setTarget(entity2);

                BehaviorScheduler::getInstance()->executeBehavior(action,entity2,Parameters());
/*
		long gid = entity2->getActiveShape()->getGID();
		Interface3D::get()->setRed(gid);
		req->getResponse()->write("<html><body>done</body></html>");
*/
	}
	else
	{
		shared_ptr<VirtualHuman> vh=shared_dynamic_cast<VirtualHuman>(entity);
		if(vh)
		{
/*
			long gid=vh->getBody()->getActiveShape()->getGID();
			Interface3D::get()->setRed(gid);
			req->getResponse()->write("<html><body>done</body></html>");
*/
		}
	}
}
extern "C" HttpServlet* createManageSetRedServlet()
{
	return new ManageSetRedServlet();
}
extern "C" void destroyManageSetRedServlet(HttpServlet* s)
{
	delete s;
}
