//----------------------------------------------------------------------------
// @(#) ENIB/LI2 (c) - AnimLib
//----------------------------------------------------------------------------
// package     : AnimLib
// file        : keyFrame.h => declaration of a single keyFrame
// description : Class managing a  keyFrameTrack
// author(s)   : CAZEAUX Eric
// mail        : cazeaux@enib.fr
//----------------------------------------------------------------------------
// last update : 05/12/2005
//----------------------------------------------------------------------------
#ifndef ANIMLIB_ARTRACKSPIRAL_H
#define ANIMLIB_ARTRACKSPIRAL_H 1

//--- Imported classes -------------------------------------------------------

#include "AnimLib/animLib_plateform.h"
#include "arKeyFrameTrack.h"
#include "AReVi/Lib3D/util3D_priv.h"
#include "AnimLib/keyFrameTrackNurbs.h"

using namespace AReVi;

namespace AReVi
{
class Base3D;

}
namespace AnimLib {

//----------------------------------------------------------------------------
// CLASS       : ArTrackSpiral
//----------------------------------------------------------------------------
/*! \class ArTrackSpiral
  * \author Eric Cazeaux
  * \date 05-12-2005
  *
  * Class managing a track of a spiral 
  * The trajectory is defined by the point rotating alors the axis "_axis"
  * of "angle", and at the distance dst of the center of reference (along _offset)
  * and height along the main axis of rotation.
  */

class ANIMLIB_API ArTrackSpiral : public ArKeyFrameTrack
{
//-------- Interpolator  -----------------------------------------------------

  typedef KeyFrameTrackNurbs<double,3> ArTrackSpiralInterpolator;

//-------- Type information --------------------------------------------------
public:

  AR_CLASS(ArTrackSpiral)

//-------- Construction / Destruction ----------------------------------------

  AR_CONSTRUCTOR(ArTrackSpiral)
  
//---------------------------- Public methods --------------------------------
public:
  
//------------------------ Reference/Mobile init -----------------------------

  //! Spiral will me made along axis (default x) with an offset (default is 0,0,0) of reference basis.
  void
  initSpiral(ArRef<Base3D> reference, ArRef<Base3D> mobile);
  
  //! Reference is centered on mobile location, then animation will be made
  //! from this location. Use accessReference to move reference.
  void
  initSpiral(ArRef<Base3D> mobile);
  
  void
  setAxis(const Vector3d &axis);
  
  const Vector3d &
  getAxis(void) const;
  
  void
  setOffset(const Vector3d &offset);
  
  const Vector3d &
  getOffset(void) const;
  
//------------------------ Reference/Mobile access ----------------------------  

  ArRef<Base3D>
  accessReference(void);
  
  ArRef<Base3D>
  accessMobile(void);
  
//---------------------------- Key frames ------------------------------------

  void
  setKeyFrame(unsigned frame, double angle, double distance, double height);
  
  bool // success
  getKeyFrame(unsigned frame, double &angleOut, double &distanceOut, double &heightOut) const;
  
  bool // success
  setPosition(double t, double &angleOut, double &distanceOut, double &heightOut);
  
  virtual bool // success
  setPosition(double t);
    
//-------- Event management --------------------------------------------------
  
  AR_CALLBACK(ArTrackSpiral,ComputeFrame,_computeFrameCB,
                          double t;// t is absolute position (passed on setSpiral)
                          double angle;
                          double distance;
                          double height;
                          ) 
  
//------------------------- Forbiden operations ------------------------------  
protected:
  
 
//! write/read to XML
  virtual
  void
  _writeXML(ArRef<XmlNode> node) const;
  
  virtual
  bool
  _readXML(ArRef<XmlNode> node);
    
    
//----------------------------- Attributes -----------------------------------
protected:
  ArRef<Base3D> _reference;
  ArRef<Base3D> _mobile;
  
  Vector3d _axis, _offset;
  
  CallbackManager<ArTrackSpiral,ComputeFrameEvent> _computeFrameCB;
};

} // end of namespace AnimLib

//-------- Inline source code ------------------------------------------------



#endif // end of ANIMLIB_ARTRACKSPIRAL_H
//---End-Of-File---------------------------------------------------------------
