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

#include "AnimLib/arTrackMixer.h"
#include "AReVi/arClass.h"

using namespace AReVi;

namespace AnimLib {

AR_CLASS_DEF(ArTrackMixer,ArObject)

//----------------------------------------------------------------------------
// CLASS       : ArTrackMixer
//----------------------------------------------------------------------------

//-------- Construction / Destruction ----------------------------------------

ArTrackMixer::ArTrackMixer(ArCW & arCW) // Protected
  : ArObject(arCW),
  _tracks()
{
// Nothing to be done here!
}

ArTrackMixer::~ArTrackMixer(void)
{
// Nothing to be done here!
}
  
//---------------------------- Public methods --------------------------------
//------------------------- Forbiden operations ------------------------------  



} // end of namespace AnimLib

//---End-Of-File---------------------------------------------------------------
