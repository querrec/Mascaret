/*
 * AReViVRComponentFactory.cpp
 *
 *  Created on: 24 févr. 2011
 *      Author: soler
 */
#include "AReViVRComponentFactory.h"
#include "Assistance/AReViColor.h"
#include "VRPlateform/AReViPointSpecification.h"
#include "VRPlateform/AReViSoundSpecification.h"
#include "VRPlateform/AReViAnimationSpecification.h"
#include "VRPlateform/AReViShapeSpecification.h"
#include "VRPlateform/AReViLight.h"
#include "VRPlateform/AReViCamera.h"
#include "VRPlateform/AReViWindow3D.h"
#include "VRPlateform/AReViScene.h"

using namespace IEHA;
using namespace VEHA;

namespace AReViMascaret
{
	using boost::make_shared;
shared_ptr<SoundSpecification> AReViVRComponentFactory::createSound(const string& name, const string url, double gain, double minDistance, double maxDistance, bool cycle)
{
	return make_shared<AReViSoundSpecification>(name,url,gain,minDistance,maxDistance,cycle);
}
shared_ptr<AnimationSpecification> AReViVRComponentFactory::createAnimation(const string& name, const string url, double speed, bool cycle)
{
	return make_shared<AReViAnimationSpecification>(name,url,speed,cycle);
}
shared_ptr<ShapeSpecification> AReViVRComponentFactory::createShape(const string& name, const string & url, bool movable,bool recursive,const string& shader)
{
	return make_shared<AReViShapeSpecification>(name,url,movable,recursive,shader);
}
shared_ptr<ShapeSpecification> AReViVRComponentFactory::createShape(const string& name, const vector<double>& distances,const vector<string> & urls, bool movable,bool recursive,const string& shader)
{
	return make_shared<AReViShapeSpecification>(name,distances,urls,movable,recursive,shader);
}
shared_ptr<PointSpecification> AReViVRComponentFactory::createPoint(Vector3 pos, RotationVector rotation)
{
	return make_shared<AReViPointSpecification>(pos,rotation);
}

shared_ptr<Light> AReViVRComponentFactory::createLight()
{
	return make_shared<AReViLight>();
}
shared_ptr<Camera> AReViVRComponentFactory::createCamera(shared_ptr<IEHA::Window3D> window)
{
	return make_shared<AReViCamera>(window);
}
shared_ptr<RenderToTexture> AReViVRComponentFactory::createRenderToTexture()
{
//    return shared_ptr<RenderToTexture>(new AReViRenderToTexture());
//    TODO
    return shared_ptr<RenderToTexture>();
}
shared_ptr<IEHA::Window3D> AReViVRComponentFactory::createWindow3D()
{
	return make_shared<AReViWindow3D>();
}
shared_ptr<Scene> AReViVRComponentFactory::createScene(const string& name)
{
	return make_shared<AReViScene>(name);
}
shared_ptr<Color> AReViVRComponentFactory::createColor(int r, int g, int b)
{
	return make_shared<AReViColor>(r,g,b);
}

shared_ptr<Color> AReViVRComponentFactory::createColor(float a)
{
	return make_shared<AReViColor>(a);
}
}
