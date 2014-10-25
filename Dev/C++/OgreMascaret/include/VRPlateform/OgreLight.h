/*
 * AReViLight.h
 *
 *  Created on: 24 févr. 2011
 *      Author: soler
 */

#ifndef OGRELIGHT_H_
#define OGRELIGHT_H_
#include "ogreMascaret_plateform.h"
#include <Ogre.h>
#include "IEHA/Visualisation/Light.h"
#include "VRPlateform/OgrePointSpecification.h"

namespace OgreMascaret
{
class OGRE_MASCARET_API OgreLight : public IEHA::Light
{
public:
	OgreLight();
	virtual ~OgreLight();

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
protected:
	friend class OgreScene;
	Ogre::Light* _light;
	static unsigned int _id;
	shared_ptr<OgrePointSpecification> _location;
};
}
#endif /* AREVILIGHT_H_ */
