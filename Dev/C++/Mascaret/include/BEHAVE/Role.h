#ifndef BEHAVE_ROLE_H
#define BEHAVE_ROLE_H

#include "Tools/veha_plateform.h"
#include "VEHA/Kernel/Element.h"

namespace BEHAVE
{
using VEHA::Element;
class RoleClass;
class Ressource;

class VEHA_API Role : public Element
{
public :
	Role(const string & name);
	virtual ~Role();

	// Role Class
public:
	inline void setRoleClass(shared_ptr<RoleClass> roleClass) {_roleClass = roleClass;}
	inline shared_ptr<RoleClass> getRoleClass(void) { return _roleClass;}
private :
	shared_ptr<RoleClass> _roleClass;

	// Ressources
public :
	inline void addRessource(shared_ptr<Ressource> ressource) {_ressources.push_back(ressource);}
	inline vector < shared_ptr<Ressource> > getRessources(void) {return _ressources;}
private:
	vector < shared_ptr<Ressource> > _ressources;

};
}

#endif

