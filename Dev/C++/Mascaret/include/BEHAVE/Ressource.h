#ifndef BEHAVE_RESSOURCE_H
#define BEHAVE_RESSOURCE_H

#include "Tools/veha_plateform.h"
#include "VEHA/Kernel/Element.h"
#include "VEHA/Entity/EntityClass.h"


namespace BEHAVE
{
using VEHA::EntityClass;
class VEHA_API Ressource : public VEHA::Element
{
public :
	Ressource(const string & name);
	virtual ~Ressource();

	// Entity Class
public :
	inline void setEntityClass(shared_ptr<EntityClass> entityClass) {_entityClass = entityClass;}
	inline shared_ptr<EntityClass> getEntityClass(void) { return _entityClass;}
private :
	shared_ptr<EntityClass> _entityClass;
};
}

#endif

