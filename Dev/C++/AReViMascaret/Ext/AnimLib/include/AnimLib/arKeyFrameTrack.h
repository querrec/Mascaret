//----------------------------------------------------------------------------
// @(#) ENIB/LI2 (c) - AnimLib
//----------------------------------------------------------------------------
// package     : AnimLib
// file        : keyFrame.h => declaration of a single keyFrame
// description : Class managing a  keyFrameTrack
// author(s)   : CAZEAUX Eric
// mail        : cazeaux@enib.fr
//----------------------------------------------------------------------------
// last update : 01/12/2005
//----------------------------------------------------------------------------
#ifndef ANIMLIB_ARKEYFRAMETRACK_H
#define ANIMLIB_ARKEYFRAMETRACK_H 1

//--- Imported classes -------------------------------------------------------
#include "AnimLib/animLib_plateform.h"
#include "AReVi/arObject.h"

namespace AReVi
{
class Activity;
class XmlNode;
}

using namespace AReVi;

namespace AnimLib {

class KeyFrameTrack;
class AnimLibUtils;
//----------------------------------------------------------------------------
// CLASS       : ArKeyFrameTrack
//----------------------------------------------------------------------------
/*! \class ArKeyFrameTrack
  * \author Eric Cazeaux
  * \date 01-12-2005
  *
  * Class managing a single track. 
  */

class ANIMLIB_API ArKeyFrameTrack : public ArObject
{
public :
    enum TrackCommand  { TRACK_PLAY,TRACK_PAUSE, TRACK_STOP, TRACK_LOOP};

          
//-------- Type information --------------------------------------------------
public:

  AR_CLASS(ArKeyFrameTrack)

//-------- Construction / Destruction ----------------------------------------

  AR_CONSTRUCTOR(ArKeyFrameTrack)
  
//---------------------------- Public methods --------------------------------
public:

    virtual inline void saveToXML(ArRef<XmlNode> root) {_writeXML(root);}
//--------------------------- Track Player -----------------------------------
// Note that track animation can be performed by user (setPosition) or auto
// by start/pause commands

    //! Precision specifiy periode between 2 steps of animation
    bool // success
    start(double precision);
    
    bool // success
    pause(void);
    
    //! Return animation status
    bool // running
    getAnimated(double &precisionOut) const;
    
    double
    getPosition(void) const;
    
    // Change position and fire compute for this position
    // Note that if position>duration
    //                 if repeat => newposition=position%duration
    //               else fail.
    virtual bool // success
    setPosition(double position);
    
    double  // -1 if fail
    getDuration(void) const;
    
    void
    setRepeat(bool repeat);
    
    bool
    getRepeat(void) const;
        
//--------------------------- Track accessors ---------------------------------
    
    const KeyFrameTrack *
    getTrack(void) const;
  
    KeyFrameTrack *
    accessTrack(void);

//-------- Event management --------------------------------------------------
    
    AR_CALLBACK(ArKeyFrameTrack,Update,_updateCB,/*no fields*/)
    
    AR_CALLBACK(ArKeyFrameTrack,Seek,_seekCB,double position;) // position if absolute position (passed on setPosition)
    
        AR_CALLBACK(ArKeyFrameTrack,Command,_commandCB,TrackCommand command;)
    
//------------------------- Forbiden operations ------------------------------  
protected:

    friend class AnimLibUtils;
      
//--------------------------- Track factory ----------------------------------
    
    // ArKeyFrameTrack will manage the keyFrameTrack object (and destroy it)
    bool // success
    _initTrack(KeyFrameTrack *track);
     
    void
    _onCommand(TrackCommand cmd);
    
//------------- KeyFrameTrack Notifications -----------------------------------
     
    friend class KeyFrameTrack;
    
    virtual void
    _onNotifyUpdate(void);
    
    virtual void
    _onNotifyDestroy(void);
    
    virtual bool
    _onAnimate(ArRef<Activity> act, double dt);
    
  //! write/read to XML
    virtual
    void
    _writeXML(ArRef<XmlNode> node) const;
    
    virtual
    bool
    _readXML(ArRef<XmlNode> node);
        
//--------------------------- Compute frame ---------------------------------
    
//     virtual bool
//     _computeFrame(double t)=0;
        
//----------------------------- Attributes -----------------------------------
protected:
  KeyFrameTrack  *_track;
  double _position;
  bool _repeat;
  ArPtr<Activity> _animator;
  
  CallbackManager<ArKeyFrameTrack,UpdateEvent> _updateCB;
  
  CallbackManager<ArKeyFrameTrack,SeekEvent> _seekCB;
  
  CallbackManager<ArKeyFrameTrack,CommandEvent> _commandCB;
};

} // end of namespace AnimLib

//-------- Inline source code ------------------------------------------------



#endif // end of ANIMLIB_ARKEYFRAMETRACK_H
//---End-Of-File---------------------------------------------------------------
