//----------------------------------------------------------------------------
// @(#) ENIB/LI2 (c) - AnimLib
//----------------------------------------------------------------------------
// package     : AnimLib
// file        : keyFrame.cpp => implementation of a single keyFrame
// description : Class describing a keyFrameTrack, a keyFrameTrackReal, keyFrameTrackReal3// author(s)   : CAZEAUX Eric
// mail        : cazeaux@enib.fr
//----------------------------------------------------------------------------
// last update : 01/12/2005
//----------------------------------------------------------------------------

#include "AnimLib/arKeyFrameTrack.h"
#include "AReVi/arClass.h"
#include "AReVi/activity.h"

#include "AnimLib/keyFrameTrack.h"

using namespace AReVi;

namespace AnimLib {

AR_CLASS_DEF(ArKeyFrameTrack,ArObject)

//----------------------------------------------------------------------------
// CLASS       : ArKeyFrameTrack
//----------------------------------------------------------------------------

//-------- Construction / Destruction ----------------------------------------

ArKeyFrameTrack::ArKeyFrameTrack(ArCW & arCW) // Protected
  : ArObject(arCW),
  _track(NULL),
  _position(0.0),
  _repeat(false),
  _animator(),
  _updateCB(thisPtr()),
  _seekCB(thisPtr()),
  _commandCB(thisPtr())
{
// Nothing to be done here!
}

ArKeyFrameTrack::~ArKeyFrameTrack(void)
{
if (_animator.valid())
  {
  _animator.destroy();
  }
  
if (_track)
  {
  _track->_setTrackManager(NULL);
  delete(_track);
  _track=NULL;
  }
}
  
//---------------------------- Public methods --------------------------------


//--------------------------- Track Player -----------------------------------
// Note that track animation can be performed by user (setPosition) or auto
// by start/pause commands

// Precision specifiy periode between 2 steps of animation
bool // success
ArKeyFrameTrack::start(double precision)
{
if (_animator.valid())
  {
  _animator->setInterval(precision);
  _animator->setSuspended(false);
  }
else
  {
  _animator=Activity::NEW(precision);
  _animator->setBehavior(thisRef(),&ArKeyFrameTrack::_onAnimate);
  }
  
_onCommand(TRACK_PLAY);
return(true);
}

bool // success
ArKeyFrameTrack::pause(void)
{
if (_animator.valid())
  {
  _onCommand(TRACK_PAUSE);
  _animator->setSuspended(true);
  return(true);
  }
else
  return(false);
}

// Return animation status
bool // running
ArKeyFrameTrack::getAnimated(double &precisionOut) const
{
if (_animator.valid())
  {
  precisionOut=_animator->getInterval();
  return(_animator->isSuspended());
  }
else
  return(false);
}

    

double
ArKeyFrameTrack::getPosition(void) const
{
return(_position);
}


// Change position and fire compute for this position
// Note that if position>duration
//                 if repeat => newposition=position%duration
//               else fail.
bool // success
ArKeyFrameTrack::setPosition(double position)
{
if (position<0.0 || !_track) return(false);

double duration=_track->getDuration();

if (position<=duration)
  _position=position;
else if (_repeat)
  {
  _onCommand(TRACK_LOOP);
  int nbLoop=(int)floor(position/duration);
  
  _position=position-(nbLoop*duration);  
  }
else
  {
  _onCommand(TRACK_STOP);
  return(false);
  }
  
if(!_seekCB.empty())
  {
  ArKeyFrameTrack::SeekEvent evt;
  evt.position=position;
  _seekCB.fire(evt);
  }  
return(true);
}

double // -1 if fail
ArKeyFrameTrack::getDuration(void) const
{
if (_track) return(_track->getDuration());
else return(-1);
}

void
ArKeyFrameTrack::setRepeat(bool repeat)
{
_repeat=repeat;
}

bool
ArKeyFrameTrack::getRepeat(void) const
{
return(_repeat);
}

//--------------------------- Track accessors ---------------------------------
    
const KeyFrameTrack *
ArKeyFrameTrack::getTrack(void) const
{
return(_track);
}

KeyFrameTrack *
ArKeyFrameTrack::accessTrack(void)
{
return(_track);
}


//------------------------- Forbiden operations ------------------------------  

void
ArKeyFrameTrack::_onNotifyUpdate(void)
{
if(!_updateCB.empty())
  {
  ArKeyFrameTrack::UpdateEvent evt;
  _updateCB.fire(evt);
  }
}

void
ArKeyFrameTrack::_onNotifyDestroy(void)
{
_track=NULL;
}

void
ArKeyFrameTrack::_onCommand(TrackCommand cmd)
{
if(!_commandCB.empty())
  {
  ArKeyFrameTrack::CommandEvent evt;
  evt.command=cmd;
  _commandCB.fire(evt);
  }
}

// Save/Load to XML
void
ArKeyFrameTrack::_writeXML(ArRef<XmlNode> node) const
{
if (_track)
  _track->_writeXML(node);
}

bool
ArKeyFrameTrack::_readXML(ArRef<XmlNode> node)
{
if (_track)
  return(_track->_readXML(node));
else
  return(false);
}


    
//--------------------------- Track factory ----------------------------------
    
// ArKeyFrameTrack will manage the keyFrameTrack object (and destroy it)
bool // success
ArKeyFrameTrack::_initTrack(KeyFrameTrack *track)
{
if ((_track==NULL) && track->_setTrackManager(thisRef().c_ptr()))
  {
  _track=track;
  return(true);
  }
return(false);
}

bool // virtual
ArKeyFrameTrack::_onAnimate(ArRef<Activity> , double dt)
{
return(setPosition(_position+dt));
}
    
} // end of namespace AnimLib

//---End-Of-File---------------------------------------------------------------
