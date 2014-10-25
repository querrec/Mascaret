/*
 * Zone.cpp
 *
 *  Created on: 24 févr. 2011
 *      Author: soler
 */

#include "VEHA/Entity/Zone.h"

namespace VEHA
{
Zone::Zone(const string& name,shared_ptr<EntityClass> cl)
:Entity(name,cl)
{

}
Zone::~Zone()
{

}

std::vector<shared_ptr<Entity> > getEntities()
{
	vector<shared_ptr<Entity> > entities;
	return entities;
}

}
