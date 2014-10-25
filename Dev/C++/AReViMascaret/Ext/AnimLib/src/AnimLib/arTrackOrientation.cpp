//----------------------------------------------------------------------------
// @(#) ENIB/LI2 (c) - AnimLib
//----------------------------------------------------------------------------
// package     : AnimLib
// file        : arTrackBase3D.cpp
// description : Class describing a  ArTrackOrientation
// author(s)   : CAZEAUX Eric
// mail        : cazeaux@enib.fr
//----------------------------------------------------------------------------
// last update : 01/12/2005
//----------------------------------------------------------------------------

#include "AnimLib/arTrackOrientation.h"
#include "AReVi/Contrib/xmlParser.h"

#include "AnimLib/keyFrameTrackNurbs.h"

using namespace AReVi;

namespace AnimLib {

AR_CLASS_DEF(ArTrackOrientation,ArKeyFrameTrack)

typedef KeyFrameTrackNurbs<double,4> ArTrackOrientationInterpolator;

//----------------------------------------------------------------------------
// CLASS       : ArTrackOrientation
//----------------------------------------------------------------------------

//-------- Construction / Destruction ----------------------------------------

ArTrackOrientation::ArTrackOrientation(ArCW & arCW) // Protected
  : ArKeyFrameTrack(arCW),
    _computeFrameCB(thisPtr())
{
_initTrack(new ArTrackOrientationInterpolator());
}

ArTrackOrientation::~ArTrackOrientation(void)
{
// Nothing to be done here!
}
  
//---------------------------- Public methods --------------------------------

    
void
ArTrackOrientation::setKeyFrame(unsigned frame, const Vector3d &vector, double angle)
{
VectorN<double,4> base;
Quaterniond q(vector,angle);
base[0]=q.q0();
base[1]=q.q1();
base[2]=q.q2();
base[3]=q.q3();

/*double s = sin(angle / 2);
base[0] = cos(angle / 2);
base[1] = vector.x() * s;
base[2] = vector.y() * s;
base[3] = vector.z() * s;
  */
((ArTrackOrientationInterpolator *)_track)->setKeyFrame(frame,base);
}

bool // success
ArTrackOrientation::getKeyFrame(unsigned frame, Vector3d &vectorOut, double &angleOut) const
{
VectorN<double,4> base;

if (!((ArTrackOrientationInterpolator *)_track)->getKeyFrame(frame,base))
  {
  return(false);
  }

Quaterniond q(base[0],base[1],base[2],base[3]);
q.getAxisAngle(vectorOut,angleOut);
return(true);
}
/*
bool // success
ArTrackOrientation::computeFrame(double t, Vector3d &vectorOut, double &angleOut)
{
VectorN<double,4> base;

if (!((ArTrackOrientationInterpolator *)_track)->computeFrame(t,base))
  {
  return(false);
  }
Quaterniond q(base[0],base[1],base[2],base[3]);
q.getAxisAngle(vectorOut,angleOut);
return(true);  
}
    
  */

    
void
ArTrackOrientation::setKeyFrame(unsigned frame, const Quaterniond &orientation)
{
VectorN<double,4> base;
base[0]=orientation.q0();
base[1]=orientation.q1();
base[2]=orientation.q2();
base[3]=orientation.q3();

((ArTrackOrientationInterpolator *)_track)->setKeyFrame(frame,base);
}

bool // success
ArTrackOrientation::getKeyFrame(unsigned frame, Quaterniond &orientationOut) const
{
VectorN<double,4> base;

if (!((ArTrackOrientationInterpolator *)_track)->getKeyFrame(frame,base))
  {
  return(false);
  }

orientationOut=Quaterniond(base[0],base[1],base[2],base[3]);
return(true);
}



bool // success
ArTrackOrientation::setPosition(double t, Quaterniond &orientationOut)
{
if(!ArKeyFrameTrack::setPosition(t))
  return(false);
  
VectorN<double,4> base;

if (!((ArTrackOrientationInterpolator *)_track)->computeFrame(_position,base))
  {
  return(false);
  }
orientationOut=Quaterniond(base[0],base[1],base[2],base[3]);
if(!_computeFrameCB.empty())
  {
  ArTrackOrientation::ComputeFrameEvent evt;
  evt.t=t;
  evt.orientation=orientationOut;
  _computeFrameCB.fire(evt);
  }
  
return(true);  
}
    
    
bool // success
ArTrackOrientation::setPosition(double t)
{
Quaterniond orientation;
return(ArTrackOrientation::setPosition(t, orientation));
}
  
    
//------------------------- Forbiden operations ------------------------------  


// Save/Load to XML
void
ArTrackOrientation::_writeXML(ArRef<XmlNode> node) const
{
node->setName("TrackOrientation");

node->setPropertyReal("duration",_track->getDuration());
node->setPropertyInteger("degree",_track->getDegree());
node->setPropertyInteger("dimension",4);

ArTrackOrientationInterpolator *track=(ArTrackOrientationInterpolator *)_track;
StlVector<unsigned> keysPositions;
track->getKeysPositions(keysPositions);
VectorN<double,4> key;

for (unsigned i=0;i<keysPositions.size();i++)
  {
  track->getKeyFrame(keysPositions[i],key );
  
  ArRef<XmlNode> keyFrame=node->addChild("KeyFrameOrientation");
  keyFrame->setPropertyInteger("frame",keysPositions[i]);
  
  Quaterniond q(key[0],key[1],key[2],key[3]);
  Vector3d axis;
  double angle;
  q.getAxisAngle(axis,angle);
  keyFrame->setPropertyReal("vx",axis.x());
  keyFrame->setPropertyReal("vy",axis.y());
  keyFrame->setPropertyReal("vz",axis.z());
  keyFrame->setPropertyReal("angle",angle);
  
  }
}

bool
ArTrackOrientation::_readXML(ArRef<XmlNode> node)
{
if (node->getName()!="TrackOrientation")
  return(ArKeyFrameTrack::_readXML(node));
  
double duration;
int degree;
node->getPropertyReal("duration",duration);
node->getPropertyInteger("degree",degree);
_track->setDuration(duration);
_track->setDegree(degree);

int dim;
if (!node->getPropertyInteger("dimension",dim))
  {
  cerr << "Missing dimensions specification" << endl;
  return(false);
  }
if (dim!=4)
  {
  cerr << "Bad track dimension " << dim << "!=4" << endl;
  return(false);
  }
  
_track->clearKeyFrames();
    
ArRef<XmlNode> keyFrame=node->getFirstChild();   
while(keyFrame)
  {
  if (keyFrame->getName()=="KeyFrameOrientation")
    {
    int frame;
    if (!keyFrame->getPropertyInteger("frame",frame))
      {
      cerr << "Invalid keyFrame misssing frame info"<< endl;
      return(false);
      }
      
    
    double angle;
    double xx,yy,zz;
    bool ok=true;
    if(!keyFrame->getPropertyReal("vx",xx))
      ok = false;
    if(!keyFrame->getPropertyReal("vy",yy))
      ok = false;
    if(!keyFrame->getPropertyReal("vz",zz))
      ok = false;
    if(!keyFrame->getPropertyReal("angle",angle))
      ok = false;
    
    if(!ok)
    {
        cerr << "Error reading orientation infos (vx,vy,vz,angle)" << endl;
        return(false);
    }
    
    Vector3d axis(xx,yy,zz);
    
/*    if (!keyFrame->getPropertyReal("vx",axis.accessVector()[0]) ||
        !keyFrame->getPropertyReal("vy",axis.accessVector()[1]) ||
        !keyFrame->getPropertyReal("vz",axis.accessVector()[2]) ||
        !keyFrame->getPropertyReal("angle",angle))
               
       {
        cerr << "Error reading orientation infos (vx,vy,vz,angle)" << endl;
        return(false);
       }*/
    
    setKeyFrame(frame,axis,angle);
    //cerr << "kf=" << frame << "   axis=" << axis << "   angme=" << angle << endl;
    }
  keyFrame=keyFrame->getNext();
  }
return(true);
}    




} // end of namespace AnimLib

//---End-Of-File---------------------------------------------------------------
