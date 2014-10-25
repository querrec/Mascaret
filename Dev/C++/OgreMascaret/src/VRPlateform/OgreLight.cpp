/*
 * OgreLight.cpp
 *
 *  Created on: 25 févr. 2011
 *      Author: soler
 */
#ifdef _WIN32
#include <winsock2.h>
#endif
#include "VRPlateform/OgreLight.h"
#include "VRPlateform/OgreMascaretApplication.h"
using VEHA::Color;
using namespace Ogre;
namespace OgreMascaret
{
unsigned int OgreLight::_id=0;
OgreLight::OgreLight() : _location(new OgrePointSpecification())
{
	string name="Light";
	name+=Ogre::StringConverter::toString(_id);
	_id++;
	_light=OgreMascaretApplication::getInstance()->getSceneManager()->createLight(name);
	_location->getNode()->attachObject(_light);
}
OgreLight::~OgreLight()
{
}

VEHA::Color OgreLight::getColor() const
{
	const ColourValue& c=_light->getDiffuseColour();
	return Color(c.r,c.g,c.b);
}
void OgreLight::setColor(const VEHA::Color& color)
{
	_light->setDiffuseColour(Real(color.r),Real(color.g),Real(color.b));
	_light->setSpecularColour(Real(color.r),Real(color.g),Real(color.b));
}

bool OgreLight::getDirectional() const
{
	return _light->getType() == Ogre::Light::LT_DIRECTIONAL;
}
void OgreLight::setDirectional(bool directional)
{
	if(directional)
		_light->setType(Ogre::Light::LT_DIRECTIONAL);
	else if(getSpotExponent() > 0)
		_light->setType(Ogre::Light::LT_SPOTLIGHT);
	else
		_light->setType(Ogre::Light::LT_POINT);
}

double OgreLight::getAttenuationConstant() const
{
	return _light->getAttenuationConstant();
}
void OgreLight::setAttenuationConstant(double constant)
{
	_light->setAttenuation(Real(1000),Real(constant),_light->getAttenuationLinear(),_light->getAttenuationQuadric());
}

double OgreLight::getAttenuationLinear() const
{
	return _light->getAttenuationLinear();
}
void OgreLight::setAttenuationLinear(double linear)
{
	_light->setAttenuation(Real(1000),_light->getAttenuationConstant(),Real(linear),_light->getAttenuationQuadric());
}

double OgreLight::getAttenuationQuadratic() const
{
	return _light->getAttenuationQuadric();
}
void OgreLight::setAttenuationQuadratic(double quadratic)
{
	_light->setAttenuation(Real(1000),_light->getAttenuationConstant(),_light->getAttenuationLinear(),Real(quadratic));
}

double OgreLight::getSpotCutOff() const
{
	return _light->getSpotlightFalloff();
}
void OgreLight::setSpotCutOff(double spot_cut_off)
{
}

double OgreLight::getSpotExponent() const
{
	return _light->getSpotlightFalloff();
}
void OgreLight::setSpotExponent(double spot_exponent)
{
}

const shared_ptr<VEHA::PointSpecification> OgreLight::getLocation() const
{
	return _location;
}
shared_ptr<VEHA::PointSpecification> OgreLight::accessLocation()
{
	return _location;
}
void OgreLight::setLocation(shared_ptr<VEHA::PointSpecification> loc)
{
	_location->getNode()->detachObject(_light);
	_location=shared_dynamic_cast<OgrePointSpecification>(loc);
	_location->getNode()->attachObject(_light);
}

}
