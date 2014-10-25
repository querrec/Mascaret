#ifndef HLIBANIMATION_H
#define HLIBANIMATION_H
#include "areviMascaret_plateform.h"
#include "Animation/AReViAnimation.h"
#include "AReVi/activity.h"
#include "AReVi/VRML/vrmlShape3D.h"
#include "AReVi/Lib3D/transform3D.h"
#include "hLib/hLib.h"
#include "boost/filesystem.hpp"

namespace AReViMascaret
{
using hLib::Animation;
using AReVi::ArRef;

class AREVI_MASCARET_API HLibAnimation : public AReViAnimation
{
public :
  AR_CLASS(HLibAnimation)
  AR_CONSTRUCTOR(HLibAnimation)

  void applyPose(ArConstRef<Keyframe> kf); 
  virtual void setPosition(double position){applyPose(_animation->evaluate(position));}
  virtual double getDuration(void){ return _animation->getDuration();}

  void setHLibAnimation(ArRef<hLib::Animation> animation) {_animation = animation;}
  ArRef<hLib::Animation> getHLibAnimation(void) {return _animation;}

private :
  ArRef<hLib::Animation> _animation;
};
}
#endif
