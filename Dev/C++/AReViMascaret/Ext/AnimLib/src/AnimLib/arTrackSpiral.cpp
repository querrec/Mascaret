//----------------------------------------------------------------------------
// @(#) ENIB/LI2 (c) - AnimLib
//----------------------------------------------------------------------------
// package     : AnimLib
// file        : arTrackBase3D.cpp
// description : Class describing a  ArTrackSpiral
// author(s)   : CAZEAUX Eric
// mail        : cazeaux@enib.fr
//----------------------------------------------------------------------------
// last update : 05/12/2005
//----------------------------------------------------------------------------

#include "AnimLib/arTrackSpiral.h"
#include "AReVi/Lib3D/base3D.h"


using namespace AReVi;

namespace AnimLib {

AR_CLASS_DEF(ArTrackSpiral,ArKeyFrameTrack)

//----------------------------------------------------------------------------
// CLASS       : ArTrackSpiral
//----------------------------------------------------------------------------

//-------- Construction / Destruction ----------------------------------------

ArTrackSpiral::ArTrackSpiral(ArCW & arCW) // Protected
  : ArKeyFrameTrack(arCW),
    _reference(Base3D::NEW()),
    _mobile(Base3D::NEW()),
    _axis(0.0,0.0,0.0),
    _offset(0.0,0.0,0.0),
    _computeFrameCB(thisPtr())
{
_initTrack(new ArTrackSpiralInterpolator());
}

ArTrackSpiral::~ArTrackSpiral(void)
{
// Nothing to be done here!
}
  
//---------------------------- Public methods --------------------------------

  
//------------------------ Reference/Mobile init -----------------------------

// Spiral will me made alors x-axis of reference basis.
void
ArTrackSpiral::initSpiral(ArRef<Base3D> reference, ArRef<Base3D> mobile)
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
ArTrackSpiral::initSpiral(ArRef<Base3D> mobile)
{
if (_mobile && _reference)  
  _mobile->attachTo(_reference,Base3D::ATTACH_NONE);

_reference=Base3D::NEW();
_mobile=mobile;
_reference->setLocation(mobile);
_mobile->attachTo(_reference);
}

void
ArTrackSpiral::setOffset(const Vector3d &offset)
{
_offset=offset;
}

const Vector3d &
ArTrackSpiral::getOffset(void) const
{
return(_offset);
}


void
ArTrackSpiral::setAxis(const Vector3d &axis)
{
_axis=axis;
}

const Vector3d &
ArTrackSpiral::getAxis(void) const
{
return(_axis);
}


//------------------------ Reference/Mobile access ----------------------------  

ArRef<Base3D>
ArTrackSpiral::accessReference(void)
{
return(_reference);
}

ArRef<Base3D>
ArTrackSpiral::accessMobile(void)
{
return(_mobile);
}


//--------------------------- Set Base3D ----------------------------------
    
void
ArTrackSpiral::setKeyFrame(unsigned frame, double position, double distance, double height)
{
VectorN<double,3> base;
base[0]=position;
base[1]=distance;
base[2]=height;
((ArTrackSpiralInterpolator *)_track)->setKeyFrame(frame,base);
}

bool // success
ArTrackSpiral::getKeyFrame(unsigned frame, double &position, double &distance, double &height) const
{
VectorN<double,3> base;

if (!((ArTrackSpiralInterpolator *)_track)->getKeyFrame(frame,base))
  {
  return(false);
  }

position=base[0];
distance=base[1];
height=base[2];
return(true);
}

bool // success
ArTrackSpiral::setPosition(double t, double &angle, double &distance, double &height)
{
if(!ArKeyFrameTrack::setPosition(t))
  return(false);
  
VectorN<double,3> base;

if (!((ArTrackSpiralInterpolator *)_track)->computeFrame(_position,base))
  {
  return(false);
  }

angle=base[0];
distance=base[1];
height=base[2];

// _mobile->accessBase()=Util3D::Base(true);
// // _mobile->accessBase().setSpiral(1.0,0,0,angle);
// _mobile->translate(0.0,_radius,0.0);
// _mobile->pitch(angle);
// 
// _reference->localToGlobalLocation(_mobile);

_mobile->accessBase()=_reference->accessBase();
Vector3d offset(_offset);
offset*=distance;
offset+=(_axis*height);
double x(offset.x()),y(offset.y()),z(offset.z());

_mobile->rotate(_axis.x(),_axis.y(),_axis.z(),angle);
_mobile->localToGlobalPosition(x,y,z);
_mobile->setPosition(x,y,z);

if(!_computeFrameCB.empty())
  {
  ArTrackSpiral::ComputeFrameEvent evt;
  evt.t=t;
  evt.angle=angle;
  evt.height=height;
  evt.distance=distance;
  _computeFrameCB.fire(evt);
  }
return(true);  
}
    
bool // success
ArTrackSpiral::setPosition(double t)
{
double angle, distance, height;
return(ArTrackSpiral::setPosition(t, angle, distance, height));
}

    
//------------------------- Forbiden operations ------------------------------  


// Save/Load to XML
void
ArTrackSpiral::_writeXML(ArRef<XmlNode> node) const
{
ArKeyFrameTrack::_writeXML(node);
node->setName("TrackSpiral");
node->setPropertyReal("vx",_axis.x());
node->setPropertyReal("vy",_axis.y());
node->setPropertyReal("vz",_axis.z());
node->setPropertyReal("ox",_offset.x());
node->setPropertyReal("oy",_offset.y());
node->setPropertyReal("oz",_offset.z());

}

bool
ArTrackSpiral::_readXML(ArRef<XmlNode> node)
{
if(!ArKeyFrameTrack::_readXML(node))
  return(false);
  
if (node->getName()=="TrackSpiral")
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
