//----------------------------------------------------------------------------
// @(#) ENIB/LI2 (c) - AnimLib
//----------------------------------------------------------------------------
// package     : AnimLib
// file        : KeyFrameTrackSwitch.cpp
// description : Class describing a  KeyFrameTrackSwitch
// author(s)   : CAZEAUX Eric
// mail        : cazeaux@enib.fr
//----------------------------------------------------------------------------
// last update : 14/12/2005
//----------------------------------------------------------------------------

#include "AnimLib/keyFrameTrackSwitch.h"
#include "AReVi/Contrib/xmlParser.h"

using namespace AReVi;

namespace AnimLib {

//----------------------------------------------------------------------------
// CLASS       : KeyFrameTrackSwitch<T>
//----------------------------------------------------------------------------


//------------------------- Forbiden operations ------------------------------  

// STRING specialisation
// Save/Load to XML
template<>
void
KeyFrameTrackSwitch<StlString>::_writeXML(ArRef<XmlNode> node) const
{
KeyFrameTrack::_writeXML(node);

node->setProperty("dataType","string");

KeyFrameConstIterator it=_keyFrames.begin(), it_end=_keyFrames.end();
while(it!=it_end)
  {
  ArRef<XmlNode> keyFrame=node->addChild("KeyFrame");
  keyFrame->setPropertyInteger("frame",it->first);
  keyFrame->setProperty("value",it->second);
  it++;
  }
}


template<>
bool
KeyFrameTrackSwitch<StlString>::_readXML(ArRef<XmlNode> node)
{
if (!KeyFrameTrack::_readXML(node))
  return(false);

StlString dataType;
if (!node->getPropertyString("dataType",dataType))
  {
  cerr << "Missing dataType specification" << endl;
  return(false);
  }
if (dataType!="string")
  {
  cerr << "Bad track data Type " << dataType << "!=" << "string" << endl;
  return(false);
  }
  
_keyFrames.clear();

ArRef<XmlNode> keyFrame=node->getFirstChild();   
while(keyFrame)
  {
  if (keyFrame->getName()=="KeyFrame")
    {
    int frame;
    if (!keyFrame->getPropertyInteger("frame",frame))
      {
      cerr << "Invalid keyFrame misssing frame info"<< endl;
      return(false);
      }
    
    StlString value;
    if (!keyFrame->getPropertyString("value",value))
      {
      cerr << "Error reading dataValue for frame " << frame << "." << endl;
      return(false);
      }
    _keyFrames[frame]=value;
    }
  keyFrame=keyFrame->getNext();
  }
_needRebuild();
return(true);
}

} // end of namespace AnimLib

//---End-Of-File---------------------------------------------------------------
