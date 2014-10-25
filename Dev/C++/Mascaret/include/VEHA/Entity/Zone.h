/*
 * Zone.h
 *
 *  Created on: 24 févr. 2011
 *      Author: soler
 */

#ifndef ZONE_H_
#define ZONE_H_
#include "Tools/veha_plateform.h"
#include "VEHA/Entity/Entity.h"

namespace VEHA
{

class VEHA_API Zone : public Entity
{
public:
	Zone(const string& name,shared_ptr<EntityClass> cl);
	virtual ~Zone();

	vector<shared_ptr<Entity> > getEntities();
};

}

#endif /* ZONE_H_ */
