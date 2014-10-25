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
#ifndef ANIMLIB_ARTRACKORIENTATION_H
#define ANIMLIB_ARTRACKORIENTATION_H 1

//--- Imported classes -------------------------------------------------------
#include "AnimLib/animLib_plateform.h"
#include "arKeyFrameTrack.h"
#include "AReVi/Lib3D/util3D_priv.h"
#include "AReVi/Contrib/arMath.h"

using namespace AReVi;

namespace AnimLib {

//----------------------------------------------------------------------------
// CLASS       : ArTrackOrientation
//----------------------------------------------------------------------------
/*! \class ArTrackOrientation
  * \author Eric Cazeaux
  * \date 01-12-2005
  *
  * Class managing a single track of a base3D. 
  */

class ANIMLIB_API ArTrackOrientation : public ArKeyFrameTrack
{
//-------- Type information --------------------------------------------------
public:

  AR_CLASS(ArTrackOrientation)

//-------- Construction / Destruction ----------------------------------------

  AR_CONSTRUCTOR(ArTrackOrientation)
  
//---------------------------- Public methods --------------------------------
public:

//--------------------------- Set KeyFrames ----------------------------------

    void
    setKeyFrame(unsigned position, const Vector3d &vector, double angle);
    
    void
    setKeyFrame(unsigned position, const Quaterniond &orientation);
    
    bool // success
    getKeyFrame(unsigned position, Vector3d &vectorOut, double &angleOut) const;
    
    bool // success
    getKeyFrame(unsigned position, Quaterniond &orientationOut) const;
    
    
    bool // success
    setPosition(double t, Quaterniond &orientationOut);
    
    virtual bool // success
    setPosition(double t);
      
//-------- Event management --------------------------------------------------
    
    AR_CALLBACK(ArTrackOrientation,ComputeFrame,_computeFrameCB,
                            double t;// t is absolute position (passed on setPosition)
                            Quaterniond orientation;
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
  CallbackManager<ArTrackOrientation,ComputeFrameEvent> _computeFrameCB;
};

} // end of namespace AnimLib

//-------- Inline source code ------------------------------------------------



#endif // end of ANIMLIB_ARTRACKORIENTATION_H
//---End-Of-File---------------------------------------------------------------
