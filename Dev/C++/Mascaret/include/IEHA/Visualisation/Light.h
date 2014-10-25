/*
 * Light.h
 *
 *  Created on: 24 févr. 2011
 *      Author: soler
 */

#ifndef LIGHT_H_
#define LIGHT_H_
#include "Tools/veha_plateform.h"
#include "VEHA/Entity/Color.h"
namespace VEHA
{
class PointSpecification;
}
namespace IEHA
{
using VEHA::Color;
using VEHA::PointSpecification;
class VEHA_API Light
{
public:
	virtual Color getColor() const=0;
	virtual void setColor(const Color& color)=0;

	virtual bool getDirectional() const=0;
	virtual void setDirectional(bool directional)=0;

	virtual double getAttenuationConstant() const=0;
	virtual void setAttenuationConstant(double constant)=0;

	virtual double getAttenuationLinear() const=0;
	virtual void setAttenuationLinear(double linear)=0;

	virtual double getAttenuationQuadratic() const=0;
	virtual void setAttenuationQuadratic(double quadratic)=0;

	virtual double getSpotCutOff() const=0;
	virtual void setSpotCutOff(double spot_cut_off)=0;

	virtual double getSpotExponent() const=0;
	virtual void setSpotExponent(double spot_exponent)=0;

	virtual const shared_ptr<PointSpecification> getLocation() const=0;
	virtual shared_ptr<PointSpecification> accessLocation()=0;
	virtual void setLocation(shared_ptr<PointSpecification> loc)=0;

};
}


#endif /* LIGHT_H_ */
