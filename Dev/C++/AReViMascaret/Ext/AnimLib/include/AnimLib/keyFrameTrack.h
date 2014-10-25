//----------------------------------------------------------------------------
// @(#) ENIB/LI2 (c) - AnimLib
//----------------------------------------------------------------------------
// package     : AnimLib
// file        : keyFrame.h => declaration of a single keyFrame
// description : Class describing a keyFrameTrack, a keyFrameTrackReal, keyFrameTrackReal3
// author(s)   : CAZEAUX Eric
// mail        : cazeaux@enib.fr
//----------------------------------------------------------------------------
// last update : 30/11/2005
//----------------------------------------------------------------------------
#ifndef ANIMLIB_KEYFRAMETRACK_H
#define ANIMLIB_KEYFRAMETRACK_H 1

//--- Imported classes -------------------------------------------------------

#include "AnimLib/animLib_plateform.h"
#include "AReVi/Utils/stlVector_priv.h"
#include "AReVi/arObject.h"


namespace AReVi
{
class XmlNode;
}

using namespace AReVi;

namespace AnimLib {

class ArKeyFrameTrack;
class AnimLibUtils;

//----------------------------------------------------------------------------
// CLASS       : KeyFrameTrack {abstract}
//----------------------------------------------------------------------------
/*! \class KeyFrameTrack
  * \author Eric Cazeaux
  * \date 25-11-2005
  *
  * Class describing a single track (abstract). This class implements methods
  * for inspecting keyFrameTrack attributes duration and type and read-only property.
  */

class ANIMLIB_API KeyFrameTrack
{
public:
  KeyFrameTrack(void);
  virtual ~KeyFrameTrack(void);
  
//---------------------------- Public methods --------------------------------
public:

//--------------------------- Inspect variable -------------------------------
    
  //! Duration of animation (duration between first and last key)
    virtual double
    getDuration(void) const;
  
    virtual void
    setDuration(double duration);
  
    //! Degree attribute inspectors (valid values : 1 (LINEAR), > 2 
    virtual unsigned
    getDegree(void) const;
    
    virtual void
    setDegree(unsigned degree);
  
//--------------------------- KeyFrames manip -------------------------------
  
  //! Nb Key Frames
    virtual unsigned
    getNbKeyFrames(void) const=0;

//! Get key Frames positions
    virtual void
    getKeysPositions(StlVector<unsigned> &keysPositionOut) const=0;
    
  //! Abstract methods to remove and add key frames
    
    virtual bool // success
    addKeyFrame(unsigned position) = 0;
    
    virtual bool // success
    removeKeyFrame(unsigned position) = 0;
  
    virtual void
    clearKeyFrames(void) = 0;
  
//--------------------------- Force rebuild ---------------------------------

    virtual bool //success
    rebuild(void)=0;    
//------------------------- Forbiden operations ------------------------------  
protected:
  
  //! Notification from specializations
    void
    _needRebuild(void);

  //! write/read to XML
    virtual
    void
    _writeXML(ArRef<XmlNode> node) const;
    
    virtual
    bool
    _readXML(ArRef<XmlNode> node);
    
//----------------------------- Attributes -----------------------------------
protected:
  double _duration;
  unsigned _degree;
  bool _needRebuildTrack;
  
private :
  friend class ArKeyFrameTrack;
  friend class AnimLibUtils;
  
  bool //fail if already managed
  _setTrackManager(ArKeyFrameTrack *arTrack);
  
  ArKeyFrameTrack * _trackManager;
};

} // end of namespace AnimLib

//-------- Inline source code ------------------------------------------------



#endif // end of ANIMLIB_KEYFRAMETRACK_H
//---End-Of-File---------------------------------------------------------------
