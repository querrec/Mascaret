/*
 * AReViEntityObject.h
 *
 *  Created on: 4 mars 2011
 *      Author: soler
 */

#ifndef AREVIENTITYOBJECT_H_
#define AREVIENTITYOBJECT_H_
#include "AReVi/Lib3D/object3D.h"
#include "areviMascaret_plateform.h"
#include "VEHA/Entity/Entity.h"
#include "AReVi/activity.h"

namespace AReViMascaret
{
using AReVi::ArRef;
using AReVi::Object3D;
using VEHA::Entity;
using AReVi::Activity;
class AREVI_MASCARET_API AReViEntityObject : public Object3D
{
public:
	AR_CLASS(AReViEntityObject)
	AR_CONSTRUCTOR(AReViEntityObject)

	inline void setEntity(shared_ptr<Entity> entity);
	inline shared_ptr<Entity> getEntity();
	
	virtual bool restaure(ArRef<AReVi::Activity> act, double);

protected:
	shared_ptr<Entity> _entity;
};
void AReViEntityObject::setEntity(shared_ptr<Entity> entity)
{
	_entity=entity;
}
shared_ptr<Entity> AReViEntityObject::getEntity()
{
	return _entity;
}
}

#endif /* AREVIENTITYOBJECT_H_ */
