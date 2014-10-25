//----------------------------------------------------------------------------
// @(#) ENIB/LI2 (c) - AnimLib
//----------------------------------------------------------------------------
// package     : AnimLib
// file        : arTrackBase3D.cpp
// description : Class describing a  ArTrackScale
// author(s)   : CAZEAUX Eric
// mail        : cazeaux@enib.fr
//----------------------------------------------------------------------------
// last update : 12/12/2005
//----------------------------------------------------------------------------

#include "AnimLib/arTrackScale.h"
#include "AReVi/Shapes/shape3D.h"
#include "AReVi/Shapes/shapePart3D.h"
#include "AReVi/Lib3D/transform3D.h"


using namespace AReVi;

namespace AnimLib {

AR_CLASS_DEF(ArTrackScale,ArKeyFrameTrack)

//----------------------------------------------------------------------------
// CLASS       : ArTrackScale
//----------------------------------------------------------------------------

//-------- Construction / Destruction ----------------------------------------

ArTrackScale::ArTrackScale(ArCW & arCW) // Protected
  : ArKeyFrameTrack(arCW),
    _reference(1),
    _shapes(),
    _parts(),
    _persistent(Transform3D::NEW()),
    _computeFrameCB(thisPtr())
{
_initTrack(new ArTrackScaleInterpolator());
}

ArTrackScale::~ArTrackScale(void)
{
// Nothing to be done here!
}
  
//---------------------------- Public methods --------------------------------

  
//------------------------ Reference/Mobile init -----------------------------
  
// Scale will be applied to all shape parts3D.
void
ArTrackScale::initScaleShapes(const StlVector<ArRef<Shape3D> > &shapes)
{
_shapes=shapes;
}

void
ArTrackScale::initScaleParts(const StlVector<ArRef<ShapePart3D> > &parts)
{
_parts=parts;
}

// Set material reference
void
ArTrackScale::initReference(const Util3D::Transform &transformRef)
{
_reference=transformRef;
} 
  

//------------------------ Reference/Mobile access ----------------------------  

Util3D::Transform &
ArTrackScale::accessReference(void)
{
return(_reference);
}

StlVector<ArRef<Shape3D> > &
ArTrackScale::accessShapes(void)
{
return(_shapes);
}

StlVector<ArRef<ShapePart3D> > &
ArTrackScale::accessParts(void)
{
return(_parts);
}


//--------------------------- Set Base3D ----------------------------------

void
ArTrackScale::setKeyFrame(unsigned frame, const Vector3d &scale)
{
VectorN<double,3> base;

base[0]=scale.x();
base[1]=scale.y();
base[2]=scale.z();

((ArTrackScaleInterpolator *)_track)->setKeyFrame(frame,base);
}

bool // success
ArTrackScale::getKeyFrame(unsigned frame, Vector3d &scale) const
{
VectorN<double,3> base;

if (!((ArTrackScaleInterpolator *)_track)->getKeyFrame(frame,base))
  {
  return(false);
  }

scale=Vector3d(base[0],base[1],base[2]);
return(true);
}

bool // success
ArTrackScale::setPosition(double t, Vector3d &scale)
{
if(!ArKeyFrameTrack::setPosition(t))
  return(false);
  
VectorN<double,3> base;

if (!((ArTrackScaleInterpolator *)_track)->computeFrame(_position,base))
  {
  return(false);
  }
scale=Vector3d(base[0],base[1],base[2]);


_persistent->accessTransformation()=_reference;
_persistent->preScale(base[0],base[1],base[2]);


for (unsigned i=_parts.size();i--;)
  {
  _parts[i]->writeTransformation(_persistent);
  }
for (unsigned i=_shapes.size();i--;)
  {
  _shapes[i]->writeTransformation(_persistent);
  }

if(!_computeFrameCB.empty())
  {
  ArTrackScale::ComputeFrameEvent evt;
  evt.t=t;
  evt.scale=scale;
  _computeFrameCB.fire(evt);
  }
return(true);  
}
    
bool // success
ArTrackScale::setPosition(double t)
{
Vector3d scale;
return(ArTrackScale::setPosition(t, scale));
}

    
//------------------------- Forbiden operations ------------------------------  


// Save/Load to XML
void
ArTrackScale::_writeXML(ArRef<XmlNode> node) const
{
ArKeyFrameTrack::_writeXML(node);
node->setName("TrackScale");
ArRef<XmlNode> reference=node->addChild("Reference");

for (unsigned i=0;i<4;i++)
  for (unsigned j=0;j<4;j++)
    {
    reference->setPropertyReal("m"+uintToStr(i)+uintToStr(j), _reference.matrix[(i*4)+j]);
    }
}

bool
ArTrackScale::_readXML(ArRef<XmlNode> node)
{
if(!ArKeyFrameTrack::_readXML(node))
  return(false);
  
_reference=Util3D::Transform(1);

if (node->getName()=="TrackScale")
  {
  ArRef<XmlNode> reference=node->getChild("Reference");
  if (reference)
    {
    Util3D::Transform tr(1);
    for (unsigned i=0;i<4;i++)
      for (unsigned j=0;j<4;j++)
        {
        if (!reference->getPropertyReal("m"+uintToStr(i)+uintToStr(j), tr.matrix[(i*4)+j]))
          return(false);
        }
     initReference(tr);
    }
  }
  
return(true);
}


} // end of namespace AnimLib

//---End-Of-File---------------------------------------------------------------
