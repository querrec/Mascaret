#include "Tools/Network/Servlets/ManageSignalServlet.h"
#include "VEHA/Kernel/Class.h"
#include "VEHA/Kernel/Environment.h"
#include "VEHA/Kernel/InstanceSpecification.h"
#include "VEHA/Behavior/StateMachine/StateMachineBehaviorExecution.h"
#include "VEHA/Behavior/BasicActions/Action.h"
#include "VEHA/Behavior/BasicActions/SendSignalAction.h"
#include "IEHA/VRApplication.h"
#include "IEHA/Visualisation/Window3D.h"

#include "MascaretApplication.h"

using namespace VEHA;
using namespace IEHA;

void ManageSignalServlet::manageRequest(HttpRequest* req)
{
	string id = req->getParameter("alias");
	string signalName = req->getParameter("signal");

	shared_ptr<Environment> env = MascaretApplication::getInstance()->getEnvironment();
	shared_ptr<InstanceSpecification> entity;
#if GCC_VERSION > 40100
	entity=env->getInstances().at(id);
#else
	entity=MAP_AT(env->getInstances(),id);
#endif

	cerr << id << " " << signalName << endl;
	cerr << "Sending Signal ..." << endl;

        VRApplication* app = dynamic_cast<VRApplication*>(MascaretApplication::getInstance());
        map<string, shared_ptr<ValueSpecification> > values;
        app->sendSignal(shared_dynamic_cast<Entity>(entity),signalName,values);

/*
	vector<shared_ptr<StateMachineBehaviorExecution> > stmExec = entity->getStateMachineExecutions();
	for (unsigned int i = 0; i < stmExec.size(); i++)
	{
		shared_ptr<Signal> signalC( new Signal(signalName) );
		shared_ptr<InstanceSpecification> signal( new InstanceSpecification(signalName, signalC) );
		(shared_dynamic_cast<StateMachineBehaviorExecution>(stmExec[i]))->addSignal(signal);
        	shared_ptr<Action> action2;
	        action2=shared_ptr<Action>(new SendSignalAction());
        	shared_dynamic_cast<SendSignalAction>(action2)->setSignal(signal);
	        shared_dynamic_cast<SendSignalAction>(action2)->setTarget(entity);
                _cbActionDone(action2);
	}
*/

}
extern "C" HttpServlet* createManageSignalServlet()
		{
	return new ManageSignalServlet();
		}
extern "C" void destroyManageSignalServlet(HttpServlet* s)
{
	delete s;
}
