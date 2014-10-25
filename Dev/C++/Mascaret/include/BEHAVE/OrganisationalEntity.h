#ifndef BEHAVE_ORGANISATIONALENTITY_H
#define BEHAVE_ORGANISATIONALENTITY_H

#include "Tools/veha_plateform.h"
#include "VEHA/Kernel/Element.h"


namespace BEHAVE
{
class RoleAssignement;
class RessourceAssignement;
class OrganisationalStructure;
using VEHA::Element;
class VEHA_API OrganisationalEntity : public Element
{
public :
	OrganisationalEntity(const string & name);
	virtual ~OrganisationalEntity();

	inline void setStructure(shared_ptr<OrganisationalStructure> structure) {_structure = structure;}
	inline shared_ptr<OrganisationalStructure> getStructure(void) { return _structure;}

	inline void addRoleAssignement(shared_ptr <RoleAssignement> assignement) {_roleAssignements.push_back(assignement);}
	inline vector< shared_ptr<RoleAssignement> > getRoleAssignements(void) {return _roleAssignements;}
	shared_ptr<RoleAssignement> findRoleAssignement(const string& roleName);

	inline void addRessourceAssignement(shared_ptr <RessourceAssignement> assignement) {_ressourceAssignements.push_back(assignement);}
	inline vector< shared_ptr<RessourceAssignement> > getRessourceAssignements(void) {return _ressourceAssignements;}
	shared_ptr<RessourceAssignement> findRessourceAssignement(const string& resourceName);
 
    void stopProcedure(const string& procedureName);
    void stopAllProcedures(void);

private :
	vector < shared_ptr<RoleAssignement> > _roleAssignements;
	vector < shared_ptr<RessourceAssignement> > _ressourceAssignements;
	shared_ptr<OrganisationalStructure> _structure;
};
}

#endif

