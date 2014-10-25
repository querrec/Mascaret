/*
 * AReViLight.cpp
 *
 *  Created on: 25 févr. 2011
 *      Author: soler
 */

#include "VRPlateform/AReViLight.h"
using namespace AReVi;
using VEHA::Color;

namespace AReViMascaret
{
AReViLight::AReViLight() : _light(Light3D::NEW()),_location(new AReViPointSpecification())
{
	_light->attachTo(_location->getPoint());
	_light->setSwitch(true);
}
AReViLight::~AReViLight()
{

}

VEHA::Color AReViLight::getColor() const
{
	double r,g,b;
	_light->getColor(r,g,b);
	return Color(r,g,b);
}
void AReViLight::setColor(const VEHA::Color& color)
{
	_light->setColor(color.r,color.g,color.b);
}

bool AReViLight::getDirectional() const
{
	return _light->getDirectional();
}
void AReViLight::setDirectional(bool directional)
{
	_light->setDirectional(directional);
}

double AReViLight::getAttenuationConstant() const
{
	double constante,linear,quadratic;
	_light->getAttenuation(constante,linear,quadratic);
	return constante;
}
void AReViLight::setAttenuationConstant(double constant)
{
	double constant2,linear,quadratic;
	_light->getAttenuation(constant2,linear,quadratic);
	_light->setAttenuation(constant,linear,quadratic);
}

double AReViLight::getAttenuationLinear() const
{
	double constant,linear,quadratic;
	_light->getAttenuation(constant,linear,quadratic);
	return linear;
}
void AReViLight::setAttenuationLinear(double linear)
{
	double constant,linear2,quadratic;
	_light->getAttenuation(constant,linear2,quadratic);
	_light->setAttenuation(constant,linear,quadratic);
}

double AReViLight::getAttenuationQuadratic() const
{
	double constant,linear,quadratic;
	_light->getAttenuation(constant,linear,quadratic);
	return quadratic;
}
void AReViLight::setAttenuationQuadratic(double quadratic)
{
	double constant,linear,quadratic2;
	_light->getAttenuation(constant,linear,quadratic2);
	_light->setAttenuation(constant,linear,quadratic);
}

double AReViLight::getSpotCutOff() const
{
	double cutOff,exponent;
	_light->getSpotParameters(exponent,cutOff);
	return cutOff;
}
void AReViLight::setSpotCutOff(double spot_cut_off)
{
	double cutOff,exponent;
	_light->getSpotParameters(exponent,cutOff);
	_light->setSpotParameters(exponent,spot_cut_off);
}

double AReViLight::getSpotExponent() const
{
	double cutOff,exponent;
	_light->getSpotParameters(exponent,cutOff);
	return exponent;
}
void AReViLight::setSpotExponent(double spot_exponent)
{
	double cutOff,exponent;
	_light->getSpotParameters(exponent,cutOff);
	_light->setSpotParameters(spot_exponent,cutOff);
}

const shared_ptr<VEHA::PointSpecification> AReViLight::getLocation() const
{
	return _location;
}
shared_ptr<VEHA::PointSpecification> AReViLight::accessLocation()
{
	return _location;
}
void AReViLight::setLocation(shared_ptr<VEHA::PointSpecification> loc)
{
	shared_ptr<AReViPointSpecification> p=shared_dynamic_cast<AReViPointSpecification>(loc);
	if(p)
	{
		_light->attachTo(_location->_point,Base3D::ATTACH_NONE);
		_location=p;
		_light->attachTo(_location->_point);
	}
}

}
