//----------------------------------------------------------------------------
// @(#) ENIB/LI2 (c) - AnimLib
//----------------------------------------------------------------------------
// package     : AnimLib
// file        : arTrackBase3D.cpp
// description : Class describing a  ArTrackRotation
// author(s)   : CAZEAUX Eric
// mail        : cazeaux@enib.fr
//----------------------------------------------------------------------------
// last update : 05/12/2005
//----------------------------------------------------------------------------

#include "AnimLib/arTrackRotation.h"
#include "AReVi/Lib3D/base3D.h"


using namespace AReVi;

namespace AnimLib {

AR_CLASS_DEF(ArTrackRotation,ArKeyFrameTrack)

//----------------------------------------------------------------------------
// CLASS       : ArTrackRotation
//----------------------------------------------------------------------------

//-------- Construction / Destruction ----------------------------------------

ArTrackRotation::ArTrackRotation(ArCW & arCW) // Protected
  : ArKeyFrameTrack(arCW),
    _reference(Base3D::NEW()),
    _mobile(Base3D::NEW()),
    _axis(0.0,0.0,0.0),
    _offset(0.0,0.0,0.0),
    _computeFrameCB(thisPtr())
{
_initTrack(new ArTrackRotationInterpolator());
}

ArTrackRotation::~ArTrackRotation(void)
{
// Nothing to be done here!
}
  
//---------------------------- Public methods --------------------------------

  
//------------------------ Reference/Mobile init -----------------------------

// Rotation will me made alors x-axis of reference basis.
void
ArTrackRotation::initRotation(ArRef<Base3D> reference, ArRef<Base3D> mobile)
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
ArTrackRotation::initRotation(ArRef<Base3D> mobile)
{
if (_mobile && _reference)  
  _mobile->attachTo(_reference,Base3D::ATTACH_NONE);

_reference=Base3D::NEW();
_mobile=mobile;
_reference->setLocation(mobile);
_mobile->attachTo(_reference);
}

void
ArTrackRotation::setOffset(const Vector3d &offset)
{
_offset=offset;
}

const Vector3d &
ArTrackRotation::getOffset(void) const
{
return(_offset);
}


void
ArTrackRotation::setAxis(const Vector3d &axis)
{
_axis=axis;
}

const Vector3d &
ArTrackRotation::getAxis(void) const
{
return(_axis);
}


//------------------------ Reference/Mobile access ----------------------------  

ArRef<Base3D>
ArTrackRotation::accessReference(void)
{
return(_reference);
}

ArRef<Base3D>
ArTrackRotation::accessMobile(void)
{
return(_mobile);
}


//--------------------------- Set Base3D ----------------------------------
    
void
ArTrackRotation::setKeyFrame(unsigned frame, double position)
{
VectorN<double,1> base;
base[0]=position;
((ArTrackRotationInterpolator *)_track)->setKeyFrame(frame,base);
}

bool // success
ArTrackRotation::getKeyFrame(unsigned frame, double &position) const
{
VectorN<double,1> base;

if (!((ArTrackRotationInterpolator *)_track)->getKeyFrame(frame,base))
  {
  return(false);
  }

position=base[0];
return(true);
}

bool // success
ArTrackRotation::setPosition(double t, double &angle)
{
if(!ArKeyFrameTrack::setPosition(t))
  return(false);
  
VectorN<double,1> base;

if (!((ArTrackRotationInterpolator *)_track)->computeFrame(_position,base))
  {
  return(false);
  }

angle=base[0];

// _mobile->accessBase()=Util3D::Base(true);
// // _mobile->accessBase().setRotation(1.0,0,0,angle);
// _mobile->translate(0.0,_radius,0.0);
// _mobile->pitch(angle);
// 
// _reference->localToGlobalLocation(_mobile);

_mobile->accessBase()=_reference->accessBase();
double x(_offset.x()),y(_offset.y()),z(_offset.z());
_mobile->rotate(_axis.x(),_axis.y(),_axis.z(),angle);
_mobile->localToGlobalPosition(x,y,z);
_mobile->setPosition(x,y,z);

if(!_computeFrameCB.empty())
  {
  ArTrackRotation::ComputeFrameEvent evt;
  evt.t=t;
  evt.angle=angle;
  _computeFrameCB.fire(evt);
  }
return(true);  
}
    
bool // success
ArTrackRotation::setPosition(double t)
{
double position;
return(ArTrackRotation::setPosition(t, position));
}

    
//------------------------- Forbiden operations ------------------------------  


// Save/Load to XML
void
ArTrackRotation::_writeXML(ArRef<XmlNode> node) const
{
ArKeyFrameTrack::_writeXML(node);
node->setName("TrackRotation");
node->setPropertyReal("vx",_axis.x());
node->setPropertyReal("vy",_axis.y());
node->setPropertyReal("vz",_axis.z());
node->setPropertyReal("ox",_offset.x());
node->setPropertyReal("oy",_offset.y());
node->setPropertyReal("oz",_offset.z());

}

bool
ArTrackRotation::_readXML(ArRef<XmlNode> node)
{
if(!ArKeyFrameTrack::_readXML(node))
  return(false);
  
if (node->getName()=="TrackRotation")
  {
  node->getPropertyReal("vx",_axis.accessVector()[0]);
  node->getPropertyReal("vy",_axis.accessVector()[1]);
  node->getPropertyReal("vz",_axis.accessVector()[2]);
  
  node->getPropertyReal("ox",_offset.accessVector()[0]);
  node->getPropertyReal("oy",_offset.accessVector()[1]);
  node->getPropertyReal("oz",_offset.accessVector()[2]);
  }
else
  {
  _offset=Vector3d(0,0,0);
  _axis=Vector3d(1,0,0);
  }
  
return(true);
}


} // end of namespace AnimLib

//---End-Of-File---------------------------------------------------------------
