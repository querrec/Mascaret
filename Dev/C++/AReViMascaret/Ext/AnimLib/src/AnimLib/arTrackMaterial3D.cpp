//----------------------------------------------------------------------------
// @(#) ENIB/LI2 (c) - AnimLib
//----------------------------------------------------------------------------
// package     : AnimLib
// file        : arTrackBase3D.cpp
// description : Class describing a  ArTrackMaterial3D
// author(s)   : CAZEAUX Eric
// mail        : cazeaux@enib.fr
//----------------------------------------------------------------------------
// last update : 12/12/2005
//----------------------------------------------------------------------------

#include "AnimLib/arTrackMaterial3D.h"
#include "AReVi/Shapes/surface3D.h"
#include "AReVi/Lib3D/material3D.h"


using namespace AReVi;

namespace AnimLib {

AR_CLASS_DEF(ArTrackMaterial3D,ArKeyFrameTrack)

//----------------------------------------------------------------------------
// CLASS       : ArTrackMaterial3D
//----------------------------------------------------------------------------

//-------- Construction / Destruction ----------------------------------------

ArTrackMaterial3D::ArTrackMaterial3D(ArCW & arCW) // Protected
  : ArKeyFrameTrack(arCW),
    _reference(),
    _mobiles(),
    _persistent(Material3D::NEW()),
    _computeFrameCB(thisPtr())
{
_initTrack(new ArTrackMaterial3DInterpolator());
}

ArTrackMaterial3D::~ArTrackMaterial3D(void)
{
// Nothing to be done here!
}
  
//---------------------------- Public methods --------------------------------

  
//------------------------ Reference/Mobile init -----------------------------

// Material3D will be applied to all shape parts3D.
void
ArTrackMaterial3D::initMaterial3D(const StlVector<ArRef<Surface3D> > &mobiles)
{
_mobiles=mobiles;
}

// Set material reference
void
ArTrackMaterial3D::initReference(const Util3D::Material &matRef)
{
_reference=matRef;
} 
  

//------------------------ Reference/Mobile access ----------------------------  

Util3D::Material &
ArTrackMaterial3D::accessReference(void)
{
return(_reference);
}

StlVector<ArRef<Surface3D> > &
ArTrackMaterial3D::accessMobiles(void)
{
return(_mobiles);
}


//--------------------------- Set Base3D ----------------------------------

void
ArTrackMaterial3D::setKeyFrame(unsigned frame, const Util3D::Material &material)
{
VectorN<double,12> base;

unsigned index=0;
base[index++]=material.getAmbientIntensity();

base[index++]=material.getDiffuseColor()[0];
base[index++]=material.getDiffuseColor()[1];
base[index++]=material.getDiffuseColor()[2];

base[index++]=material.getTransparency();


base[index++]=material.getSpecularColor()[0];
base[index++]=material.getSpecularColor()[1];
base[index++]=material.getSpecularColor()[2];

base[index++]=material.getShininess();


base[index++]=material.getEmissiveColor()[0];
base[index++]=material.getEmissiveColor()[1];
base[index++]=material.getEmissiveColor()[2];
((ArTrackMaterial3DInterpolator *)_track)->setKeyFrame(frame,base);
}

bool // success
ArTrackMaterial3D::getKeyFrame(unsigned frame, Util3D::Material &material) const
{
VectorN<double,12> base;

if (!((ArTrackMaterial3DInterpolator *)_track)->getKeyFrame(frame,base))
  {
  return(false);
  }
material.setAmbientIntensity(base[0]);
material.setDiffuseColor(base[1],base[2],base[3]);

material.setTransparency(base[4]);


material.setSpecularColor(base[5],base[6],base[7]);

material.setShininess(base[8]);


material.setEmissiveColor(base[9],base[10],base[11]);
return(true);
}

bool // success
ArTrackMaterial3D::setPosition(double t, Util3D::Material &material)
{
if(!ArKeyFrameTrack::setPosition(t))
  return(false);
  
VectorN<double,12> base;

if (!((ArTrackMaterial3DInterpolator *)_track)->computeFrame(_position,base))
  {
  return(false);
  }

material.setAmbientIntensity(_reference.getAmbientIntensity()+base[0]);
material.setDiffuseColor(_reference.getDiffuseColor()[0]+base[1],_reference.getDiffuseColor()[1]+base[2],_reference.getDiffuseColor()[2]+base[3]);
material.setTransparency(_reference.getTransparency()+base[4]);
material.setSpecularColor(_reference.getSpecularColor()[0]+base[5],_reference.getSpecularColor()[1]+base[6],_reference.getSpecularColor()[2]+base[7]);
material.setShininess(_reference.getShininess()+base[8]);
material.setEmissiveColor(_reference.getEmissiveColor()[0]+base[9],_reference.getEmissiveColor()[1]+base[10],_reference.getEmissiveColor()[2]+base[11]);

_persistent->accessMaterial()=material;

for (unsigned i=_mobiles.size();i--;)
  {
  _mobiles[i]->writeMaterial(_persistent);
  }

if(!_computeFrameCB.empty())
  {
  ArTrackMaterial3D::ComputeFrameEvent evt;
  evt.t=t;
  evt.material=material;
  _computeFrameCB.fire(evt);
  }
return(true);  
}
    
bool // success
ArTrackMaterial3D::setPosition(double t)
{
Util3D::Material material;
return(ArTrackMaterial3D::setPosition(t, material));
}

    
//------------------------- Forbiden operations ------------------------------  


// Save/Load to XML
void
ArTrackMaterial3D::_writeXML(ArRef<XmlNode> node) const
{
// ArKeyFrameTrack::_writeXML(node);
node->setName("TrackMaterial3D");
ArRef<XmlNode> reference=node->addChild("Reference");
_writeMaterialXML(reference,_reference);

node->setPropertyReal("duration",_track->getDuration());
node->setPropertyInteger("degree",_track->getDegree());
node->setPropertyInteger("dimension",12);

ArTrackMaterial3DInterpolator *track=(ArTrackMaterial3DInterpolator *)_track;
StlVector<unsigned> keysPositions;
track->getKeysPositions(keysPositions);
Util3D::Material material;

for (unsigned i=0;i<keysPositions.size();i++)
  {
  getKeyFrame(keysPositions[i],material);
  
  ArRef<XmlNode> keyFrame=node->addChild("KeyFrameMaterial");
  keyFrame->setPropertyInteger("frame",keysPositions[i]);
  
  _writeMaterialXML(keyFrame,material);
  }
}

void
ArTrackMaterial3D::_writeMaterialXML(ArRef<XmlNode> node, const Util3D::Material &material) const
{
node->setPropertyReal("ambient",material.getAmbientIntensity());
node->setPropertyReal("transparency",material.getTransparency());
node->setPropertyReal("shininess",material.getShininess());
node->setPropertyReal("diffuseR",material.getDiffuseColor()[0]);
node->setPropertyReal("diffuseG",material.getDiffuseColor()[1]);
node->setPropertyReal("diffuseB",material.getDiffuseColor()[2]);
node->setPropertyReal("specularR",material.getSpecularColor()[0]);
node->setPropertyReal("specularG",material.getSpecularColor()[1]);
node->setPropertyReal("specularB",material.getSpecularColor()[2]);
node->setPropertyReal("emissiveR",material.getEmissiveColor()[0]);
node->setPropertyReal("emissiveG",material.getEmissiveColor()[1]);
node->setPropertyReal("emissiveB",material.getEmissiveColor()[2]);
}

bool
ArTrackMaterial3D::_readXML(ArRef<XmlNode> node)
{
ArConstRef<ArClass> className=ArClass::find(node->getProperty("type"));

if (!className->isA(ArTrackMaterial3D::CLASS()))
  {
  cerr << ("Invalid type '"+ node->getProperty("type") + "'") << endl;
  return(false);
  }

ArRef<XmlNode> reference=node->getChild("Reference");
if (reference)
  {
  _readMaterialXML(reference,_reference);
  }
else
  _reference=Util3D::Material();
  
if (node->getName()!="TrackMaterial3D")
  {
//   cerr << "Parse with default mode" << endl;
  return(ArKeyFrameTrack::_readXML(node));
  }

// Parse data of tracks
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
if (dim!=12)
  {
  cerr << "Bad material3D track dimension " << dim << "!=12" << endl;
  return(false);
  }
  
_track->clearKeyFrames();
    
ArRef<XmlNode> keyFrame=node->getFirstChild();   
while(keyFrame)
  {
  if (keyFrame->getName()=="KeyFrameMaterial")
    {
    int frame;
    if (!keyFrame->getPropertyInteger("frame",frame))
      {
      cerr << "Invalid keyFrame misssing frame info"<< endl;
      return(false);
      }
      
    Util3D::Material material;
    
    if (!_readMaterialXML(keyFrame,material))
       {
        cerr << "Error reading material infos (ambient, transparency, shininess, diffuseR,diffuseG,diffuseB,specularR,specularG,specularB,emissiveR,emissiveG,emissiveB)" << endl;
        return(false);
       }
    setKeyFrame(frame,material);
    }
  keyFrame=keyFrame->getNext();
  }
return(true);  
}


bool
ArTrackMaterial3D::_readMaterialXML(ArRef<XmlNode> node, Util3D::Material &materialOut) const
{
double r,g,b;
if (node->getPropertyReal("shininess",r))
  materialOut.setShininess(r);
else
  materialOut.setShininess(0);  
  
if (node->getPropertyReal("transparency",r))
  materialOut.setTransparency(r);
else
  materialOut.setTransparency(0);
  
if (node->getPropertyReal("ambient",r))
  materialOut.setAmbientIntensity(r);
else
  materialOut.setAmbientIntensity(0);
  
if (node->getPropertyReal("diffuseR",r) && node->getPropertyReal("diffuseG",g) && node->getPropertyReal("diffuseB",b))
  materialOut.setDiffuseColor(r,g,b);
else
  materialOut.setDiffuseColor(0,0,0);
  
if (node->getPropertyReal("emissiveR",r) && node->getPropertyReal("emissiveG",g) && node->getPropertyReal("emissiveB",b))
  materialOut.setEmissiveColor(r,g,b);
else
  materialOut.setEmissiveColor(0,0,0);
  
if (node->getPropertyReal("specularR",r) && node->getPropertyReal("specularG",g) && node->getPropertyReal("specularB",b))
  materialOut.setSpecularColor(r,g,b);
else
  materialOut.setSpecularColor(0,0,0);
  
return(true);  
}

} // end of namespace AnimLib

//---End-Of-File---------------------------------------------------------------
