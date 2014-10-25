//----------------------------------------------------------------------------
// @(#) ENIB/LI2 (c) - AnimLib
//----------------------------------------------------------------------------
// package     : AnimLib
// file        : keyFrame.cpp => implementation of a single keyFrame
// description : Class describing a keyFrameTrack, a keyFrameTrackReal, keyFrameTrackReal3// author(s)   : CAZEAUX Eric
// mail        : cazeaux@enib.fr
//----------------------------------------------------------------------------
// last update : 01/12/2005
//----------------------------------------------------------------------------

#include "AnimLib/arKineticLink.h"

using namespace AReVi;

namespace AnimLib {

AR_CLASS_DEF(ArKineticLink,Base3D)

//----------------------------------------------------------------------------
// CLASS       : ArKineticLink
//----------------------------------------------------------------------------

//-------- Construction / Destruction ----------------------------------------

ArKineticLink::ArKineticLink(ArCW & arCW) // Protected
  : Base3D(arCW),
    _link(),
    _s1(),
    _s2()
{
// Nothing to be done here!
}

ArKineticLink::~ArKineticLink(void)
{
// Nothing to be done here!
}
  
//---------------------------- Public methods --------------------------------

void
ArKineticLink::attach(ArRef<Base3D> s1, ArRef<Base3D> s2, const Matrix3x3d &/*link*/)
{
s1->attachTo(s2,ATTACH_TRANSLATION|ATTACH_ROLL|/*ATTACH_PITCH|*/ATTACH_YAW);
// s2->attachTo(s1,ATTACH_TRANSLATION|ATTACH_ROLL|ATTACH_PITCH);
}  
  
//------------------------- Forbiden operations ------------------------------  

void
ArKineticLink::_onMotion(const Base3D::MotionEvent &/*evt*/)
{
}
  
      
} // end of namespace AnimLib

//---End-Of-File---------------------------------------------------------------
