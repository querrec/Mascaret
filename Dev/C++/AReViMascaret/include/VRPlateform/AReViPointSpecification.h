/*
 * AReViShapeSpecification.h
 *
 *  Created on: 24 févr. 2011
 *      Author: soler
 */

#ifndef AREVIPOINTSPECIFICATION_H_
#define AREVIPOINTSPECIFICATION_H_
#include "AReVi/Lib3D/base3D.h"

#include "areviMascaret_plateform.h"
#include "VEHA/Entity/PointSpecification.h"

namespace AReViMascaret
{
using VEHA::Vector3;
using VEHA::RotationVector;

class AREVI_MASCARET_API AReViPointSpecification : public VEHA::PointSpecification
{
public:
	AReViPointSpecification();
	AReViPointSpecification(Vector3 pos, RotationVector rotation);
	virtual ~AReViPointSpecification();

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

	inline AReVi::ArRef<AReVi::Base3D> getPoint() const;
	inline void setPoint(AReVi::ArRef<AReVi::Base3D> point);
protected:
	friend class AReViShapeSpecification;
	friend class AReViSoundSpecification;
	friend class AReViLight;
	AReVi::ArRef<AReVi::Base3D> _point;
};
inline AReVi::ArRef<AReVi::Base3D> AReViPointSpecification::getPoint() const
{
	return _point;
}
inline void AReViPointSpecification::setPoint(AReVi::ArRef<AReVi::Base3D> point)
{
	_point=point;
}
}
#endif /* AREVISOUNDSPECIFICATION_H_ */
