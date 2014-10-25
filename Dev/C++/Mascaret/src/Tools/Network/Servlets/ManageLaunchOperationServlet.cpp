#include "Tools/veha_plateform.h"
#include "Tools/Network/Servlets/ManageLaunchOperationServlet.h"

#include "VEHA/Kernel/Environment.h"
#include "VEHA/Kernel/InstanceSpecification.h"
#include "VEHA/Kernel/Class.h"
#include "VEHA/Kernel/Operation.h"
#include "VEHA/Kernel/Parameter.h"

#include "BEHAVE/Agent.h"

#include "MascaretApplication.h"

using VEHA::Environment;
using VEHA::InstanceSpecification;
using VEHA::Class;
using VEHA::Operation;
using VEHA::Parameter;

using BEHAVE::Agent;
using BEHAVE::ACLMessage;

using BEHAVE::REQUEST;

void ManageLaunchOperationServlet::manageRequest(HttpRequest* req)
{
	string id = req->getParameter("alias");
	string oper = req->getParameter("oper");

	shared_ptr<Environment> env = MascaretApplication::getInstance()->getEnvironment();
	shared_ptr<InstanceSpecification> entity;

	cerr << "[ManageLaunchOperationServlet Info] Launch operation: " << oper << endl;

#if GCC_VERSION > 40100
	entity=env->getInstances().at(id);
#else
	entity=MAP_AT(env->getInstances(),id);
#endif

	if(entity)
		cerr << "[ManageLaunchOperationServlet Info] Entity id: " << id << " found." << endl;
	else
		cerr << "[ManageLaunchOperationServlet Warning] Entity id: " << id << " not found." << endl;

	shared_ptr<Class> classifier = entity->getClassifier();
#if GCC_VERSION > 40100
	shared_ptr<Operation> operation=classifier->getOperations().at(oper);
#else
	shared_ptr<Operation> operation=MAP_AT(classifier->getOperations(),oper);
#endif

	if(operation)
		cerr << "[ManageLaunchOperationServlet Info] Operation named: " << oper << " found." << endl;
	else
		cerr << "[ManageLaunchOperationServlet Warning] Operation named: " << oper << " not found." << endl;

	Parameters param;
	string paramString = "";
	vector<shared_ptr<Parameter> > parameters = operation->getParameter();
	for (size_t i=parameters.size();i--;)
	{
		string strVal=req->getParameter(string("_")+parameters[i]->getName());
		for (unsigned int iC = 0; iC < strVal.size(); iC++)
		{
			if (strVal[iC] == '+') strVal[iC] = ' ';
		}
		cerr << " ----- > " << strVal << endl;
		param[parameters[i]->getName()]=parameters[i]->getType()->createValueFromString(strVal);
		paramString += " :" + parameters[i]->getName() + " " + strVal;
	}
	
	//---------------------------------------------------------------------------------//
	
	shared_ptr<Agent> agt = (shared_dynamic_cast<Agent>(entity));

	if (agt)
	{
		shared_ptr<ACLMessage> message = make_shared<ACLMessage>(REQUEST);
		string content = "((action "+agt->getAID()->getName()+" ("+operation->getName()+paramString+")))";
		message->setContent(content);
		message->setConversationID("launchOpServlet");
		message->addReceiver(agt->getAID());
		MascaretApplication::getInstance()->agent->send(message);
	}
	else
	{
		cerr << "[ManageLaunchOperationServlet Warning] Agent not found." << endl;
	}
	
	//---------------------------------------------------------------------------------//

	req->getResponse()->write("<html>");
	req->getResponse()->write("<body>");
	req->getResponse()->write("<META HTTP-EQUIV=\"Content-Type\" content=\"text/html; charset=UTF-8\">");
	req->getResponse()->write("<link href=\"doxygen.css\" rel=\"stylesheet\" type=\"text/css\">");
	req->getResponse()->write("Operation "+operation->getName()+" launched on entity "+entity->getName());
	req->getResponse()->write("</body>");
	req->getResponse()->write("</html>");
}
extern "C" HttpServlet* createManageLaunchOperationServlet()
{
	return new ManageLaunchOperationServlet();
}
extern "C" void destroyManageLaunchOperationServlet(HttpServlet* s)
{
	delete s;
}
