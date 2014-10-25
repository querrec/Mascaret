/*
 * AReViPointSpecification.cpp
 *
 *  Created on: 25 févr. 2011
 *      Author: soler
 */

#include "VRPlateform/AReViPointSpecification.h"
#include "VEHA/Entity/RotationVector.h"
using namespace AReVi;
using namespace VEHA;

namespace AReViMascaret
{
AReViPointSpecification::AReViPointSpecification()
: PointSpecification(), _point(Base3D::NEW())
{

}
AReViPointSpecification::AReViPointSpecification(Vector3 pos, RotationVector rotation)
: PointSpecification(), _point(Base3D::NEW())
{
	setGlobalPosition(pos);
	setGlobalRotation(rotation);
}
AReViPointSpecification::~AReViPointSpecification()
{

}

void AReViPointSpecification::setLocalPosition(const Vector3& pos)
{
	Vector3 pos2=pos;
	if(_parent)
		_parent->localToGlobalPosition(pos2);
	_point->setPosition(pos2.x,pos2.y,pos2.z);
}
Vector3 AReViPointSpecification::getLocalPosition() const
{
	double x,y,z;
	_point->getPosition(x,y,z);
	Vector3 pos(x,y,z);
	if(_parent)
		_parent->globalToLocalPosition(pos);
	return pos;
}
Vector3 AReViPointSpecification::getGlobalPosition() const
{
	double x,y,z;
	_point->getPosition(x,y,z);
	return Vector3(x,y,z);
}
void AReViPointSpecification::setGlobalPosition(const Vector3& pos)
{
	_point->setPosition(pos.x,pos.y,pos.z);
}
void AReViPointSpecification::translate(const Vector3& pos)
{
	_point->translate(pos.x,pos.y,pos.z);
}

RotationVector AReViPointSpecification::getLocalRotation() const
{
	double x,y,z,a;
	_point->extractRotation(x,y,z,a);
	//TODO
	//_point->
	return RotationVector(x,y,z,a);
}
void AReViPointSpecification::setLocalRotation(const RotationVector& orientation)
{
	//TODO
	_point->setRotation(orientation.x,orientation.y,orientation.z,orientation.angle);
}
RotationVector AReViPointSpecification::getGlobalRotation() const
{
	double x,y,z,a;
	_point->extractRotation(x,y,z,a);
	return RotationVector(x,y,z,a);
}
void AReViPointSpecification::setGlobalRotation(const RotationVector& orientation)
{
	_point->setRotation(orientation.x,orientation.y,orientation.z,orientation.angle);
}
void AReViPointSpecification::rotate(const RotationVector& orientation)
{
	_point->rotate(orientation.x,orientation.y,orientation.z,orientation.angle);
}
void AReViPointSpecification::rotate(const Vector3& orientation)
{
	_point->roll(orientation.x);
	_point->pitch(orientation.y);
	_point->yaw(orientation.z);
}
void AReViPointSpecification::setLocalOrientation(const Vector3& ori)
{
	Vector3 ori2=ori;
	if(_parent)
		_parent->localToGlobalOrientation(ori2);
	_point->setOrientation(ori2.x,ori2.y,ori2.z);
}
Vector3 AReViPointSpecification::getLocalOrientation() const
{
	double x,y,z;
	_point->extractOrientation(x,y,z);
	Vector3 ori(x,y,z);
	if(_parent)
		_parent->globalToLocalOrientation(ori);
	return ori;
}

void AReViPointSpecification::setGlobalOrientation(const Vector3& ori)
{
	_point->setOrientation(ori.x,ori.y,ori.z);
}
Vector3 AReViPointSpecification::getGlobalOrientation() const
{
	double x,y,z;
	_point->extractOrientation(x,y,z);
	return Vector3(x,y,z);
}

void AReViPointSpecification::globalToLocalLocation(shared_ptr<PointSpecification> pointInOut) const
{
	_point->globalToLocalLocation(shared_dynamic_cast<AReViPointSpecification>(pointInOut)->_point);
}
void AReViPointSpecification::localToGlobalLocation(shared_ptr<PointSpecification> pointInOut) const
{
	_point->globalToLocalLocation(shared_dynamic_cast<AReViPointSpecification>(pointInOut)->_point);
}

void AReViPointSpecification::globalToLocalPosition(Vector3& posInOut) const
{
	_point->globalToLocalPosition(posInOut.x,posInOut.y,posInOut.z);
}
void AReViPointSpecification::localToGlobalPosition(Vector3& posInOut) const
{
	_point->localToGlobalPosition(posInOut.x,posInOut.y,posInOut.z);
}

void AReViPointSpecification::localToGlobalOrientation(Vector3& oriInOut) const
{
	_point->localToGlobalOrientation(oriInOut.x,oriInOut.y,oriInOut.z);
}
void AReViPointSpecification::globalToLocalOrientation(Vector3& oriInOut) const
{
	_point->globalToLocalOrientation(oriInOut.x,oriInOut.y,oriInOut.z);
}

void AReViPointSpecification::setParent(shared_ptr<PointSpecification> point)
{
	if(_parent)
		_point->attachTo(shared_dynamic_cast<AReViPointSpecification>(_parent)->_point,Base3D::ATTACH_NONE);
	PointSpecification::setParent(point);
	if(point)
		_point->attachTo(shared_dynamic_cast<AReViPointSpecification>(point)->_point);
}

}
