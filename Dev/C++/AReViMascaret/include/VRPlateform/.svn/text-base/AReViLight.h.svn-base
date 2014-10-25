/*
 * AReViLight.h
 *
 *  Created on: 24 févr. 2011
 *      Author: soler
 */

#ifndef AREVILIGHT_H_
#define AREVILIGHT_H_
#include "AReVi/Lib3D/light3D.h"

#include "areviMascaret_plateform.h"
#include "IEHA/Visualisation/Light.h"
#include "VRPlateform/AReViPointSpecification.h"

namespace AReViMascaret
{
class AREVI_MASCARET_API AReViLight : public IEHA::Light
{
public:
	AReViLight();
	virtual ~AReViLight();

	virtual VEHA::Color getColor() const;
	virtual void setColor(const VEHA::Color& color);

	virtual bool getDirectional() const;
	virtual void setDirectional(bool directional);

	virtual double getAttenuationConstant() const;
	virtual void setAttenuationConstant(double constant);

	virtual double getAttenuationLinear() const;
	virtual void setAttenuationLinear(double linear);

	virtual double getAttenuationQuadratic() const;
	virtual void setAttenuationQuadratic(double quadratic);

	virtual double getSpotCutOff() const;
	virtual void setSpotCutOff(double spot_cut_off);

	virtual double getSpotExponent() const;
	virtual void setSpotExponent(double spot_exponent);

	virtual const shared_ptr<VEHA::PointSpecification> getLocation() const;
	virtual shared_ptr<VEHA::PointSpecification> accessLocation();
	virtual void setLocation(shared_ptr<VEHA::PointSpecification> loc);


	AReVi::ArRef<AReVi::Light3D> getLight();
protected:
	AReVi::ArRef<AReVi::Light3D> _light;
	shared_ptr<AReViPointSpecification> _location;
};
inline AReVi::ArRef<AReVi::Light3D> AReViLight::getLight()
{
	return _light;
}
}
#endif /* AREVILIGHT_H_ */
