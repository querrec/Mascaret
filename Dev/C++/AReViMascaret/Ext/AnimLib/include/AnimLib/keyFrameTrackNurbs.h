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
#ifndef ANIMLIB_KEYFRAMETRACKNURBS_H
#define ANIMLIB_KEYFRAMETRACKNURBS_H 1

//--- Imported classes -------------------------------------------------------
#include "AReVi/Utils/stlMap_priv.h"
#include "AReVi/Contrib/arNurbs.h"
#include "AnimLib/keyFrameTrack.h"
#include "AReVi/Contrib/xmlParser.h"


namespace AnimLib {

//----------------------------------------------------------------------------
// CLASS       : KeyFrameTrackNurbs
//----------------------------------------------------------------------------
/*! \class KeyFrameTrackNurbs
  * \author Eric Cazeaux
  * \date 25-11-2005
  *
  * Class describing a single KeyFrameTrackNurbs template. This class implements methods
  * for inspecting keyFrameTrack attributes duration and type and read-only property.
  */
// T is the type of data
// N is the dimension of vector (ie 3 <=> Vector3)
template <class T, int N>
class KeyFrameTrackNurbs : public KeyFrameTrack
{
public:

  struct KeyFrame
    {
    AReVi::VectorN<T,N> position;
    AReVi::VectorN<T,N> deriv; // Deriv take effect only if specied/activated
    bool useDeriv;
    };
    
  KeyFrameTrackNurbs(void);
  virtual ~KeyFrameTrackNurbs(void);
  
//---------------------------- Public methods --------------------------------
public:

//--------------------------- Evaluate a point -------------------------------
  
  //! Compute a key frame at time t in [0,duration]
  inline bool // success
  computeFrame(double t, AReVi::VectorN<T,N> &keyFrameOut);
  
  //! Compute a key frame at position pos in [firstPost,lastPos]
  inline bool // success
  computePosition(unsigned pos, AReVi::VectorN<T,N> &keyFrameOut);

  //! Compute a frame at time t in [0,duration], specialized for T
  virtual
  bool // success
  computeFrame(double t, StlVector<T> &frameOut);
    
  //! Compute a key frame at position pos in [firstPost,lastPos], specialized for T
  virtual
  bool // success
  computePosition(unsigned  position, StlVector<T> &frameOut);  
  
//--------------------------- Inspect variable -------------------------------
  
  //! Inspect list of keyFrames
  inline const StlMap<unsigned, AReVi::VectorN<T,N> > &
  getKeyFrames(void) const;

//--------------------------- KeyFrames manip -------------------------------
  
  //! Add Key Frame
  inline void // if a key at position "position" already exist, otherwrite
  setKeyFrame(unsigned position, const AReVi::VectorN<T,N> &key);

  //! Get Key Frame
  inline bool // success
  getKeyFrame(unsigned position, AReVi::VectorN<T,N> &keyOut) const;

    
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
  
//--------------------------- Control interpolation --------------------------
  
  inline void
  setKeyFrameDeriv(unsigned position, const VectorN<T,N> &deriv, bool active);
  
  inline bool
  getKeyFrameDeriv(unsigned position, VectorN<T,N> &derivOut, bool &activeOut) const;
  
//--------------------------- Curve inspector -------------------------------
  
  inline const AReVi::ArNurbs::CurveN<T,N> &
  getCurve(void) const;
  
//--------------------------- Force rebuild ---------------------------------

  virtual bool //success
  rebuild(void);
      
//------------------------- Forbiden operations ------------------------------  
protected:

  virtual
  bool
  _rebuildTrackInterpolator(void);
  
  //! Save/Load to XML
  virtual
  void
  _writeXML(ArRef<XmlNode> node) const;
  
  virtual
  bool
  _readXML(ArRef<XmlNode> node);
  
//----------------------------- Attributes -----------------------------------
protected:
  StlMap<unsigned, KeyFrame> _keyFrames;
  AReVi::ArNurbs::CurveN<T,N> _curve;
  
  typedef typename StlMap<unsigned, KeyFrame >::iterator KeyFrameIterator;
  typedef typename StlMap<unsigned, KeyFrame >::const_iterator KeyFrameConstIterator;
};

} // end of namespace AnimLib

//-------- Inline source code ------------------------------------------------

#include "keyFrameTrackNurbs.Ci"


#endif // end of ANIMLIB_KEYFRAMETRACKNURBS_H
//---End-Of-File---------------------------------------------------------------
