#ifndef BEHAVE_ROLEASSIGNEMENT_H
#define BEHAVE_ROLEASSIGNEMENT_H

#include "Tools/veha_plateform.h"
namespace BEHAVE
{
class AID;
class Role;
class OrganisationalEntity;
class RessourceAssignement;

class VEHA_API RoleAssignement
{
public :
	RoleAssignement(shared_ptr<OrganisationalEntity> organisation);
	virtual ~RoleAssignement();

	// Role
public :
	inline void setRole(shared_ptr<Role> role) {_role = role;}
	inline shared_ptr<Role> getRole(void) { return _role;}
private :
	shared_ptr<Role> _role;

	// Ressource assignement
public :
	inline void addRessourceAssignement(shared_ptr <RessourceAssignement> assignement) {_ressourceAssignements.push_back(assignement);}
	inline vector< shared_ptr<RessourceAssignement> > getRessourceAssignements(void) {return _ressourceAssignements;}
private :
	vector < shared_ptr<RessourceAssignement> > _ressourceAssignements;

	// Agent
public :
	inline void setAgent(shared_ptr <AID> agent) {_agent = agent;}
	inline shared_ptr<AID>  getAgent(void) {return _agent;}
private :
	shared_ptr<AID> _agent;

public :
	inline shared_ptr<OrganisationalEntity> getOrganisation() { return _organisation;}

private :
	shared_ptr<OrganisationalEntity> _organisation;
};
}

#endif

