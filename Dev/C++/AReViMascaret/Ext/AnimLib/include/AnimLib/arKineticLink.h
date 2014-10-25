//----------------------------------------------------------------------------
// @(#) ENIB/LI2 (c) - AnimLib
//----------------------------------------------------------------------------
// package     : AnimLib
// file        : arKineticLink.h
// description : Class managing a Kinetic Link
// author(s)   : CAZEAUX Eric
// mail        : cazeaux@enib.fr
//----------------------------------------------------------------------------
// last update : 02/12/2005
//----------------------------------------------------------------------------
#ifndef ANIMLIB_ARKINETICLINK_H
#define ANIMLIB_ARKINETICLINK_H 1

//--- Imported classes -------------------------------------------------------
#include "AnimLib/animLib_plateform.h"
#include "AReVi/Lib3D/base3D.h"
#include "AReVi/Contrib/arMath.h"

namespace AReVi
{
class Activity;
}

using namespace AReVi;

namespace AnimLib {

//----------------------------------------------------------------------------
// CLASS       : ArKineticLink
//----------------------------------------------------------------------------
/*! \class ArKineticLink
  * \author Eric Cazeaux
  * \date 02-12-2005
  *
  * Class managing a kinetic link. 
  */

class ANIMLIB_API ArKineticLink : public Base3D
{          
//-------- Type information --------------------------------------------------
public:

  AR_CLASS(ArKineticLink)

//-------- Construction / Destruction ----------------------------------------

  AR_CONSTRUCTOR(ArKineticLink)
  
//---------------------------- Public methods --------------------------------
public:

  void
  attach(ArRef<Base3D> s1, ArRef<Base3D> s2, const Matrix3x3d &link);
  
//------------------------- Forbiden operations ------------------------------  
protected:
  
  virtual
  void
  _onMotion(const Base3D::MotionEvent &evt);
          
//----------------------------- Attributes -----------------------------------
protected:
  Matrix3x3d _link;
  ArRef<Base3D> _s1,_s2;
};

} // end of namespace AnimLib

//-------- Inline source code ------------------------------------------------



#endif // end of ANIMLIB_ARKINETICLINK_H
//---End-Of-File---------------------------------------------------------------
