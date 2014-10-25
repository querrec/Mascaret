/*
 * AReViVRComponentFactory.h
 *
 *  Created on: 24 févr. 2011
 *      Author: soler
 */

#ifndef AREVIVRCOMPONENTFACTORY_H_
#define AREVIVRCOMPONENTFACTORY_H_
#include "areviMascaret_plateform.h"
#include "AReVi/arObject.h"
#include "VEHA/Entity/VirtualRealityComponentFactory.h"

namespace AReViMascaret
{
class AREVI_MASCARET_API AReViVRComponentFactory : public VEHA::VirtualRealityComponentFactory
{
public:
	virtual shared_ptr<VEHA::SoundSpecification> createSound(const string& name, const string url, double gain = 1.0, double minDistance = 1.0, double maxDistance = 100.0, bool cycle=false);
	virtual shared_ptr<VEHA::AnimationSpecification> createAnimation(const string& name, const string url, double speed = 1.0, bool cycle=false);
	virtual shared_ptr<VEHA::ShapeSpecification> createShape(const string& name, const string & url, bool movable=true,bool recursive =false,const string& shader="");
	virtual shared_ptr<VEHA::ShapeSpecification> createShape(const string& name, const vector<double>& distances,const vector<string> & urls, bool movable=true,bool recursive = false,const string& shader="");
	virtual shared_ptr<VEHA::PointSpecification> createPoint(VEHA::Vector3 pos=VEHA::Vector3(), VEHA::RotationVector rotation=VEHA::RotationVector());
	virtual shared_ptr<VEHA::Color> createColor(int r, int g, int b);
	virtual shared_ptr<VEHA::Color> createColor(float a);

	virtual shared_ptr<IEHA::Light> createLight();
	virtual shared_ptr<IEHA::Camera> createCamera(shared_ptr<IEHA::Window3D> window);
    virtual shared_ptr<IEHA::RenderToTexture> createRenderToTexture();
	virtual shared_ptr<IEHA::Window3D> createWindow3D();
	virtual shared_ptr<IEHA::Scene> createScene(const string& name);
};
}
#endif /* AREVIVRCOMPONENTFACTORY_H_ */
