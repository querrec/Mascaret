#ifndef OBJECTANIMATION_H
#define OBJECTANIMATION_H
#include "areviMascaret_plateform.h"
#include "AnimLib/arKeyFrameTrack.h"
#include "Animation/AReViAnimation.h"


namespace AReViMascaret
{
using AnimLib::ArKeyFrameTrack;
class AREVI_MASCARET_API ObjectAnimation : public AReViAnimation
{
public :
  AR_CLASS(ObjectAnimation)
  AR_CONSTRUCTOR(ObjectAnimation)

  void addTrack(ArRef<ArKeyFrameTrack> track);
  virtual void setPosition(double position);
  virtual double getDuration(void);

protected :
  StlVector< ArRef<ArKeyFrameTrack> > _tracks;
};
}
#endif
