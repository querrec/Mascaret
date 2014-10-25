//----------------------------------------------------------------------------
// @(#) ENIB/LI2 (c) - AnimLib
//----------------------------------------------------------------------------
// package     : AnimLib
// file        : keyFrame.h => declaration of a single keyFrame
// description : Class managing a  keyFrameTrack
// author(s)   : CAZEAUX Eric
// mail        : cazeaux@enib.fr
//----------------------------------------------------------------------------
// last update : 13/12/2005
//----------------------------------------------------------------------------
#ifndef ANIMLIB_ARTRACKTEXTURE_H
#define ANIMLIB_ARTRACKTEXTURE_H 1

//--- Imported classes -------------------------------------------------------

#include "AnimLib/animLib_plateform.h"
#include "arKeyFrameTrack.h"
#include "AReVi/Lib3D/util3D_priv.h"
#include "AnimLib/keyFrameTrackSwitch.h"

using namespace AReVi;

namespace AReVi
{
class Surface3D;
class URLTexture;
}
namespace AnimLib {

//----------------------------------------------------------------------------
// CLASS       : ArTrackTexture
//----------------------------------------------------------------------------
/*! \class ArTrackTexture
  * \author Eric Cazeaux
  * \date 13-12-2005
  *
  * Class managing a track of a surface3D texture
  */

class ANIMLIB_API ArTrackTexture : public ArKeyFrameTrack
{
//-------- Interpolator  -----------------------------------------------------

  typedef KeyFrameTrackSwitch<StlString> ArTrackTextureInterpolator;

//-------- Type information --------------------------------------------------
public:

  AR_CLASS(ArTrackTexture)

//-------- Construction / Destruction ----------------------------------------

  AR_CONSTRUCTOR(ArTrackTexture)
  
//---------------------------- Public methods --------------------------------
public:
  
//------------------------ Reference/Mobile init -----------------------------

  //! Texture will be applied to all shape parts3D.
  void
  initTexture(const StlVector<ArRef<Surface3D> > &mobiles);
  
//------------------------ Reference/Mobile access ----------------------------  

  StlVector< ArRef<Surface3D> > &
  accessMobiles(void);
  
//---------------------------- Key frames ------------------------------------

  void
  setKeyFrame(unsigned frame, ArRef<URLTexture> texture);
  
  bool // success
  getKeyFrame(unsigned frame, ArRef<URLTexture> &textureOut) const;
  
  bool //success
  setPosition(double t, ArRef<URLTexture> &textureOut);
  
  virtual bool // success
  setPosition(double t);
    
//-------- Event management --------------------------------------------------
  
  AR_CALLBACK(ArTrackTexture,ComputeFrame,_computeFrameCB,
                          double t;// t is absolute position (passed on setPosition)
                          ArRef<URLTexture> texture;
                          ) 
  
//------------------------- Forbiden operations ------------------------------  
protected:
   
  virtual
  ArRef<URLTexture>
  _accessCache(const StlString &url);
  
//----------------------------- Attributes -----------------------------------
protected:
  StlVector<ArRef<Surface3D> > _mobiles;

  StlMap<StlString, ArRef<URLTexture> > _cache; // avoid instanciation of Texture for each eval.
  CallbackManager<ArTrackTexture,ComputeFrameEvent> _computeFrameCB;
};

} // end of namespace AnimLib

//-------- Inline source code ------------------------------------------------



#endif // end of ANIMLIB_ARTRACKTEXTURE_H
//---End-Of-File---------------------------------------------------------------
