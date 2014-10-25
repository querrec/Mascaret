#include "Tools/Network/Servlets/ManageProcedureServlet.h"
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

#include "Tools/GraphvizExport.h"
#include <sstream>

using namespace VEHA;

using BEHAVE::OrganisationalEntity;
using BEHAVE::OrganisationalStructure;
using BEHAVE::Procedure;

using GraphTools::GraphVizActivity;
using GraphTools::GraphVizExport;

bool _isBaseType2(shared_ptr<Classifier> type)
{
    if (type->getName() == "integer") return true;
    if (type->getName() == "real") return true;
    if (type->getName() == "string") return true;
    else return false;
}


void ManageProcedureServlet::manageRequest(HttpRequest* req)
{
	string procName = req->getParameter("alias");
	string orgName = req->getParameter("org");

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
	const map<string, shared_ptr< InstanceSpecification> >& ents = env->getInstances();


	vector<shared_ptr<Parameter> > parameters ;
	vector< shared_ptr<ActivityNode> > nodes  = proc->getActivity()->getNode();
	for(size_t iNode = 0; iNode < nodes.size(); iNode ++)
	{
		if (nodes[iNode]->getKind() == "parameter")
			parameters.push_back((shared_dynamic_cast<ActivityParameterNode>(nodes[iNode]))->getParameter());
	}

	req->getResponse()->write("<HR>");
	req->getResponse()->write("<H2>Parametres</H2>");
	req->getResponse()->write("<FORM METHOD=POST action=\"launchProcedure\">");
	req->getResponse()->write("<input type=\"hidden\" name=\"org\" value=\"");
	req->getResponse()->write(orgName);
	req->getResponse()->write("\"/>");
	req->getResponse()->write("<input type=\"hidden\" name=\"alias\" value=\"");
	req->getResponse()->write(procName);
	req->getResponse()->write("\"/>");
	req->getResponse()->write("<ul>");
	for (unsigned int i = 0; i < parameters.size(); i++)
	{
		shared_ptr<Parameter> param = parameters[i];
		req->getResponse()->write("<li>");
		shared_ptr<Classifier> type = param->getType();
		req->getResponse()->write(type->getName());
		req->getResponse()->write("\t");
		req->getResponse()->write(param->getName());
		if (_isBaseType2(type))
		{
			req->getResponse()->write(" = ");
			req->getResponse()->write(" <INPUT name=_");
			req->getResponse()->write(param->getName());
			req->getResponse()->write(">");
			req->getResponse()->write("</li>");
		}
		else
		{
			req->getResponse()->write(" <select name=\"_");
			req->getResponse()->write(param->getName());
			req->getResponse()->write("\">");

			map<string, shared_ptr<InstanceSpecification> >::const_iterator it;
			for (it=ents.begin();it!=ents.end();it++)
				//         for (unsigned int iEnt = 0; iEnt < ents.size(); iEnt ++)
			{
				if (it->second->getClassifier())
				{
					if (it->second->getClassifier()->isA(type))
					{
						req->getResponse()->write(" = <option value=\"");
						req->getResponse()->write(it->first);
						req->getResponse()->write("\">");
						req->getResponse()->write(it->first);
						req->getResponse()->write("</option>");
					}
				}
			}
			req->getResponse()->write("</select>");
		}
	}
	req->getResponse()->write("</ul>");
	req->getResponse()->write("<INPUT TYPE=\"submit\" VALUE=\"Lancer\">");
	req->getResponse()->write("</FORM>");


/*
	vector< shared_ptr<RoleAssignement> > assignements = org->getRoleAssignements();
	for (size_t iA = 0; iA < assignements.size() ; iA++)
	{
		shared_ptr<ACLMessage> procMsg = shared_ptr<ACLMessage>(new ACLMessage(REQUEST));
		procMsg->addReceiver(assignements[iA]->getAgent());

		//procMsg->setSender(_agent->getAID());
		shared_ptr<AID> agentAID=MascaretApplication::getInstance()->agent->getAID();
		procMsg->setSender(agentAID);
		string content ="((action ";
		content += assignements[iA]->getAgent()->getName();
		content += " ";
		content += "(" + proc->getName() +")";
		content += "))";
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
*/

/*
	std::stringstream dotfile;
	GraphVizActivity::exporting(dotfile, proc->getActivity());
	string svg = GraphVizExport::generate_svg(dotfile.str());

	req->getResponse()->write(svg);
*/

	req->getResponse()->write("</body>");
	req->getResponse()->write("</html>");
}

extern "C" HttpServlet* createManageProcedureServlet()
{
	return new ManageProcedureServlet();
}
extern "C" void destroyManageProcedureServlet(HttpServlet* s)
{
	delete s;
}
