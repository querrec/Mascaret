/*
 * OgreAnimationSpecification.cpp
 *
 *  Created on: 25 févr. 2011
 *      Author: soler
 */

#include "Tools/filesystem.h"

#include "VRPlateform/OgreAnimationSpecification.h"
#include "VRPlateform/OgreMascaretApplication.h"
#include "VEHA/Behavior/Common/behaviorScheduler.h"

DECLARE_NAMESPACE_FILESYSTEM;
using namespace VEHA;

namespace OgreMascaret
{
OgreAnimationSpecification::OgreAnimationSpecification(const string & name, const string & url, int sens, double speed, bool cycle)
: AnimationSpecification(name,url), _time(0.0), _sens(sens), _speed(speed), _cycle(cycle)
{
    fs::path path1(url);
#if BOOST_FILESYSTEM_VERSION == 2
    string filename = path1.filename();
#else
    string filename = path1.filename().string();
#endif
    
    if (filename == "") filename = name;

    _ogreAnimName = filename;
    _anim = NULL;
}

OgreAnimationSpecification::~OgreAnimationSpecification()
{

}

void OgreAnimationSpecification::setShape(shared_ptr<ShapeSpecification> shape)
{
	_shape=shared_dynamic_cast<OgreShapeSpecification>(shape);
        _anim = _shape->getObject3D()->getAnimationState(_ogreAnimName);
         

}
shared_ptr<ShapeSpecification> OgreAnimationSpecification::getShape() const
{
	return _shape;
}
bool OgreAnimationSpecification::play(double /*speed*/, int /*sens*/, bool /*cycle*/)
{
        OgreMascaretApplication::getInstance()->getRoot()->addFrameListener(this);
        _anim->setEnabled(true);
	return true;
}
bool OgreAnimationSpecification::stop()
{
        OgreMascaretApplication::getInstance()->getRoot()->removeFrameListener(this);
        _anim->setEnabled(false);
	return true;
}
void OgreAnimationSpecification::_setPosition(double position)
{
}
bool OgreAnimationSpecification::frameEnded(const FrameEvent& evt)
{
    bool suspended = VEHA::BehaviorScheduler::getInstance()->isSuspended();
        
    if (_anim && _anim->getEnabled() && !suspended)
    {
        double sim_speed_factor = VEHA::BehaviorScheduler::getInstance()->getAccelerationFactor();
        _anim->addTime(evt.timeSinceLastFrame * _speed * sim_speed_factor);
    }
    
    return true;
}
}
