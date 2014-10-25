/*
 * AReViShapeSpecification.h
 *
 *  Created on: 24 févr. 2011
 *      Author: soler
 */

#ifndef OGREPOINTSPECIFICATION_H_
#define OGREPOINTSPECIFICATION_H_

#include <boost/shared_ptr.hpp>

#include "ogreMascaret_plateform.h"
#include <Ogre.h>
#include "VEHA/Entity/PointSpecification.h"

using namespace boost;

namespace OgreMascaret
{
using VEHA::Vector3;
using VEHA::RotationVector;

class OGRE_MASCARET_API OgrePointSpecification : public VEHA::PointSpecification
{
public:
	OgrePointSpecification();
	OgrePointSpecification(Vector3 pos, RotationVector rotation);
	virtual ~OgrePointSpecification();

	virtual Vector3 getLocalPosition() const;
	virtual void setLocalPosition(const Vector3& pos);
	virtual Vector3 getGlobalPosition() const;
	virtual void setGlobalPosition(const Vector3& pos);
	virtual void translate(const Vector3& pos);

	virtual RotationVector getLocalRotation() const;
	virtual void setLocalRotation(const RotationVector& orientation);
	virtual RotationVector getGlobalRotation() const;
	virtual void setGlobalRotation(const RotationVector& orientation);
	virtual void rotate(const RotationVector& orientation);
	virtual void rotate(const Vector3& orientation);

	virtual void setLocalOrientation(const Vector3& ori);
	virtual Vector3 getLocalOrientation() const;
	virtual void setGlobalOrientation(const Vector3& ori);
	virtual Vector3 getGlobalOrientation() const;

	virtual void globalToLocalLocation(shared_ptr<PointSpecification> pointInOut) const;
	virtual void localToGlobalLocation(shared_ptr<PointSpecification> pointInOut) const;

	virtual void globalToLocalPosition(Vector3& posInOut) const;
	virtual void localToGlobalPosition(Vector3& posInOut) const;

	virtual void localToGlobalOrientation(Vector3& oriInOut) const;
	virtual void globalToLocalOrientation(Vector3& oriInOut) const;

	virtual void setParent(shared_ptr<PointSpecification> point);

	inline Ogre::SceneNode * getNode() {return _node;}
protected:
	Ogre::Quaternion _quaternionFromEuler(double x,double y,double z) const;
	Ogre::SceneNode * _node;
	static unsigned int _id;
};
}
#endif /* AREVISOUNDSPECIFICATION_H_ */
