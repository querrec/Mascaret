#ifndef BEHAVE_RESSOURCEASSIGNEMENT_H
#define BEHAVE_RESSOURCEASSIGNEMENT_H

#include "Tools/veha_plateform.h"
#include "VEHA/Entity/Entity.h"

namespace BEHAVE
{
using VEHA::Entity;
class Ressource;

class VEHA_API RessourceAssignement
{
public :
	RessourceAssignement();
	virtual ~RessourceAssignement();

	// Ressource
public :
	inline void setRessource(shared_ptr<Ressource> ressource) {_ressource = ressource;}
	inline shared_ptr<Ressource> getRessource(void) { return _ressource;}
private :
	shared_ptr<Ressource> _ressource;

	// Entity
public :
	inline void setEntity(shared_ptr <Entity> entity) {_entity = entity;}
	inline shared_ptr<Entity>  getEntity(void) {return _entity;}
private :
	shared_ptr<Entity> _entity;
};
}

#endif

