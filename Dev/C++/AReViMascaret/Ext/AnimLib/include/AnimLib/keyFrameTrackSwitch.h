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
#ifndef ANIMLIB_KEYFRAMETRACKSWITCH_H
#define ANIMLIB_KEYFRAMETRACKSWITCH_H 1

//--- Imported classes -------------------------------------------------------
#include "AReVi/Utils/stlMap_priv.h"
#include "AnimLib/animLib_plateform.h"
#include "AnimLib/keyFrameTrack.h"
#include "AReVi/Contrib/xmlParser.h"


namespace AnimLib {

//----------------------------------------------------------------------------
// CLASS       : KeyFrameTrackSwitch
//----------------------------------------------------------------------------
/*! \class KeyFrameTrackSwitch
  * \author Eric Cazeaux
  * \date 13-12-2005
  *
  * Class describing a single KeyFrameTrackSwitch template. This class implements methods
  * for inspecting keyFrameTrack attributes duration and type and read-only property.
  */
// T is the type of data
template <class T>
class KeyFrameTrackSwitch : public KeyFrameTrack
{
public:
    
  KeyFrameTrackSwitch(void);
  virtual ~KeyFrameTrackSwitch(void);
  
//---------------------------- Public methods --------------------------------
public:

//--------------------------- Evaluate a point -------------------------------
  
  //! Compute a key frame at time t in [0,duration]
  inline bool // success
  computeFrame(double t, T &keyFrameOut);
  
  //! Compute a key frame at position pos in [firstPost,lastPos]
  inline bool // success
  computePosition(unsigned pos, T &keyFrameOut);
  
//--------------------------- Inspect variable -------------------------------
  
  //! Inspect list of keyFrames
  inline const StlMap<unsigned, T> &
  getKeyFrames(void) const;

//--------------------------- KeyFrames manip -------------------------------
  
  //! Add Key Frame
  inline void // if a key at position "position" already exist, otherwrite
  setKeyFrame(unsigned position, const T &key);

  //! Get Key Frame
  inline bool // success
  getKeyFrame(unsigned position, T &keyOut) const;

    
  //! Nb Key Frames
  virtual unsigned
  getNbKeyFrames(void) const;
  
  //! Get key Frames positions
  virtual void
  getKeysPositions(StlVector<unsigned> &keysPositionOut) const;
    
  //! Abstract methods to remove and add key frames
    
  virtual bool // success
  addKeyFrame(unsigned position);
    
  virtual bool // success
  removeKeyFrame(unsigned position);
  
  virtual void
  clearKeyFrames(void);
  
//--------------------------- Force rebuild ---------------------------------

  virtual bool //success
  rebuild(void);
      
//------------------------- Forbiden operations ------------------------------  
protected:

  //! Save/Load to XML
  virtual
  void
  _writeXML(ArRef<XmlNode> node) const;
  
  virtual
  bool
  _readXML(ArRef<XmlNode> node);
  
//----------------------------- Attributes -----------------------------------
protected:
  StlMap<unsigned, T> _keyFrames;
  
  typedef typename StlMap<unsigned, T >::iterator KeyFrameIterator;
  typedef typename StlMap<unsigned, T >::const_iterator KeyFrameConstIterator;
};

} // end of namespace AnimLib

//-------- Inline source code ------------------------------------------------

#include "keyFrameTrackSwitch.Ci"


#endif // end of ANIMLIB_KEYFRAMETRACKSWITCH_H
//---End-Of-File---------------------------------------------------------------
