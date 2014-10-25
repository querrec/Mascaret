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
#ifndef ANIMLIB_ARTRACKPOSITION_H
#define ANIMLIB_ARTRACKPOSITION_H 1

//--- Imported classes -------------------------------------------------------
#include "AnimLib/animLib_plateform.h"
#include "arKeyFrameTrack.h"
#include "AReVi/Lib3D/util3D_priv.h"
#include "AnimLib/keyFrameTrackNurbs.h"

using namespace AReVi;

namespace AnimLib {

//----------------------------------------------------------------------------
// CLASS       : ArTrackPosition
//----------------------------------------------------------------------------
/*! \class ArTrackPosition
  * \author Eric Cazeaux
  * \date 01-12-2005
  *
  * Class managing a single track of a base3D. 
  */

class ANIMLIB_API ArTrackPosition : public ArKeyFrameTrack
{
//-------- Interpolator  -----------------------------------------------------
public:
  typedef KeyFrameTrackNurbs<double,3> ArTrackPositionInterpolator;

//-------- Type information --------------------------------------------------
public:

  AR_CLASS(ArTrackPosition)

//-------- Construction / Destruction ----------------------------------------

  AR_CONSTRUCTOR(ArTrackPosition)
  
//---------------------------- Public methods --------------------------------
public:

    //! Allow to see trajectory defined by this track
    bool // success
    getTrajectory(ArNurbs::Curve3d &trajectoryOut) const;
    
    void
    setKeyFrame(unsigned pos, const Vector3d &position);
    
    bool // success
    getKeyFrame(unsigned position, Vector3d &positionOut) const;
    
    void
    setKeyFrameDeriv(unsigned position, const Vector3d &deriv, bool active);
    
    bool // success
    getKeyFrameDeriv(unsigned position, Vector3d &derivOut, bool &activeOut) const;
    
    bool // success
    setPosition(double t, Vector3d &positionOut);
    
    virtual bool // success
    setPosition(double t);
      
//-------- Event management --------------------------------------------------
    
    AR_CALLBACK(ArTrackPosition,ComputeFrame,_computeFrameCB,
                            double t;// t is absolute position (passed on setPosition)
                            Vector3d position;
                            ) 
    
//------------------------- Forbiden operations ------------------------------  
protected:
  
//--------------------------- Compute frame ---------------------------------
    
//     virtual bool
//     _computeFrame(double position);
    
//----------------------------- Attributes -----------------------------------
protected:

    CallbackManager<ArTrackPosition,ComputeFrameEvent> _computeFrameCB;
};

} // end of namespace AnimLib

//-------- Inline source code ------------------------------------------------



#endif // end of ANIMLIB_ARTRACKPOSITION_H
//---End-Of-File---------------------------------------------------------------
