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
#ifndef ANIMLIB_ARTRACKBASE3D_H
#define ANIMLIB_ARTRACKBASE3D_H 1

//--- Imported classes -------------------------------------------------------
#include "AnimLib/animLib_plateform.h"
#include "arTrackPosition.h"
#include "AReVi/Lib3D/util3D_priv.h"
#include "AReVi/Contrib/arMath.h"

using namespace AReVi;

namespace AReVi
{
class Base3D;
}

namespace AnimLib {
class ArTrackOrientation;

//----------------------------------------------------------------------------
// CLASS       : ArTrackBase3D
//----------------------------------------------------------------------------
/*! \class ArTrackBase3D
  * \author Eric Cazeaux
  * \date 12-12-2005
  *
  * Class managing a single track of a base3D. 
  */

class ANIMLIB_API ArTrackBase3D : public ArTrackPosition
{
//-------- Type information --------------------------------------------------
public:

  AR_CLASS(ArTrackBase3D)

//-------- Construction / Destruction ----------------------------------------

  AR_CONSTRUCTOR(ArTrackBase3D)
  
//---------------------------- Public methods --------------------------------
public:

//--------------------------- Set Base3D ----------------------------------
    
  //! Location will be changed using reference (if specified else location
  //!  will be expressed in global coordinates)
  void
  initBase3D(ArRef<Base3D> reference, ArRef<Base3D> mobile);
  
  //! Reference is centered on mobile location, then animation will be made
  //! from this location. Use accessReference to move reference.
  void
  initBase3D(ArRef<Base3D> mobile);

  virtual bool // success
  setPosition(double t);
    
//------------------------ Tracks access ----------------------------  

  ArRef<ArTrackPosition>
  accessTrackPosition(void);
  
  ArRef<ArTrackOrientation>
  accessTrackOrientation(void);
    
//------------------------ Reference/Mobile access ----------------------------  

  ArRef<Base3D>
  accessReference(void);
  
  ArRef<Base3D>
  accessMobile(void);
    
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
  ArRef<Base3D> _reference,_mobile;
  ArRef<ArTrackOrientation> _orientationTrack;
};

} // end of namespace AnimLib

//-------- Inline source code ------------------------------------------------



#endif // end of ANIMLIB_ARTRACKBASE3D_H
//---End-Of-File---------------------------------------------------------------
