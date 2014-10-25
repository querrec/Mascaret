/*
 * VirtualRealityComponentFactory.h
 *
 *  Created on: 23 févr. 2011
 *      Author: soler
 */

#ifndef VIRTUALREALITYCOMPONENTFACTORY_H_
#define VIRTUALREALITYCOMPONENTFACTORY_H_
#include "Tools/veha_plateform.h"
#include "VEHA/Entity/Vector3.h"
#include "VEHA/Entity/RotationVector.h"

namespace IEHA
{
class Light;
class Camera;
class Scene;
class Window3D;
class RenderToTexture;
}

namespace VEHA
{
class SoundSpecification;
class AnimationSpecification;
class ShapeSpecification;
class PointSpecification;
class Color;

class VEHA_API VirtualRealityComponentFactory
{
public:
	virtual shared_ptr<SoundSpecification> createSoundFromString(const string& str);
	virtual shared_ptr<SoundSpecification> createSound(const string& name, const string url, double gain = 1.0, double minDistance = 1.0, double maxDistance = 100.0, bool cycle=false)=0;
	virtual shared_ptr<AnimationSpecification> createAnimationFromString(const string& str);
	virtual shared_ptr<AnimationSpecification> createAnimation(const string& name, const string url, double speed = 1.0, bool cycle=false)=0;
	virtual shared_ptr<ShapeSpecification> createShapeFromString(const string& str);
	virtual shared_ptr<ShapeSpecification> createShape(const string& name, const string & url, bool movable=true,bool recursive =false,const string& shader="")=0;
	virtual shared_ptr<ShapeSpecification> createShape(const string& name, const vector<double>& distances,const vector<string> & urls, bool movable=true,bool recursive = false,const string& shader="")=0;
	virtual shared_ptr<PointSpecification> createPointFromString(const string& str);
	virtual shared_ptr<PointSpecification> createPoint(Vector3 pos=Vector3(), RotationVector rotation=RotationVector())=0;
	virtual shared_ptr<Color> createColor(int r,int g,int b)=0;
	virtual shared_ptr<Color> createColor(float a)=0;

	virtual shared_ptr<IEHA::Light> createLight()=0;
	virtual shared_ptr<IEHA::Scene> createScene(const string& name)=0;
	virtual shared_ptr<IEHA::Camera> createCamera(shared_ptr<IEHA::Window3D> window)=0;
	virtual shared_ptr<IEHA::RenderToTexture> createRenderToTexture()=0;
	virtual shared_ptr<IEHA::Window3D> createWindow3D()=0;

};

}

#endif /* VIRTUALREALITYCOMPONENTFACTORY_H_ */
