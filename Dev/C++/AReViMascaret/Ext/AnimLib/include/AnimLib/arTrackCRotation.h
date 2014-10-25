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
#ifndef ANIMLIB_ARTRACKROTATION_H
#define ANIMLIB_ARTRACKROTATION_H 1

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
// CLASS       : ArTrackRotation
//----------------------------------------------------------------------------
/*! \class ArTrackRotation
  * \author Eric Cazeaux
  * \date 05-12-2005
  *
  * Class managing a single track of a translation. 
  */

class ANIMLIB_API ArTrackRotation : public ArKeyFrameTrack
{
//-------- Interpolator  -----------------------------------------------------

  typedef KeyFrameTrackNurbs<double,1> ArTrackRotationInterpolator;

//-------- Type information --------------------------------------------------
public:

  AR_CLASS(ArTrackRotation)

//-------- Construction / Destruction ----------------------------------------

  AR_CONSTRUCTOR(ArTrackRotation)
  
//---------------------------- Public methods --------------------------------
public:
  
//------------------------ Reference/Mobile init -----------------------------

  //! Rotation will me made along axis (default x) with an offset (default is 0,0,0) of reference basis.
  void
  initRotation(ArRef<Base3D> reference, ArRef<Base3D> mobile);
  
  //! Reference is centered on mobile location, then animation will be made
  //! from this location. Use accessReference to move reference.
  void
  initRotation(ArRef<Base3D> mobile);
  
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
  setKeyFrame(unsigned frame, double angle);
  
  bool // success
  getKeyFrame(unsigned frame, double &angleOut) const;
  
  bool // success
  setPosition(double t, double &angleOut);
  
  virtual bool // success
  setPosition(double t);
    
//-------- Event management --------------------------------------------------
  
  AR_CALLBACK(ArTrackRotation,ComputeFrame,_computeFrameCB,
                          double t;// t is absolute position (passed on setRotation)
                          double angle;
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
  
  CallbackManager<ArTrackRotation,ComputeFrameEvent> _computeFrameCB;
};

} // end of namespace AnimLib

//-------- Inline source code ------------------------------------------------



#endif // end of ANIMLIB_ARTRACKROTATION_H
//---End-Of-File---------------------------------------------------------------
