//----------------------------------------------------------------------------
// @(#) ENIB/LI2 (c) - AnimLib
//----------------------------------------------------------------------------
// package     : AnimLib
// file        : utils.cpp => reader ...
// description : Utils
// author(s)   : CAZEAUX Eric
// mail        : cazeaux@enib.fr
//----------------------------------------------------------------------------
// last update : 02/12/2005
//----------------------------------------------------------------------------

#include "AnimLib/utils.h"
#include "AnimLib/arKeyFrameTrack.h"
#include "AnimLib/keyFrameTrack.h"
#include "AReVi/arClass.h"

using namespace AnimLib;

namespace AnimLib {

//----------------------------------------------------------------------------
// CLASS       : AnimLibUtils {abstract}
//----------------------------------------------------------------------------


bool  // success
AnimLibUtils::readTracks(ArRef<XmlNode> tracksNode, StlMap<StlString, ArRef<ArKeyFrameTrack> > & tracksOut)
{
ArRef<XmlNode> trackNode=tracksNode->getFirstChild();

while(trackNode)
  {
  if (trackNode->getName().substr(0,5)=="Track")
    {
    ArRef<ArKeyFrameTrack> track=readTrack(trackNode);
    StlString id;
    trackNode->getPropertyString("ID",id);
    if (track.valid() && trackNode->getPropertyString("ID",id))
      {
      tracksOut[id]=track;
      }
      else cerr << "Erreur Track : " << id << endl;
    }
  trackNode=trackNode->getNext();
  }
  
return(true);
}
  
ArRef<ArKeyFrameTrack>  // ArKeyFrameTrack::nullRef() il fail
AnimLibUtils::readTrack(ArRef<XmlNode> trackNode)
{
StlString type;
if (!trackNode->getPropertyString("type",type))
  return(ArKeyFrameTrack::nullRef());

  
ArConstRef<ArClass> trackClass=ArClass::find(type);

if (!trackClass.valid())
  {
  cerr << "Fail to find class " << type << endl;
  return(ArKeyFrameTrack::nullRef());
  }

ArRef<ArKeyFrameTrack> track;
trackClass->newInstance(track);
if (!track.valid())
  {
  cerr << "Fail to create instance of " << type << endl;
  return(ArKeyFrameTrack::nullRef());
  }
  
if (AnimLibUtils::readTrack(trackNode, track))
  {
  return(track);
  }
else
  {
  cerr << track->getErrorMessage() << endl;
  track.destroy();
  return(ArKeyFrameTrack::nullRef());
  }
}

// Warning : For this method, trackOut must of appropriated type.
bool // success static 
AnimLibUtils::readTrack(ArRef<XmlNode> trackNode, ArRef<ArKeyFrameTrack> track)
{
if (!track->_readXML(trackNode))
  {
  cerr << "Fail parsing track infos " << endl;
  return(false);
  }
  
double vReal(0.0);
bool vBool(false);

trackNode->getPropertyBoolean("running",vBool);
if (vBool) 
  {
  trackNode->getPropertyReal("precision",vReal);
  track->start(vReal);
  }
else track->pause();

trackNode->getPropertyBoolean("repeat",vBool);
track->setRepeat(vBool);
// cerr << "repeat=" << vBool << endl;
trackNode->getPropertyReal("position",vReal);
track->setPosition(vReal);
  
return(true);
}



ArRef<XmlNode> // success if != null
AnimLibUtils::writeTracks(const StlMap<StlString, ArRef<ArKeyFrameTrack> > & tracksOut, ArRef<XmlNode> parent)
{
StlMap<StlString, ArRef<ArKeyFrameTrack> >::const_iterator it=tracksOut.begin(), it_end=tracksOut.end();
// ArRef<XmlNode> tracks=parent->addChild("Tracks");

while(it!=it_end)
  {
  ArRef<XmlNode> track=writeTrack(it->second,parent,it->first);   
  it++;
  }
  
return(parent);
}
 
ArRef<XmlNode> 
AnimLibUtils::writeTrack(ArConstRef<ArKeyFrameTrack> track, ArRef<XmlNode> parent, const StlString &ID)
{
ArRef<XmlNode> node=parent->addChild("Track");
node->setPropertyString("type",track->getClass()->getName());
node->setProperty("ID",ID);
track->_writeXML(node);
double precision(0.1);
node->setPropertyReal("position",track->getPosition());
node->setPropertyReal("running",track->getAnimated(precision));
node->getPropertyReal("precision",precision);
node->setPropertyBoolean("repeat",track->getRepeat());
return(node);
}

    
} // end of namespace AnimLib

//---End-Of-File---------------------------------------------------------------
