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
#ifndef ANIMLIB_ARTRACKTRANSLATION_H
#define ANIMLIB_ARTRACKTRANSLATION_H 1

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
// CLASS       : ArTrackTranslation
//----------------------------------------------------------------------------
/*! \class ArTrackTranslation
  * \author Eric Cazeaux
  * \date 05-12-2005
  *
  * Class managing a single track of a translation. 
  */

class ANIMLIB_API ArTrackTranslation : public ArKeyFrameTrack
{
//-------- Interpolator  -----------------------------------------------------

  typedef KeyFrameTrackNurbs<double,1> ArTrackTranslationInterpolator;

//-------- Type information --------------------------------------------------
public:

  AR_CLASS(ArTrackTranslation)

//-------- Construction / Destruction ----------------------------------------

  AR_CONSTRUCTOR(ArTrackTranslation)
  
//---------------------------- Public methods --------------------------------
public:
  
//------------------------ Reference/Mobile init -----------------------------

  //! Translation will me made along axis (default is x) of reference basis.
  void
  initTranslation(ArRef<Base3D> reference, ArRef<Base3D> mobile);
  
  //! Reference is centered on mobile location, then animation will be made
  //! from this location. Use accessReference to move reference.
  void
  initTranslation(ArRef<Base3D> mobile);
  
  void
  setAxis(const Vector3d &axis);
  
  const Vector3d &
  getAxis(void) const;
  
//------------------------ Reference/Mobile access ----------------------------  

  ArRef<Base3D>
  accessReference(void);
  
  ArRef<Base3D>
  accessMobile(void);
  
//---------------------------- Key frames ------------------------------------

  void
  setKeyFrame(unsigned frame, double position);
  
  bool // success
  getKeyFrame(unsigned frame, double &positionOut) const;
  
  bool // success
  setPosition(double t, double &positionOut);
  
  virtual bool // success
  setPosition(double t);
    
//-------- Event management --------------------------------------------------
  
  AR_CALLBACK(ArTrackTranslation,ComputeFrame,_computeFrameCB,
                          double t;// t is absolute position (passed on setTranslation)
                          double position;
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
  Vector3d _axis;
  
  CallbackManager<ArTrackTranslation,ComputeFrameEvent> _computeFrameCB;
};

} // end of namespace AnimLib

//-------- Inline source code ------------------------------------------------



#endif // end of ANIMLIB_ARTRACKTRANSLATION_H
//---End-Of-File---------------------------------------------------------------
