//----------------------------------------------------------------------------
// @(#) ENIB/LI2 (c) - AnimLib
//----------------------------------------------------------------------------
// package     : AnimLib
// file        : arTrackBase3D.cpp
// description : Class describing a  ArTrackPosition
// author(s)   : CAZEAUX Eric
// mail        : cazeaux@enib.fr
//----------------------------------------------------------------------------
// last update : 01/12/2005
//----------------------------------------------------------------------------

#include "AnimLib/arTrackPosition.h"
#include "AReVi/arClass.h"


using namespace AReVi;

namespace AnimLib {

AR_CLASS_DEF(ArTrackPosition,ArKeyFrameTrack)

//----------------------------------------------------------------------------
// CLASS       : ArTrackPosition
//----------------------------------------------------------------------------

//-------- Construction / Destruction ----------------------------------------

ArTrackPosition::ArTrackPosition(ArCW & arCW) // Protected
  : ArKeyFrameTrack(arCW),
    _computeFrameCB(thisPtr())
{
_initTrack(new ArTrackPositionInterpolator());
}

ArTrackPosition::~ArTrackPosition(void)
{
// Nothing to be done here!
}
  
//---------------------------- Public methods --------------------------------

//--------------------------- Set Base3D ----------------------------------
    
bool
ArTrackPosition::getTrajectory(ArNurbs::Curve3d &trajectoryOut) const
{
const ArNurbs::CurveN<double,3> &curve(((ArTrackPositionInterpolator *)_track)->getCurve());

const StlVector<VectorN<double,3> > &src(curve.getCtrlPoints());

trajectoryOut.resize(curve.getCtrlPoints().size(),curve.getDegree());


StlVector<Vector3d> &dst(trajectoryOut.accessCtrlPoints());

for (unsigned cpt=dst.size();cpt--;)
  {
  dst[cpt]=Vector3d(src[cpt][0],src[cpt][1],src[cpt][2]);
  }
  
trajectoryOut.accessKnotVector()=curve.getKnotVector();
trajectoryOut.accessWeights()=curve.getWeights();

return(trajectoryOut.valid());
}

    
void
ArTrackPosition::setKeyFrame(unsigned frame, const Vector3d &position)
{
VectorN<double,3> base;
base[0]=position.x();
base[1]=position.y();
base[2]=position.z();

((ArTrackPositionInterpolator *)_track)->setKeyFrame(frame,base);
}

bool // success
ArTrackPosition::getKeyFrame(unsigned frame, Vector3d &position) const
{
VectorN<double,3> base;

if (!((ArTrackPositionInterpolator *)_track)->getKeyFrame(frame,base))
  {
  return(false);
  }

position=Vector3d(base[0],base[1],base[2]);
return(true);
}

void
ArTrackPosition::setKeyFrameDeriv(unsigned frame, const Vector3d &deriv, bool active)
{
VectorN<double,3> base;
base[0]=deriv.x();
base[1]=deriv.y();
base[2]=deriv.z();

((ArTrackPositionInterpolator *)_track)->setKeyFrameDeriv(frame,base, active);
}

bool // success
ArTrackPosition::getKeyFrameDeriv(unsigned frame, Vector3d &derivOut, bool &activeOut) const
{
VectorN<double,3> base;

if (!((ArTrackPositionInterpolator *)_track)->getKeyFrameDeriv(frame,base,activeOut))
  {
  return(false);
  }

derivOut=Vector3d(base[0],base[1],base[2]);
return(true);
}

    
bool // success
ArTrackPosition::setPosition(double t, Vector3d &position)
{
if(!ArKeyFrameTrack::setPosition(t))
  return(false);
  
VectorN<double,3> base;

if (!((ArTrackPositionInterpolator *)_track)->computeFrame(_position,base))
  {
  return(false);
  }

position=Vector3d(base[0],base[1],base[2]);

if(!_computeFrameCB.empty())
  {
  ArTrackPosition::ComputeFrameEvent evt;
  evt.t=t;
  evt.position=position;
  _computeFrameCB.fire(evt);
  }
return(true);  
}
    
bool // success
ArTrackPosition::setPosition(double t)
{
Vector3d position;
return(ArTrackPosition::setPosition(t, position));
}

    
//------------------------- Forbiden operations ------------------------------  

//--------------------------- Compute frame ---------------------------------
/*    
bool // success
ArTrackPosition::_computeFrame(double position) // virtual 
{
return(true);
}*/

} // end of namespace AnimLib

//---End-Of-File---------------------------------------------------------------
