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
#ifndef ANIMLIB_ARTRACKMIXER_H
#define ANIMLIB_ARTRACKMIXER_H 1

//--- Imported classes -------------------------------------------------------
#include "AnimLib/animLib_plateform.h"
#include "AReVi/arObject.h"

using namespace AReVi;

namespace AnimLib {

class ArKeyFrameTrack;

//----------------------------------------------------------------------------
// CLASS       : ArTrackMixer
//----------------------------------------------------------------------------
/*! \class ArTrackMixer
  * \author Eric Cazeaux
  * \date 01-12-2005
  *
  * Class managing a map of tracks. 
  */

class ANIMLIB_API ArTrackMixer : public ArObject
{
//-------- Type information --------------------------------------------------
public:

  AR_CLASS(ArTrackMixer)

//-------- Construction / Destruction ----------------------------------------

  AR_CONSTRUCTOR(ArTrackMixer)
  
//---------------------------- Public methods --------------------------------
public:

//--------------------------- Track accessors ---------------------------------
    
//-------- Event management --------------------------------------------------
    
//------------------------- Forbiden operations ------------------------------  
protected:
  
//----------------------------- Attributes -----------------------------------
protected:
  StlMap<StlString, ArRef<ArKeyFrameTrack> > _tracks;
};

} // end of namespace AnimLib

//-------- Inline source code ------------------------------------------------



#endif // end of ANIMLIB_ARTRACKMIXER_H
//---End-Of-File---------------------------------------------------------------
