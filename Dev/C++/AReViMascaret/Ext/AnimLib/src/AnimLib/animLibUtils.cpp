//----------------------------------------------------------------------------
// @(#) ENIB/LI2 (c) - AnimLib
//----------------------------------------------------------------------------
// package     : AnimLib
// file        : keyFrame.cpp => implementation of a single keyFrame
// description : Class describing a keyFrameTrack, a keyFrameTrackReal, keyFrameTrackReal3// author(s)   : CAZEAUX Eric
// mail        : cazeaux@enib.fr
//----------------------------------------------------------------------------
// last update : 25/11/2005
//----------------------------------------------------------------------------

#include "AnimLib/keyFrameTrack.h"
#include "AnimLib/arKeyFrameTrack.h"

using namespace AReVi;

namespace AnimLib {

//----------------------------------------------------------------------------
// CLASS       : KeyFrameTrack
//----------------------------------------------------------------------------
KeyFrameTrack::KeyFrameTrack(void) : 
  _duration(1.0),
  _degree(1),
  _needRebuildTrack(true),
  _trackManager(NULL)
{
// Nothing to be done here!
}

KeyFrameTrack::~KeyFrameTrack(void)
{
if (_trackManager)
  _trackManager->_onNotifyDestroy();
//else
  // Nothing to be done here!
}
  
//---------------------------- Public methods --------------------------------


//--------------------------- Inspect variable -------------------------------


// Duration of animation (duration between first and last key)
double
KeyFrameTrack::getDuration(void) const
{
return(_duration);
}

void
KeyFrameTrack::setDuration(double duration)
{
if (duration<0.0) duration=0.0;
_duration=duration;
_needRebuild();
}

// Degree attribute inspectors (valid values : 1 (LINEAR), > 2 
unsigned
KeyFrameTrack::getDegree(void) const
{
return(_degree);
}

void
KeyFrameTrack::setDegree(unsigned degree)
{
_degree=degree;
_needRebuild();
}


//------------------------- Forbiden operations ------------------------------  


bool //fail if already managed
KeyFrameTrack::_setTrackManager(ArKeyFrameTrack *arTrack)
{
if (arTrack && _trackManager) // if arTRack is NULL, then unreference manager
  {
  return(false);
  }
  
_trackManager=arTrack;
return(true);
}  


// Notification from childs
  
void
KeyFrameTrack::_needRebuild(void)
{
_needRebuildTrack=true;
if (_trackManager)
  _trackManager->_onNotifyUpdate();
}

    
    
} // end of namespace AnimLib

//---End-Of-File---------------------------------------------------------------
