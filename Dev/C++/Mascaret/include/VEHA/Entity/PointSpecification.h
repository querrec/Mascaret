#ifndef _v2_VEHA_Entity_Point_H
#define _v2_VEHA_Entity_Point_H

#include "Tools/veha_plateform.h"
#include "VEHA/Entity/TopologicalSpecification.h"
//#include "VEHA/Entity/RotationVector.h"
#include "VEHA/Entity/Vector3.h"

namespace VEHA
{
class RotationVector;
class VEHA_API PointSpecification : public TopologicalSpecification
{
public:
	PointSpecification();
	virtual ~PointSpecification();

	virtual Vector3 getLocalPosition() const=0;
	virtual void setLocalPosition(const Vector3& pos)=0;
	virtual Vector3 getGlobalPosition() const=0;
	virtual void setGlobalPosition(const Vector3& pos)=0;

	virtual void translate(const Vector3& pos)=0;

	virtual RotationVector getLocalRotation() const=0;
	virtual void setLocalRotation(const RotationVector& orientation)=0;
	virtual RotationVector getGlobalRotation() const=0;
	virtual void setGlobalRotation(const RotationVector& orientation)=0;
	virtual void rotate(const RotationVector& orientation)=0;
	virtual void rotate(const Vector3& orientation)=0;

	virtual void setLocalOrientation(const Vector3& ori)=0;
	virtual Vector3 getLocalOrientation() const=0;
	virtual void setGlobalOrientation(const Vector3& ori)=0;
	virtual Vector3 getGlobalOrientation() const=0;

	virtual void globalToLocalLocation(shared_ptr<PointSpecification> pointInOut) const=0;
	virtual void localToGlobalLocation(shared_ptr<PointSpecification> pointInOut) const=0;

	virtual void globalToLocalPosition(Vector3& posInOut) const=0;
	virtual void localToGlobalPosition(Vector3& posInOut) const=0;

	virtual void localToGlobalOrientation(Vector3& oriInOut) const=0;
	virtual void globalToLocalOrientation(Vector3& oriInOut) const=0;

	virtual void setParent(shared_ptr<PointSpecification> point);
	virtual shared_ptr<PointSpecification> getParent();

	//virtual const ValueSpecification& operator= (const ValueSpecification&);
	virtual shared_ptr<ValueSpecification> clone() const;
protected:
	shared_ptr<PointSpecification> _parent;
};


inline void PointSpecification::setParent(shared_ptr<PointSpecification> point)
{
	_parent=point;
}
inline shared_ptr<PointSpecification> PointSpecification::getParent()
{
	return _parent;
}
}
#endif
