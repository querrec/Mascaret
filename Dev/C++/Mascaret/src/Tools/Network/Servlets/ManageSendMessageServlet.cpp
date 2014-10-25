#include "Tools/Network/Servlets/ManageSendMessageServlet.h"
#include "VEHA/Kernel/Class.h"
#include "VEHA/Kernel/Environment.h"
#include "HAVE/VirtualHuman.h"
#include "BEHAVE/ACLMessage.h"
#include "MascaretApplication.h"

using namespace VEHA;
using namespace HAVE;
using namespace BEHAVE;
void ManageSendMessageServlet::manageRequest(HttpRequest* req)
{
	string id = req->getParameter("alias");
	string performative = req->getParameter("perf");
	string contenu = req->getParameter("contenu");
	ACLPerformative perf;

cerr << " PERFORMATIVE " << performative << endl;

	if (performative == "ACCEPT_PROPOSAL") perf =ACCEPT_PROPOSAL;
	if (performative == "AGREE") perf =AGREE;
	if (performative == "CANCEL") perf =CANCEL;
	if (performative == "CFP") perf =CFP;
	if (performative == "CONFIRM") perf =CONFIRM;
	if (performative == "DISCONFIRM") perf =DISCONFIRM;
	if (performative == "FAILURE") perf =FAILURE;
	if (performative == "INFORM") perf =INFORM;
	if (performative == "INFORM_IF") perf =INFORM_IF;
	if (performative == "INFORM_REF") perf =INFORM_REF;
	if (performative == "NOT_UNDERSTOOD") perf =NOT_UNDERSTOOD;
	if (performative == "PROPAGATE") perf =PROPAGATE;
	if (performative == "PROPOSE") perf =PROPOSE;
	if (performative == "PROXY") perf =PROXY;
	if (performative == "QUERY_IF") perf =QUERY_IF;
	if (performative == "QUERY_REF") perf =QUERY_REF;
	if (performative == "REFUSE") perf =REFUSE;
	if (performative == "REJECT_PROPOSAL") perf =REJECT_PROPOSAL;
	if (performative == "REQUEST") perf =REQUEST;
	if (performative == "REQUEST_WHEN") perf =REQUEST_WHEN;
	if (performative == "REQUEST_WHENEVER") perf =REQUEST_WHENEVER;
	if (performative == "SUBSCRIBE") perf =SUBSCRIBE;
	if (performative == "UNKNOWN") perf =UNKNOWN;

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
	shared_ptr<VirtualHuman> human = shared_dynamic_cast<VirtualHuman>(entity);
	shared_ptr<ACLMessage> message = shared_ptr<ACLMessage>(new ACLMessage(perf));
	message->addReceiver(human->getAID());
	message->setContent(contenu);
	MascaretApplication::getInstance()->agent->send(message);
}
extern "C" HttpServlet* createManageSendMessageServlet()
		{
	return new ManageSendMessageServlet();
		}
extern "C" void destroyManageSendMessageServlet(HttpServlet* s)
{
	delete s;
}
