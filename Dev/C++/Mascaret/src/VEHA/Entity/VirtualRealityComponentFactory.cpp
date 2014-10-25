/*
 * VirtualRealityComponentFactory.cpp
 *
 *  Created on: 28 févr. 2011
 *      Author: soler
 */
#include "VEHA/Entity/VirtualRealityComponentFactory.h"
namespace VEHA
{

shared_ptr<SoundSpecification> VirtualRealityComponentFactory::createSoundFromString(const string& str)
{
	return shared_ptr<SoundSpecification>();
}
shared_ptr<AnimationSpecification> VirtualRealityComponentFactory::createAnimationFromString(const string& str)
{
	return shared_ptr<AnimationSpecification>();
}
shared_ptr<ShapeSpecification> VirtualRealityComponentFactory::createShapeFromString(const string& str)
{
	return shared_ptr<ShapeSpecification>();
}
shared_ptr<PointSpecification> VirtualRealityComponentFactory::createPointFromString(const string& str)
{
	return shared_ptr<PointSpecification>();
}

}
