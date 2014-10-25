#include "BEHAVE/OrganisationalEntity.h"
#include "BEHAVE/RoleAssignement.h"
#include "BEHAVE/RessourceAssignement.h"
#include "BEHAVE/Role.h"
#include "BEHAVE/Ressource.h"
#include "BEHAVE/AgentPlateform.h"
#include "BEHAVE/ACLMessage.h"

namespace BEHAVE
{

OrganisationalEntity::OrganisationalEntity(const string & name) : Element(name)
{}

OrganisationalEntity::~OrganisationalEntity()
{}

shared_ptr<RoleAssignement> OrganisationalEntity::findRoleAssignement(const string& roleName)
{
	for(size_t i=0;i<_roleAssignements.size();i++)
	{
		if(_roleAssignements[i]->getRole()->getName() == roleName)
			return _roleAssignements[i];
	}
	return shared_ptr<RoleAssignement>();
}
shared_ptr<RessourceAssignement> OrganisationalEntity::findRessourceAssignement(const string& resourceName)
{
	for(size_t i=0;i<_ressourceAssignements.size();i++)
	{
		if(_ressourceAssignements[i]->getRessource()->getName() == resourceName)
			return _ressourceAssignements[i];
	}
	return shared_ptr<RessourceAssignement>();
}

void OrganisationalEntity::stopProcedure(const string& procedureName)
{
	for(size_t i=0;i<_roleAssignements.size();i++)
	{
	   shared_ptr<AID> agent = _roleAssignements[i]->getAgent();
	   shared_ptr<ACLMessage> message = shared_ptr<ACLMessage>(new ACLMessage(REQUEST));
           string content = "STOP " + procedureName;
           message->setContent(content);
           message->addReceiver(agent);
           
           MascaretApplication::getInstance()->getAgentPlateform()->sendMessage(message);

	}
}

void OrganisationalEntity::stopAllProcedures()
{
	for(size_t i=0;i<_roleAssignements.size();i++)
	{
	   shared_ptr<AID> agent = _roleAssignements[i]->getAgent();
	   shared_ptr<ACLMessage> message = shared_ptr<ACLMessage>(new ACLMessage(REQUEST));
           string content = "STOP ALL";
           message->setContent(content);
           message->addReceiver(agent);
           
           MascaretApplication::getInstance()->getAgentPlateform()->sendMessage(message);

	}
}

}
