#ifndef BEHAVE_ORGANISATIONALSTRUCTURE_H
#define BEHAVE_ORGANISATIONALSTRUCTURE_H

#include "Tools/veha_plateform.h"
#include "VEHA/Kernel/Element.h"

namespace BEHAVE
{
class Role;
class Ressource;
class OrganisationalEntity;
class Procedure;

class VEHA_API OrganisationalStructure : public VEHA::Element
{
public :
	OrganisationalStructure(const string & name);
	virtual ~OrganisationalStructure();

	// Organisational entities
public :
	inline void addEntity(shared_ptr<OrganisationalEntity> entity) {_entities.push_back(entity);}
	inline vector<shared_ptr<OrganisationalEntity> > getEntities(void) { return _entities;}
private :
	vector < shared_ptr<OrganisationalEntity> > _entities;

	// Roles
public :
	virtual inline void addRole(shared_ptr<Role> role) {_roles.push_back(role);}
	shared_ptr<Role> getRole(string roleName) const;
	void removeRole(shared_ptr<Role>);
	inline vector < shared_ptr<Role> > getRoles(void) const {return _roles;}
private :
	vector < shared_ptr<Role> > _roles;

	// Ressources
public :
	inline void addRessource(shared_ptr<Ressource> ressource) {_ressources.push_back(ressource);}
	inline vector < shared_ptr<Ressource> > getRessources(void) {return _ressources;}
private :
	vector < shared_ptr<Ressource> > _ressources;

	// Procedure
public :
	inline void addProcedure(shared_ptr<Procedure> procedure) {_procedures.push_back(procedure);}
	inline vector < shared_ptr<Procedure> > getProcedures(void) {return _procedures;}
private :
	vector < shared_ptr<Procedure> > _procedures;

};
}

#endif

