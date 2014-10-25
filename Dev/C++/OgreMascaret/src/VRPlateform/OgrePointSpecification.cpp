/*
 * OgrePointSpecification.cpp
 *
 *  Created on: 25 févr. 2011
 *      Author: soler
 */
#ifdef _WIN32
#include <winsock2.h>
#endif
#include "VRPlateform/OgrePointSpecification.h"
#include "VRPlateform/OgreMascaretApplication.h"
#include <boost/shared_ptr.hpp>
#include "VEHA/Entity/RotationVector.h"

using namespace VEHA;
using namespace boost;
using namespace Ogre;
namespace OgreMascaret
{
unsigned int OgrePointSpecification::_id=0;
OgrePointSpecification::OgrePointSpecification()
: PointSpecification()
{
	string name="Node";
	name+=Ogre::StringConverter::toString(_id);
	_id++;
	_node=OgreMascaretApplication::getInstance()->getSceneManager()->createSceneNode(name);
	OgreMascaretApplication::getInstance()->getSceneManager()->getRootSceneNode()->addChild(_node);
}
OgrePointSpecification::OgrePointSpecification(Vector3 pos, RotationVector rotation)
: PointSpecification()
{
	string name="Node";
	name+=Ogre::StringConverter::toString(_id);
	_id++;
	_node=OgreMascaretApplication::getInstance()->getSceneManager()->createSceneNode(name);
	OgreMascaretApplication::getInstance()->getSceneManager()->getRootSceneNode()->addChild(_node);
	setGlobalPosition(pos);
	setGlobalRotation(rotation);
}
OgrePointSpecification::~OgrePointSpecification()
{

}
VEHA::Vector3 OgrePointSpecification::getLocalPosition() const
{
	Ogre::Vector3 vec=_node->getPosition();
	return VEHA::Vector3(vec.x,vec.y,vec.z);
}
void OgrePointSpecification::setLocalPosition(const VEHA::Vector3& pos)
{
	Ogre::Vector3 vec(Real(pos.x),Real(pos.y),Real(pos.z));
	_node->setPosition(vec);
        _node->_update(true,true);
}
VEHA::Vector3 OgrePointSpecification::getGlobalPosition() const
{
	Ogre::Vector3 vec;
	vec=_node->_getDerivedPosition();
	return VEHA::Vector3(vec.x,vec.y,vec.z);
}
void OgrePointSpecification::setGlobalPosition(const VEHA::Vector3& pos)
{
	Ogre::Vector3 vec(Real(pos.x),Real(pos.y),Real(pos.z));
	if(_parent)
		vec=shared_dynamic_cast<OgrePointSpecification>(_parent)->_node->convertWorldToLocalPosition(vec);
	_node->setPosition(vec);
	_node->_update(true,true);
}
void OgrePointSpecification::translate(const VEHA::Vector3& pos)
{
	_node->translate(Real(pos.x),Real(pos.y),Real(pos.z),Ogre::Node::TS_LOCAL);
	_node->_update(true,true);
}

VEHA::RotationVector OgrePointSpecification::getLocalRotation() const
{
	Ogre::Quaternion q=_node->getOrientation();
	Ogre::Vector3 v;
	Ogre::Radian a;
	q.ToAngleAxis(a,v);
	return VEHA::RotationVector((double)v.x,(double)v.y,(double)v.z,(double)a.valueRadians());
}
void OgrePointSpecification::setLocalRotation(const VEHA::RotationVector& orientation)
{
	Ogre::Quaternion q;
	Ogre::Vector3 v(orientation.x,orientation.y,orientation.z);
	q.FromAngleAxis(Ogre::Radian(orientation.angle),v);
	_node->setOrientation(q);
	_node->_update(true,true);
}

VEHA::RotationVector OgrePointSpecification::getGlobalRotation() const
{
	Ogre::Quaternion q=_node->getOrientation();
	if(_parent)
		q=shared_dynamic_cast<OgrePointSpecification>(_parent)->_node->convertLocalToWorldOrientation(q);
	Ogre::Vector3 v;
	Ogre::Radian a;
	q.ToAngleAxis(a,v);
	return VEHA::RotationVector((double)v.x,(double)v.y,(double)v.z,(double)a.valueRadians());
}
void OgrePointSpecification::setGlobalRotation(const VEHA::RotationVector& orientation)
{
	Ogre::Quaternion q;
	Ogre::Vector3 v(orientation.x,orientation.y,orientation.z);
	q.FromAngleAxis(Ogre::Radian(orientation.angle),v);
	if(getParent())
		q=shared_dynamic_cast<OgrePointSpecification>(_parent)->_node->convertWorldToLocalOrientation(q);
	_node->setOrientation(q);
	_node->_update(true,true);
}
void OgrePointSpecification::rotate(const VEHA::RotationVector& orientation)
{
	_node->rotate(Ogre::Vector3(Real(orientation.x),Real(orientation.y),Real(orientation.z)),Radian(orientation.angle));
	_node->_update(true,true);
}
void OgrePointSpecification::rotate(const Vector3& orientation)
{
	Ogre::Matrix3 m;
	m.FromEulerAnglesXYZ(Ogre::Radian(orientation.x),Ogre::Radian(orientation.y),Ogre::Radian(orientation.z));
	Ogre::Quaternion q(m);
	_node->rotate(q,Ogre::Node::TS_LOCAL);
	_node->_update(true,true);
}
void OgrePointSpecification::setLocalOrientation(const Vector3& ori)
{
	Ogre::Matrix3 m;
	m.FromEulerAnglesXYZ(Ogre::Radian(ori.x),Ogre::Radian(ori.y),Ogre::Radian(ori.z));
	Ogre::Quaternion q(m);
	_node->setOrientation(q);
	_node->_update(true,true);
}
VEHA::Vector3 OgrePointSpecification::getLocalOrientation() const
{
	Ogre::Quaternion q=_node->getOrientation();
	Ogre::Matrix3 m;
	q.ToRotationMatrix(m);
	Ogre::Radian x,y,z;
	m.ToEulerAnglesXYZ(x,y,z);

	return VEHA::Vector3((double)x.valueRadians(), (double)y.valueRadians(), (double)z.valueRadians());
}
void OgrePointSpecification::setGlobalOrientation(const Vector3& ori)
{
	Ogre::Matrix3 m;
	m.FromEulerAnglesXYZ(Ogre::Radian(ori.x),Ogre::Radian(ori.y),Ogre::Radian(ori.z));
	Ogre::Quaternion q(m);
	if(_parent)
		q=shared_dynamic_cast<OgrePointSpecification>(_parent)->_node->convertWorldToLocalOrientation(q);
	_node->setOrientation(q);
	_node->_update(true,true);
}
VEHA::Vector3 OgrePointSpecification::getGlobalOrientation() const
{
	Ogre::Quaternion q=_node->getOrientation();
	if(_parent)
		q=shared_dynamic_cast<OgrePointSpecification>(_parent)->_node->convertLocalToWorldOrientation(q);
	Ogre::Matrix3 m;
	q.ToRotationMatrix(m);
	Ogre::Radian x,y,z;
	m.ToEulerAnglesXYZ(x,y,z);

	return VEHA::Vector3((double)x.valueRadians(), (double)y.valueRadians(), (double)z.valueRadians());
}

void OgrePointSpecification::globalToLocalLocation(shared_ptr<PointSpecification> pointInOut) const
{
	cerr << "globalToLocalLocation not implemented" << endl;
}
void OgrePointSpecification::localToGlobalLocation(shared_ptr<PointSpecification> pointInOut) const
{
	cerr << "localToGlobalLocation not implemented" << endl;
}

void OgrePointSpecification::globalToLocalPosition(VEHA::Vector3& posInOut) const
{
	Ogre::Vector3 v(posInOut.x,posInOut.y,posInOut.z);
	v=_node->convertWorldToLocalPosition(v);
	posInOut.x=v.x;
	posInOut.y=v.y;
	posInOut.z=v.z;
}
void OgrePointSpecification::localToGlobalPosition(VEHA::Vector3& posInOut) const
{
	Ogre::Vector3 v(posInOut.x,posInOut.y,posInOut.z);
	v=_node->convertLocalToWorldPosition(v);
	posInOut.x=v.x;
	posInOut.y=v.y;
	posInOut.z=v.z;
}

void OgrePointSpecification::localToGlobalOrientation(VEHA::Vector3& oriInOut) const
{
	Ogre::Quaternion q=_quaternionFromEuler(oriInOut.x,oriInOut.y,oriInOut.z);
	q=_node->convertLocalToWorldOrientation(q);
	oriInOut.x=q.getRoll().valueRadians();
	oriInOut.y=q.getPitch().valueRadians();
	oriInOut.z=q.getYaw().valueRadians();
}
void OgrePointSpecification::globalToLocalOrientation(VEHA::Vector3& oriInOut) const
{
	Ogre::Quaternion q=_quaternionFromEuler(oriInOut.x,oriInOut.y,oriInOut.z);
	q=_node->convertWorldToLocalOrientation(q);
	oriInOut.x=q.getRoll().valueRadians();
	oriInOut.y=q.getPitch().valueRadians();
	oriInOut.z=q.getYaw().valueRadians();
}


void OgrePointSpecification::setParent(shared_ptr<PointSpecification> point)
{

	if (_node)
		_node->getParent()->removeChild(_node);

	PointSpecification::setParent(point);
	if(point)
		shared_dynamic_cast<OgrePointSpecification>(point)->_node->addChild(_node);
	else
		OgreMascaretApplication::getInstance()->getSceneManager()->getRootSceneNode()->addChild(_node);

	if (_node)
		_node->_update(true,true);
}

Ogre::Quaternion OgrePointSpecification::_quaternionFromEuler(double x,double y,double z) const
{
	Ogre::Quaternion q;
	//q= Ogre::Quaternion(Ogre::Radian(z), Ogre::Vector3::NEGATIVE_UNIT_Y) * Ogre::Quaternion(Ogre::Radian(y), Ogre::Vector3::UNIT_X) * Ogre::Quaternion(Ogre::Radian(x), Ogre::Vector3::NEGATIVE_UNIT_Z); 
	
	/*Ogre::Quaternion qx(Ogre::Radian(x),Ogre::Vector3(1,0,0));
	Ogre::Quaternion qy(Ogre::Radian(y),Ogre::Vector3(0,1,0));
	Ogre::Quaternion qz(Ogre::Radian(z),Ogre::Vector3(0,0,1));
	q=qz*qy*qx;*/
	

	// Assuming the angles are in radians.
    double c1 = cos(z/2);
    double s1 = sin(z/2);
    double c2 = cos(y/2);
    double s2 = sin(y/2);
    double c3 = cos(x/2);
    double s3 = sin(x/2);
    double c1c2 = c1*c2;
    double s1s2 = s1*s2;
    q.w =c1c2*c3 - s1s2*s3;
  	q.x =c1c2*s3 + s1s2*c3;
	q.y =s1*c2*c3 + c1*s2*s3;
	q.z =c1*s2*c3 - s1*c2*s3;


	/*double ti = x*0.5;
	double tj = y*0.5;
	double th = z*0.5;

	double ci = cos(ti);
	double cj = cos(tj);
	double ch = cos(th);
	double si = sin(ti);
	double sj = sin(tj);
	double sh = sin(th);
	double cc = ci*ch;
	double cs = ci*sh;
	double sc = si*ch;
	double ss = si*sh;
	q.w = cj*cc + sj*ss;
	q.x = cj*sc - sj*cs;
	q.y = cj*ss + sj*cc;
	q.z = cj*cs - sj*sc;*/
	return q;
}
}
