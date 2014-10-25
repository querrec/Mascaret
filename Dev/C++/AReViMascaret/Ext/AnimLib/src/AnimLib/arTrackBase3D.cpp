//----------------------------------------------------------------------------
// @(#) ENIB/LI2 (c) - AnimLib
//----------------------------------------------------------------------------
// package     : AnimLib
// file        : arTrackBase3D.cpp
// description : Class describing a  ArTrackBase3D
// author(s)   : CAZEAUX Eric
// mail        : cazeaux@enib.fr
//----------------------------------------------------------------------------
// last update : 01/12/2005
//----------------------------------------------------------------------------

#include "AnimLib/arTrackBase3D.h"
#include "AnimLib/arTrackOrientation.h"
#include "AnimLib/utils.h"
#include "AReVi/Lib3D/base3D.h"

using namespace AReVi;

namespace AnimLib {

AR_CLASS_DEF(ArTrackBase3D,ArTrackPosition)

//----------------------------------------------------------------------------
// CLASS       : ArTrackBase3D
//----------------------------------------------------------------------------

//-------- Construction / Destruction ----------------------------------------

ArTrackBase3D::ArTrackBase3D(ArCW & arCW) // Protected
  : ArTrackPosition(arCW),
    _reference(Base3D::NEW()),
    _mobile(Base3D::NEW()),
    _orientationTrack(ArTrackOrientation::NEW())
{
// Nothing to be done here!
}

ArTrackBase3D::~ArTrackBase3D(void)
{
// Nothing to be done here!
}
  
//---------------------------- Public methods --------------------------------

//--------------------------- Set Base3D ----------------------------------
    
// Location will be changed using reference (if specified)
void
ArTrackBase3D::initBase3D(ArRef<Base3D> reference, ArRef<Base3D> mobile)
{
if (_mobile && _reference)  
  _mobile->attachTo(_reference,Base3D::ATTACH_NONE);
  
_reference=reference;
_mobile=mobile;
//_orientationTrack->init
if (_reference)
  _mobile->attachTo(_reference);
}

// Reference is centered on mobile location, then animation will be made
// from this location. Use accessReference to move reference.
void
ArTrackBase3D::initBase3D(ArRef<Base3D> mobile)
{
if (_mobile && _reference)  
  _mobile->attachTo(_reference,Base3D::ATTACH_NONE);
  
_reference=Base3D::NEW();
_mobile=mobile;
_reference->setLocation(mobile);
_mobile->attachTo(_reference);
}

//------------------------ Tracks access ----------------------------  

ArRef<ArTrackPosition>
ArTrackBase3D::accessTrackPosition(void)
{
return(ar_down_cast<ArTrackPosition>(thisRef()));
}

ArRef<ArTrackOrientation>
ArTrackBase3D::accessTrackOrientation(void)
{
return(_orientationTrack);
}
    
//------------------------ Reference/Mobile access ----------------------------  

ArRef<Base3D>
ArTrackBase3D::accessReference(void)
{
return(_reference);
}

ArRef<Base3D>
ArTrackBase3D::accessMobile(void)
{
return(_mobile);
}

bool // success
ArTrackBase3D::setPosition(double t)
{
Vector3d position;
Quaterniond orientation;

bool newPos=ArTrackPosition::setPosition(t,position);
bool newOri=_orientationTrack->setPosition(t,orientation);

if (newPos)
  {
  if (_reference)
    _reference->localToGlobalPosition(position.accessVector()[0],position.accessVector()[1],position.accessVector()[2]),
    
  _mobile->setPosition(position.x(),position.y(),position.z());
  }
  
if (newOri)
  {
  Vector3d axis;
  double angle;
  orientation.getAxisAngle(axis,angle);
  // cerr<<"before setRotaion: "<<axis.x()<<", "<<axis.y()<<", "<<axis.z()<<"; "<<angle<<endl; 
  if (_reference)
{
  _mobile->maskMotion();
  _mobile->setRotation(axis.x(),axis.y(),axis.z(), angle);
  double x,y,z,w;
  //_mobile->extractRotation(x,y,z,w);
  ArRef<Base3D> b = Base3D::NEW();
  b->setLocation(_mobile);

  _reference->localToGlobalLocation(b);
  b->extractRotation(x,y,z,w);
  _mobile->setRotation(x,y,z,w);
  //_mobile->setOrientation(x,y,z);
  _mobile->unmaskMotion();
} else
//    _reference->localToGlobalVector(axis.accessVector()[0],axis.accessVector()[1],axis.accessVector()[2]),
   //cerr<<"setRotaion: "<<axis.x()<<", "<<axis.y()<<", "<<axis.z()<<"; "<<angle<<endl; 
  _mobile->setRotation(axis.x(),axis.y(),axis.z(), angle);
  }
return(newOri&&newPos);
}
  
  
//------------------------- Forbiden operations ------------------------------  
// write/read to XML
void
ArTrackBase3D::_writeXML(ArRef<XmlNode> node) const
{
ArRef<XmlNode> positionNode=node->addChild("TrackPosition");
ArTrackPosition::_writeXML(positionNode);
AnimLibUtils::writeTrack(_orientationTrack,node,"TrackOrientation");
}

bool
ArTrackBase3D::_readXML(ArRef<XmlNode> node)
{
ArRef<XmlNode> positionNode=node->getChild("TrackPosition");
ArRef<XmlNode> orientationNode=node->getChild("TrackOrientation");
if (positionNode.valid())
  {
  if (!ArTrackPosition::_readXML(positionNode))
    {
    setErrorMessage("Error reading TrackPosition");
    }
  }
else
  setErrorMessage("Can not find TrackPosition node");

if (orientationNode.valid())
  {
  if (!AnimLibUtils::readTrack(orientationNode,_orientationTrack))
    {
    setErrorMessage("Error reading TrackOrientation");
    }
  else
    _orientationTrack->pause(); // manager by base3D track
  }
else
  setErrorMessage("Can not find TrackOrientation node");
  
return(!fail());
}

} // end of namespace AnimLib

//---End-Of-File---------------------------------------------------------------
