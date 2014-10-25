//----------------------------------------------------------------------------
// @(#) ENIB/LI2 (c) - AnimLib
//----------------------------------------------------------------------------
// package     : AnimLib
// file        : arTrackBase3D.cpp
// description : Class describing a  ArTrackTexture
// author(s)   : CAZEAUX Eric
// mail        : cazeaux@enib.fr
//----------------------------------------------------------------------------
// last update : 12/12/2005
//----------------------------------------------------------------------------

#include "AnimLib/arTrackTexture.h"
#include "AReVi/Shapes/surface3D.h"
#include "AReVi/Lib3D/urlTexture.h"


using namespace AReVi;

namespace AnimLib {

AR_CLASS_DEF(ArTrackTexture,ArKeyFrameTrack)

//----------------------------------------------------------------------------
// CLASS       : ArTrackTexture
//----------------------------------------------------------------------------

//-------- Construction / Destruction ----------------------------------------

ArTrackTexture::ArTrackTexture(ArCW & arCW) // Protected
  : ArKeyFrameTrack(arCW),
    _mobiles(),
    _cache(),
    _computeFrameCB(thisPtr())
{
_initTrack(new ArTrackTextureInterpolator());
_cache[""]=URLTexture::nullRef();
}

ArTrackTexture::~ArTrackTexture(void)
{
// Nothing to be done here!
}
  
//---------------------------- Public methods --------------------------------

  
//------------------------ Reference/Mobile init -----------------------------

// Texture will be applied to all shape parts3D.
void
ArTrackTexture::initTexture(const StlVector<ArRef<Surface3D> > &mobiles)
{
_mobiles=mobiles;
}

//------------------------ Reference/Mobile access ----------------------------  

StlVector<ArRef<Surface3D> > &
ArTrackTexture::accessMobiles(void)
{
return(_mobiles);
}


//--------------------------- Set Base3D ----------------------------------

void
ArTrackTexture::setKeyFrame(unsigned frame, ArRef<URLTexture> texture)
{
if (texture.valid())
  {
  _cache[texture->getURL()]=texture;
  ((ArTrackTextureInterpolator *)_track)->setKeyFrame(frame,texture->getURL());
  }
else
  {
  ((ArTrackTextureInterpolator *)_track)->setKeyFrame(frame,"");
  }
}

bool // success
ArTrackTexture::getKeyFrame(unsigned frame, ArRef<URLTexture> &texture) const
{
StlString value;

if (!((ArTrackTextureInterpolator *)_track)->getKeyFrame(frame,value))
  {
  return(false);
  }
  
texture=ar_const_cast(thisRef())->_accessCache(value);

return(true);
}

bool //success
ArTrackTexture::setPosition(double t, ArRef<URLTexture> &texture)
{
if(!ArKeyFrameTrack::setPosition(t))
  return(false);
  
StlString value;

if (!((ArTrackTextureInterpolator *)_track)->computeFrame(_position,value))
  {
  return(false);
  }
// cerr << "ArTrackTexture::setPosition   t=" << t << " valu=" << value << endl;
texture=_accessCache(value);

for (unsigned i=_mobiles.size();i--;)
  {
  _mobiles[i]->setTexture(texture);
  }

  
if(!_computeFrameCB.empty())
  {
  ArTrackTexture::ComputeFrameEvent evt;
  evt.t=t;
  evt.texture=texture;
  _computeFrameCB.fire(evt);
  }
return(true);  
}
    
bool // success
ArTrackTexture::setPosition(double t)
{
ArRef<URLTexture> texture;
return(ArTrackTexture::setPosition(t, texture));
}

    
//------------------------- Forbiden operations ------------------------------  


ArRef<URLTexture>
ArTrackTexture::_accessCache(const StlString &url)
{
StlMap<StlString, ArRef<URLTexture> >::iterator it=_cache.find(url);
if (it==_cache.end()) // add it
  {
  _cache[url]=URLTexture::NEW(url,true,false);
  return(_cache[url]);
  }
else
  return(it->second);
}

  
} // end of namespace AnimLib

//---End-Of-File---------------------------------------------------------------
