//----------------------------------------------------------------------------
// @(#) ENIB/LI2 (c) - AnimLib
//----------------------------------------------------------------------------
// package     : AnimLib
// file        : keyFrame.h => declaration of a single keyFrame
// description : Class managing a  keyFrameTrack
// author(s)   : CAZEAUX Eric
// mail        : cazeaux@enib.fr
//----------------------------------------------------------------------------
// last update : 12/12/2005
//----------------------------------------------------------------------------
#ifndef ANIMLIB_ARTRACKSCALE_H
#define ANIMLIB_ARTRACKSCALE_H 1

//--- Imported classes -------------------------------------------------------

#include "AnimLib/animLib_plateform.h"
#include "arKeyFrameTrack.h"
#include "AReVi/Lib3D/util3D_priv.h"
#include "AnimLib/keyFrameTrackNurbs.h"

using namespace AReVi;

namespace AReVi
{
class ShapePart3D;
class Shape3D;
class Transform3D;
}
namespace AnimLib {

//----------------------------------------------------------------------------
// CLASS       : ArTrackScale
//----------------------------------------------------------------------------
/*! \class ArTrackScale
  * \author Eric Cazeaux
  * \date 12-12-2005
  *
  * Class managing a track of a material
  * Material track are definied relative to material referency.
  */

class ANIMLIB_API ArTrackScale : public ArKeyFrameTrack
{
//-------- Interpolator  -----------------------------------------------------

  typedef KeyFrameTrackNurbs<double,3> ArTrackScaleInterpolator;

//-------- Type information --------------------------------------------------
public:

  AR_CLASS(ArTrackScale)

//-------- Construction / Destruction ----------------------------------------

  AR_CONSTRUCTOR(ArTrackScale)
  
//---------------------------- Public methods --------------------------------
public:
  
//------------------------ Reference/Mobile init -----------------------------

  //! Scale will be applied to all shape parts3D.
  void
  initScaleShapes(const StlVector<ArRef<Shape3D> > &mobiles);
  
  void
  initScaleParts(const StlVector<ArRef<ShapePart3D> > &mobiles);
  
  //! Set material reference
  void
  initReference(const Util3D::Transform &transformRef);
  
//------------------------ Reference/Mobile access ----------------------------  

  Util3D::Transform &
  accessReference(void);
  
  StlVector< ArRef<Shape3D> > &
  accessShapes(void);
  
  StlVector< ArRef<ShapePart3D> > &
  accessParts(void);
  
//---------------------------- Key frames ------------------------------------

  void
  setKeyFrame(unsigned frame, const Vector3d &scale);
  
  bool // success
  getKeyFrame(unsigned frame, Vector3d &scaleOut) const;
  
  bool // success
  setPosition(double t, Vector3d &scaleOut);
  
  virtual bool // success
  setPosition(double t);
    
//-------- Event management --------------------------------------------------
  
  AR_CALLBACK(ArTrackScale,ComputeFrame,_computeFrameCB,
                          double t;// t is absolute position (passed on setPosition)
                          Vector3d scale;
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
  Util3D::Transform _reference;
  StlVector<ArRef<Shape3D> > _shapes;
  StlVector<ArRef<ShapePart3D> > _parts;

  ArRef<Transform3D> _persistent; // avoid instanciation of instance for each eval.
  CallbackManager<ArTrackScale,ComputeFrameEvent> _computeFrameCB;
};

} // end of namespace AnimLib

//-------- Inline source code ------------------------------------------------



#endif // end of ANIMLIB_ARTRACKSCALE_H
//---End-Of-File---------------------------------------------------------------
