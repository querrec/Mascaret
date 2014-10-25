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
#ifndef ANIMLIB_ARTRACKMATERIAL3D_H
#define ANIMLIB_ARTRACKMATERIAL3D_H 1

//--- Imported classes -------------------------------------------------------
#include "AnimLib/animLib_plateform.h"
#include "arKeyFrameTrack.h"
#include "AReVi/Lib3D/util3D_priv.h"
#include "AnimLib/keyFrameTrackNurbs.h"

using namespace AReVi;

namespace AReVi
{
class Surface3D;
class Material3D;
}
namespace AnimLib {

//----------------------------------------------------------------------------
// CLASS       : ArTrackMaterial3D
//----------------------------------------------------------------------------
/*! \class ArTrackMaterial3D
  * \author Eric Cazeaux
  * \date 12-12-2005
  *
  * Class managing a track of a material
  * Material track are definied relative to material referency.
  */

class ANIMLIB_API ArTrackMaterial3D : public ArKeyFrameTrack
{
//-------- Interpolator  -----------------------------------------------------

  typedef KeyFrameTrackNurbs<double,12> ArTrackMaterial3DInterpolator;

//-------- Type information --------------------------------------------------
public:

  AR_CLASS(ArTrackMaterial3D)

//-------- Construction / Destruction ----------------------------------------

  AR_CONSTRUCTOR(ArTrackMaterial3D)
  
//---------------------------- Public methods --------------------------------
public:
  
//------------------------ Reference/Mobile init -----------------------------

  //! Material3D will be applied to all shape parts3D.
  void
  initMaterial3D(const StlVector<ArRef<Surface3D> > &mobiles);
  
  //! Set material reference
  void
  initReference(const Util3D::Material &matRef);
  
//------------------------ Reference/Mobile access ----------------------------  

  Util3D::Material &
  accessReference(void);
  
  StlVector< ArRef<Surface3D> > &
  accessMobiles(void);
  
//---------------------------- Key frames ------------------------------------

  void
  setKeyFrame(unsigned frame, const Util3D::Material &material);
  
  bool // success
  getKeyFrame(unsigned frame, Util3D::Material &materialOut) const;
  
  bool // success
  setPosition(double t, Util3D::Material &materialOut);
  
  virtual bool // success
  setPosition(double t);
    
//-------- Event management --------------------------------------------------
  
  AR_CALLBACK(ArTrackMaterial3D,ComputeFrame,_computeFrameCB,
                          double t;// t is absolute position (passed on setPosition)
                          Util3D::Material material;
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
    
  virtual
  void
  _writeMaterialXML(ArRef<XmlNode> node, const Util3D::Material &material) const;
  
  virtual
  bool
  _readMaterialXML(ArRef<XmlNode> node, Util3D::Material &materialOut) const;
    
//----------------------------- Attributes -----------------------------------
protected:
  Util3D::Material _reference;
  StlVector<ArRef<Surface3D> > _mobiles;

  ArRef<Material3D> _persistent; // avoid instanciation of Material3D for each eval.
  CallbackManager<ArTrackMaterial3D,ComputeFrameEvent> _computeFrameCB;
};

} // end of namespace AnimLib

//-------- Inline source code ------------------------------------------------



#endif // end of ANIMLIB_ARTRACKMATERIAL3D_H
//---End-Of-File---------------------------------------------------------------
