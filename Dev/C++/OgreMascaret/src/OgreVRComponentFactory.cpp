/*
 * OgreVRComponentFactory.cpp
 *
 *  Created on: 24 févr. 2011
 *      Author: soler
 */
#include "OgreVRComponentFactory.h"
#include "VRPlateform/OgrePointSpecification.h"
#include "VRPlateform/OgreSoundSpecification.h"
#include "VRPlateform/OgreAnimationSpecification.h"
#include "VRPlateform/OgreShapeSpecification.h"
#include "VRPlateform/OgreLight.h"
#include "VRPlateform/OgreCamera.h"
#include "VRPlateform/OgreWindow3D.h"
#include "VRPlateform/OgreRenderToTexture.h"
#include "VRPlateform/OgreScene.h"
#include "Assistance/OgreColor.h"

using namespace IEHA;
using namespace VEHA;

namespace OgreMascaret
{
shared_ptr<SoundSpecification> OgreVRComponentFactory::createSound(const string& name, const string url, double gain, double minDistance, double maxDistance, bool cycle)
{
	return shared_ptr<SoundSpecification>(new OgreSoundSpecification(name,url,gain,minDistance,maxDistance,cycle));
}
shared_ptr<AnimationSpecification> OgreVRComponentFactory::createAnimation(const string& name, const string url, double speed, bool cycle)
{
	return shared_ptr<AnimationSpecification>(new OgreAnimationSpecification(name,url,speed,cycle));
}
shared_ptr<ShapeSpecification> OgreVRComponentFactory::createShape(const string& name, const string & url, bool movable,bool recursive,const string& shader)
{
	return shared_ptr<ShapeSpecification>(new OgreShapeSpecification(name,url,movable,recursive,shader));
}
shared_ptr<ShapeSpecification> OgreVRComponentFactory::createShape(const string& name, const vector<double>& distances,const vector<string> & urls, bool movable,bool recursive,const string& shader)
{
	return shared_ptr<ShapeSpecification>(new OgreShapeSpecification(name,distances,urls,movable,recursive,shader));
}
shared_ptr<PointSpecification> OgreVRComponentFactory::createPoint(Vector3 pos, RotationVector rotation)
{
	return shared_ptr<PointSpecification>(new OgrePointSpecification(pos,rotation));
}

shared_ptr<Light> OgreVRComponentFactory::createLight()
{
	return shared_ptr<Light>(new OgreLight());
}
shared_ptr<Camera> OgreVRComponentFactory::createCamera(shared_ptr<Window3D> window)
{
	return shared_ptr<Camera>(new OgreCamera(window));
}
shared_ptr<RenderToTexture> OgreVRComponentFactory::createRenderToTexture()
{
  return shared_ptr<RenderToTexture>(new OgreRenderToTexture());
}
shared_ptr<Window3D> OgreVRComponentFactory::createWindow3D()
{
	return shared_ptr<Window3D>(new OgreWindow3D());
}
shared_ptr<Scene> OgreVRComponentFactory::createScene(const string& name)
{
	return shared_ptr<Scene>(new OgreScene(name));
}

shared_ptr<VEHA::Color> OgreVRComponentFactory::createColor(int r,int g,int b)
{
	return shared_ptr<VEHA::Color>(new OgreColor(r,g,b));
}

shared_ptr<VEHA::Color> OgreVRComponentFactory::createColor(float a)
{
	return shared_ptr<VEHA::Color>(new OgreColor(a));
}

}
