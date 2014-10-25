#include <boost/foreach.hpp>
#include "Tools/Network/Servlets/ManageAttributeServlet.h"
#include "VEHA/Kernel/InstanceSpecification.h"
#include "VEHA/Kernel/Environment.h"
#include "VEHA/Entity/Entity.h"
#include "VEHA/Entity/Vector3.h"
#include "VEHA/Behavior/Common/Behavior.h"
#include "VEHA/Behavior/Common/Event.h"
#include "VEHA/Behavior/Common/Signal.h"
#include "VEHA/Behavior/Common/Trigger.h"
#include "VEHA/Behavior/StateMachine/SignalEvent.h"
#include "VEHA/Behavior/StateMachine/StateMachine.h"
#include "VEHA/Behavior/StateMachine/Transition.h"
#include "VEHA/Behavior/StateMachine/Region.h"
#include "MascaretApplication.h"

using namespace VEHA;

void ManageAttributeServlet::manageRequest(HttpRequest* req)
{
	string id = req->getParameter("alias");
	string attribute = req->getParameter("attr");
	shared_ptr<Environment> env = MascaretApplication::getInstance()->getEnvironment();
	shared_ptr<Entity> entity;
	try
	{
#if GCC_VERSION > 40100
	  entity=shared_dynamic_cast<Entity>(env->getInstances().at(id));
#else
	  entity=shared_dynamic_cast<Entity>(MAP_AT(env->getInstances(),id));
#endif
	}
	catch (std::out_of_range&)
	{
		cerr << "Can't find : " << id << endl;
		req->getResponse()->write("error");
		return;
	}
	
	shared_ptr<Slot> attr = entity->getProperty(attribute);
	req->getResponse()->write(attr->getValue()->getStringFromValue());
	req->getResponse()->flushBuffer();

}
extern "C" HttpServlet* createManageAttributeServlet()
{
  return new ManageAttributeServlet();
}
extern "C" void destroyManageAttributeServlet(HttpServlet* s)
{
	delete s;
}

