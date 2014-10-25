//----------------------------------------------------------------------------
// @(#) ENIB/LI2 (c) - AnimLib
//----------------------------------------------------------------------------
// package     : AnimLib
// file        : arTrackBase3D.cpp
// description : Class describing a  ArTrackTranslation
// author(s)   : CAZEAUX Eric
// mail        : cazeaux@enib.fr
//----------------------------------------------------------------------------
// last update : 05/12/2005
//----------------------------------------------------------------------------

#include "AnimLib/arTrackTranslation.h"
#include "AReVi/Lib3D/base3D.h"


using namespace AReVi;

namespace AnimLib {

AR_CLASS_DEF(ArTrackTranslation,ArKeyFrameTrack)

//----------------------------------------------------------------------------
// CLASS       : ArTrackTranslation
//----------------------------------------------------------------------------

//-------- Construction / Destruction ----------------------------------------

ArTrackTranslation::ArTrackTranslation(ArCW & arCW) // Protected
  : ArKeyFrameTrack(arCW),
    _reference(Base3D::NEW()),
    _mobile(Base3D::NEW()),
    _axis(1,0,0),
    _computeFrameCB(thisPtr())
{
_initTrack(new ArTrackTranslationInterpolator());
}

ArTrackTranslation::~ArTrackTranslation(void)
{
// Nothing to be done here!
}
  
//---------------------------- Public methods --------------------------------

  
//------------------------ Reference/Mobile init -----------------------------

// Translation will me made alors x-axis of reference basis.
void
ArTrackTranslation::initTranslation(ArRef<Base3D> reference, ArRef<Base3D> mobile)
{
if (_mobile && _reference)  
  _mobile->attachTo(_reference,Base3D::ATTACH_NONE);
  
_reference=reference;
_mobile=mobile;
_mobile->attachTo(_reference);
}

// Reference is centered on mobile location, then animation will be made
// from this location. Use accessReference to move reference.
void
ArTrackTranslation::initTranslation(ArRef<Base3D> mobile)
{
if (_mobile && _reference)  
  _mobile->attachTo(_reference,Base3D::ATTACH_NONE);
  
_reference=Base3D::NEW();
_mobile=mobile;
_reference->setLocation(mobile);
_mobile->attachTo(_reference);
}

void
ArTrackTranslation::setAxis(const Vector3d &axis)
{
_axis=axis;
}

const Vector3d &
ArTrackTranslation::getAxis(void) const
{
return(_axis);
}

//------------------------ Reference/Mobile access ----------------------------  

ArRef<Base3D>
ArTrackTranslation::accessReference(void)
{
return(_reference);
}

ArRef<Base3D>
ArTrackTranslation::accessMobile(void)
{
return(_mobile);
}


//--------------------------- Set Base3D ----------------------------------
    
void
ArTrackTranslation::setKeyFrame(unsigned frame, double position)
{
VectorN<double,1> base;
base[0]=position;
((ArTrackTranslationInterpolator *)_track)->setKeyFrame(frame,base);
}

bool // success
ArTrackTranslation::getKeyFrame(unsigned frame, double &position) const
{
VectorN<double,1> base;

if (!((ArTrackTranslationInterpolator *)_track)->getKeyFrame(frame,base))
  {
  return(false);
  }

position=base[0];
return(true);
}

bool // success
ArTrackTranslation::setPosition(double t, double &position)
{
if(!ArKeyFrameTrack::setPosition(t))
  return(false);
  
VectorN<double,1> base;

if (!((ArTrackTranslationInterpolator *)_track)->computeFrame(_position,base))
  {
  return(false);
  }

position=base[0];

Vector3d mvt(_axis*position);
_reference->localToGlobalPosition(mvt.accessVector()[0],mvt.accessVector()[1],mvt.accessVector()[2]);
_mobile->setPosition(mvt.x(),mvt.y(),mvt.z());

if(!_computeFrameCB.empty())
  {
  ArTrackTranslation::ComputeFrameEvent evt;
  evt.t=t;
  evt.position=position;
  _computeFrameCB.fire(evt);
  }
return(true);  
}
    
bool // success
ArTrackTranslation::setPosition(double t)
{
double position;
return(ArTrackTranslation::setPosition(t, position));
}

    
//------------------------- Forbiden operations ------------------------------  


// Save/Load to XML
void
ArTrackTranslation::_writeXML(ArRef<XmlNode> node) const
{
ArKeyFrameTrack::_writeXML(node);
node->setName("TrackTranslation");
node->setPropertyReal("vx",_axis.x());
node->setPropertyReal("vy",_axis.y());
node->setPropertyReal("vz",_axis.z());
}

bool
ArTrackTranslation::_readXML(ArRef<XmlNode> node)
{
if(!ArKeyFrameTrack::_readXML(node))
  return(false);
  
if (node->getName()=="TrackTranslation")
  {
  node->getPropertyReal("vx",_axis.accessVector()[0]);
  node->getPropertyReal("vy",_axis.accessVector()[1]);
  node->getPropertyReal("vz",_axis.accessVector()[2]);
  }
else
  _axis=Vector3d(1,0,0);
  
return(true);
}


} // end of namespace AnimLib

//---End-Of-File---------------------------------------------------------------
