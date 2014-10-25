#include "Tools/Network/Servlets/ManageActionServlet.h"
#include "MascaretApplication.h"
#include "BEHAVE/OrganisationalEntity.h"
#include "BEHAVE/OrganisationalStructure.h"
#include "BEHAVE/Procedure.h"
#include "BEHAVE/RoleClass.h"
#include "BEHAVE/RessourceAssignement.h"
#include "BEHAVE/Agent.h"
#include "BEHAVE/AgentPlateform.h"
#include "HAVE/VirtualHuman.h"
#include "HAVE/VirtualHumanClass.h"
#include "VEHA/Behavior/Activity/ActivityPartition.h"
#include "VEHA/Behavior/Activity/ActivityNode.h"
#include "VEHA/Behavior/Activity/ObjectNode.h"
#include "VEHA/Kernel/Model.h"


using namespace VEHA;
using HAVE::VirtualHuman;
using HAVE::VirtualHumanClass;

using BEHAVE::AID;
using BEHAVE::ACLMessage;
using BEHAVE::OrganisationalEntity;
using BEHAVE::OrganisationalStructure;
using BEHAVE::Procedure;
using BEHAVE::KnowledgeBase;

void ManageActionServlet::manageRequest(HttpRequest* req)
{
	string actionName = req->getParameter("alias");
	string orgName = req->getParameter("org");
	string roleName = req->getParameter("role");
	string procName = req->getParameter("proc");

	/* Execution de l'action */
	shared_ptr<OrganisationalEntity> org;
	vector <shared_ptr<OrganisationalEntity> > orgs = MascaretApplication::getInstance()->getAgentPlateform()->getOrganisationalEntity();
	bool found = false;
	int i = 0;
	while (!found && (unsigned)i < orgs.size())
	{
		if (orgs[i]->getName() == orgName)
			found = true;
		else i++;
	}
	if (found) org = orgs[i];
	else cerr << "Erreur..." << endl;

	shared_ptr<OrganisationalStructure> struc = org->getStructure();
	vector < shared_ptr<Procedure> > procs = struc->getProcedures();
	shared_ptr<Procedure> proc;
	found = false;
	i= 0;
	while (!found && (unsigned)i < procs.size())
	{
		if (procs[i]->getName() == procName)
			found = true;
		else i++;
	}
	if (found) proc = procs[i];
	else cerr << "Erreur ..." << endl;

	string tmp = actionName;

	if (actionName[0] == '/')
		tmp = actionName.substr(1);
	if (tmp[tmp.size()-1] == ')')
		tmp = tmp.substr(0,tmp.size()-2);
	actionName = tmp;


	vector< shared_ptr<ActivityPartition> > partitions=proc->getActivity()->getPartition();
	for(size_t i=0;i<partitions.size();i++)
	{
		if(partitions[i]->getName() == roleName)
		{
			string name=partitions[i]->getRole()->getRoleClass()->getName();
			vector<shared_ptr<Class> > classes=MascaretApplication::getInstance()->getModel()->getClassesByName(name);
			for(size_t j=0;j<classes.size();j++)
			{
				if(classes[j] != partitions[i]->getRole()->getRoleClass())
				{
					shared_ptr<VirtualHumanClass> vhClasse=shared_dynamic_cast<VirtualHumanClass>(classes[j]);
					if(vhClasse)
					{

						shared_ptr<VirtualHuman> h(new VirtualHuman(MascaretApplication::getInstance()->getAgentPlateform(), "dummy",vhClasse));
						InstanceSpecification::OperationCallParameters p;
						vector< shared_ptr<ActivityNode> > nodes=partitions[i]->getNode();
						for(size_t k=0;k<nodes.size();k++)
						{
							if(nodes[k]->getName().find(actionName)!= string::npos)
							{
								vector< shared_ptr<ObjectNode> >objects=nodes[k]->getOutgoingObjectNode();
								for(size_t l=0;l<objects.size();l++)
								{
									p.push_back(shared_ptr<InstanceValue>(new InstanceValue(org->findRessourceAssignement(objects[l]->getName())->getEntity())));
								}
								break;
							}
						}
						h->executeOperation(actionName,p);
					}
					break;
				}
			}
			break;
		}
	}




	vector< pair<string , shared_ptr<AID> > >  agts = MascaretApplication::getInstance()->getAgentPlateform()->getAgentToInform();
	shared_ptr<AID> agentAID=MascaretApplication::getInstance()->agent->getAID();
	for (size_t i = 0; i < agts.size() ; i++)
	{
		shared_ptr<ACLMessage> procMsg = shared_ptr<ACLMessage>(new ACLMessage(BEHAVE::INFORM));
		procMsg->setSender(agentAID);
		string content ="((action ";
		content += agentAID->getName();
		content += " ";
		content += "(" + actionName +")";
		content += "))";
		procMsg->setContent(content);
		procMsg->setConversationID(agts[i].first);
		procMsg->addReceiver(agts[i].second);
		MascaretApplication::getInstance()->getAgentPlateform()->sendMessage(procMsg);
	}


	req->getResponse()->write("<html>");
	req->getResponse()->write("<META HTTP-EQUIV=\"Content-Type\" content=\"text/html; charset=UTF-8\">");
	req->getResponse()->write("<body>");
	req->getResponse()->write("Execution de l'action " + actionName +"<br/>");
	req->getResponse()->write("Procedure " + proc->getName() + "<br/>");

	req->getResponse()->write("Role " + roleName +"<br/>");
	req->getResponse()->write("</body>");
	req->getResponse()->write("</html>");
}

extern "C" HttpServlet* createManageActionServlet()
{
	return new ManageActionServlet();
}
extern "C" void destroyManageActionServlet(HttpServlet* s)
{
	delete s;
}
