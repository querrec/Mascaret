#include "Tools/Network/Servlets/ManageRoleClassServlet.h"
#include "MascaretApplication.h"
#include "BEHAVE/OrganisationalStructure.h"
#include "BEHAVE/Role.h"
#include "BEHAVE/RoleClass.h"
#include "BEHAVE/Agent.h"
#include "BEHAVE/AgentPlateform.h"
#include "VEHA/Kernel/Operation.h"
#include "VEHA/Kernel/Parameter.h"

using BEHAVE::OrganisationalStructure;
using BEHAVE::Role;
using BEHAVE::RoleClass;

void ManageRoleClassServlet::manageRequest(HttpRequest* req)
{
	string structName = req->getParameter("alias");
	string roleName = req->getParameter("role");

	req->getResponse()->write("<html>");
	req->getResponse()->write("<META HTTP-EQUIV=\"Content-Type\" content=\"text/html; charset=UTF-8\">");
	req->getResponse()->write("<body>");

	shared_ptr<OrganisationalStructure> struc;
	vector <shared_ptr<OrganisationalStructure> > structs = MascaretApplication::getInstance()->getAgentPlateform()->getOrganisationalStructure();
	bool found = false;
	int i = 0;
	while (!found && (unsigned)i < structs.size())
	{
		if (structs[i]->getName() == structName)
			found = true;
		else i++;
	}
	if (found) struc = structs[i];
	else cerr << "Erreur..." << endl;

	shared_ptr<Role> role;
	shared_ptr<RoleClass> classe;
	vector <shared_ptr<Role> > roles = struc->getRoles();
	found = false;
	i = 0;
	while (!found && (unsigned)i < roles.size())
	{
		if (roles[i]->getName() == roleName)
			found = true;
		else i++;
	}
	if (found) role = roles[i];
	else cerr << "Erreur..." << endl;
	classe = role->getRoleClass();

	req->getResponse()->write("<H2>Description</H2>");
	req->getResponse()->write("<ul>");
	req->getResponse()->write("<li>");
	req->getResponse()->write(classe->getName());
	req->getResponse()->write("</li>");
	req->getResponse()->write("<li>");
	req->getResponse()->write(classe->getFullName());
	req->getResponse()->write("</li>");
	req->getResponse()->write("<li>");
	req->getResponse()->write(classe->getSummary());
	req->getResponse()->write("</li>");
	req->getResponse()->write("<li>");
	req->getResponse()->write(classe->getDescription());
	req->getResponse()->write("</li>");
	req->getResponse()->write("</ul>");

	req->getResponse()->write("<HR>");
	req->getResponse()->write("<H2>Attributs</H2>");
	req->getResponse()->write("<ul>");
	const map <string,shared_ptr<Property> >& attributes = classe->getAttributes();
	map <string,shared_ptr<Property> >::const_iterator itA;
	for(itA=attributes.begin();itA!=attributes.end();itA++)
	{
		req->getResponse()->write("<li>");
		req->getResponse()->write(itA->first);
		req->getResponse()->write(" : ");
		req->getResponse()->write(itA->second->getType()->getName());
		req->getResponse()->write("</li>");
	}
	req->getResponse()->write("</ul>");

	req->getResponse()->write("<HR>");

	req->getResponse()->write("<HR>");
	req->getResponse()->write("<H2>Actions</H2>");
	const map<string, shared_ptr<Operation> >& operations = classe->getOperations();
	map<string, shared_ptr<Operation> >::const_iterator itO;
	for (itO=operations.begin();itO!=operations.end();itO++)
	{
		//     Operation * oper = operations[iOper];
		req->getResponse()->write("<li>");
		req->getResponse()->write(itO->first);
		req->getResponse()->write("( ");
		const vector <shared_ptr<Parameter> >& paramsT = itO->second->getParameter();

		for (unsigned int iP = 0; iP < paramsT.size(); iP++)
		{
			shared_ptr<Parameter> attr = paramsT[iP];
			req->getResponse()->write(attr->getName());
			req->getResponse()->write(" : <a href=\"Class?alias=");
			req->getResponse()->write(attr->getType()->getName());
			req->getResponse()->write("\" target = \"Body\">");
			req->getResponse()->write(attr->getType()->getName());
			req->getResponse()->write("</a>");
			if (iP !=  paramsT.size()-1)
				req->getResponse()->write(", ");
		}
		req->getResponse()->write(" )");
		req->getResponse()->write(" : ");
		if (itO->second->getType() )
			req->getResponse()->write(itO->second->getType()->getName());
		else
			req->getResponse()->write("undefined");
		req->getResponse()->write("</li>");
	}


	req->getResponse()->write("</body>");
	req->getResponse()->write("</html>");
}
extern "C" HttpServlet* createManageRoleClassServlet()
		{
	return new ManageRoleClassServlet();
		}
extern "C" void destroyManageRoleClassServlet(HttpServlet* s)
{
	delete s;
}
