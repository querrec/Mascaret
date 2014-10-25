#include "Tools/Network/Servlets/ManageLaunchProcedureServlet.h"
#include "MascaretApplication.h"
#include "BEHAVE/OrganisationalEntity.h"
#include "BEHAVE/OrganisationalStructure.h"
#include "BEHAVE/Procedure.h"
#include "BEHAVE/RoleAssignement.h"
#include "BEHAVE/Agent.h"
#include "BEHAVE/AgentPlateform.h"
#include "VEHA/Behavior/Activity/Activity.h"
#include "VEHA/Behavior/Activity/ActivityNode.h"
#include "VEHA/Behavior/Activity/ActivityPartition.h"
#include "VEHA/Behavior/Activity/ActivityParameterNode.h"
#include "VEHA/Kernel/Parameter.h"
using namespace VEHA;

using BEHAVE::ACLMessage;
using BEHAVE::AID;
using BEHAVE::OrganisationalEntity;
using BEHAVE::OrganisationalStructure;
using BEHAVE::Procedure;
using BEHAVE::RoleAssignement;

void ManageLaunchProcedureServlet::manageRequest(HttpRequest* req)
{
	string procName = req->getParameter("alias");
	string orgName = req->getParameter("org");

	//cerr << req->getRequestString() << endl;

	req->getResponse()->write("<html>");
	req->getResponse()->write("<META HTTP-EQUIV=\"Content-Type\" content=\"text/html; charset=UTF-8\">");
	req->getResponse()->write("<body>");

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

        shared_ptr<Environment> env = MascaretApplication::getInstance()->getEnvironment();


	vector<shared_ptr<Parameter> > parameters ;
        vector< shared_ptr<ActivityNode> > nodes  = proc->getActivity()->getNode();
        for (unsigned int iNode = 0; iNode < nodes.size(); iNode ++)
        {
           if (nodes[iNode]->getKind() == "parameter")
              parameters.push_back((shared_dynamic_cast<ActivityParameterNode>(nodes[iNode]))->getParameter());
        }

        Parameters param;
        string paramString = "";
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

	vector< shared_ptr<RoleAssignement> > assignements = org->getRoleAssignements();
	for (size_t iA = 0; iA < assignements.size() ; iA++)
	{
		shared_ptr<ACLMessage> procMsg = shared_ptr<ACLMessage>(new ACLMessage(BEHAVE::REQUEST));
		procMsg->addReceiver(assignements[iA]->getAgent());

		//procMsg->setSender(_agent->getAID());
		cout << "ManageLaunchProcedureServlet ::: SENDING MESSAGE TO "
			<< assignements[iA]->getAgent()->getName() << endl;
		shared_ptr<AID> agentAID=MascaretApplication::getInstance()->agent->getAID();
		procMsg->setSender(agentAID);
		string content ="((action ";
		content += assignements[iA]->getAgent()->getName();
		content += " ";
		content += "(" + proc->getName();
		content += paramString;
		content += ")))";
		cout << "\t\t\t" << content << endl;
		procMsg->setContent(content);
		MascaretApplication::getInstance()->getAgentPlateform()->sendMessage(procMsg);

		if (agentAID->getName() == assignements[iA]->getAgent()->getName())
		{
			req->getResponse()->write("<H2>Vous jouez le role ");
			req->getResponse()->write(assignements[iA]->getRole()->getName());
			req->getResponse()->write("</H2>");
			req->getResponse()->write("<H3>Vous pouvez faire les actions suivantes : </H3>");

			req->getResponse()->write("<ul>");
			shared_ptr<VEHA::Activity> act = proc->getActivity();
			vector< shared_ptr<ActivityPartition> > partitions = act->getPartition();
			for (size_t iPart = 0; iPart < partitions.size(); iPart++)
			{
				if (partitions[iPart]->getName() == assignements[iA]->getRole()->getName())
				{
					vector< shared_ptr<ActivityNode> > nodes = partitions[iPart]->getNode();
					for (unsigned iNode = 0; iNode < nodes.size(); iNode ++)
					{
						if (nodes[iNode]->getKind() == "action")
						{
							req->getResponse()->write("<li>");
							req->getResponse()->write("<a href=\"Actions?alias=");
							req->getResponse()->write(nodes[iNode]->getName());
							req->getResponse()->write("&org=");
							req->getResponse()->write(org->getName());
							req->getResponse()->write("&role=");
							req->getResponse()->write(assignements[iA]->getRole()->getName());
							req->getResponse()->write("&proc=");
							req->getResponse()->write(proc->getName());
							req->getResponse()->write("\" target = \"Body\">");
							req->getResponse()->write(nodes[iNode]->getName());
							req->getResponse()->write("</a></li>");
						}
					}
				}
			}
			req->getResponse()->write("</ul>");
		}
	}

	req->getResponse()->write("</body>");
	req->getResponse()->write("</html>");
}

extern "C" HttpServlet* createManageLaunchProcedureServlet()
{
	return new ManageLaunchProcedureServlet();
}
extern "C" void destroyManageLaunchProcedureServlet(HttpServlet* s)
{
	delete s;
}
