//----------------------------------------------------------------------------
// @(#) ENIB/LI2 (c) - AnimLib
//----------------------------------------------------------------------------
// package     : AnimLib
// file        : utils.h => reader ...
// description : Class describing a keyFrameTrack, a keyFrameTrackReal, keyFrameTrackReal3
// author(s)   : CAZEAUX Eric
// mail        : cazeaux@enib.fr
//----------------------------------------------------------------------------
// last update : 02/12/2005
//----------------------------------------------------------------------------
#ifndef ANIMLIB_UTILS_H
#define ANIMLIB_UTILS_H 1

//--- Imported classes -------------------------------------------------------
#include "AnimLib/animLib_plateform.h"
#include "AReVi/Utils/stlMap_priv.h"
#include "AReVi/Contrib/xmlParser.h"

using namespace AReVi;

namespace AnimLib {

  class ArKeyFrameTrack;
  
//----------------------------------------------------------------------------
// CLASS       : AnimLibUtils {abstract}
//----------------------------------------------------------------------------
/*! \class AnimLibUtils
  * \author Eric Cazeaux
  * \date 02-12-2005
  *
  * Utility class, containing a reader, writer for tracks.
  */

class ANIMLIB_API AnimLibUtils
{
public:
  
  static 
  bool  // success
  readTracks(ArRef<XmlNode> tracksNode, StlMap<StlString, ArRef<ArKeyFrameTrack> > & tracksOut);
  
  static 
  ArRef<ArKeyFrameTrack>  // ArKeyFrameTrack::nullRef() il fail
  readTrack(ArRef<XmlNode> trackNode);
  
  // Warning : For this method, trackOut must of appropriated type.
  static 
  bool // success
  readTrack(ArRef<XmlNode> trackNode, ArRef<ArKeyFrameTrack> trackOut);
  
  static 
  ArRef<XmlNode> // success if != null
  writeTracks(const StlMap<StlString, ArRef<ArKeyFrameTrack> > & tracksOut, ArRef<XmlNode> parent);
  
  static 
  ArRef<XmlNode> // success if != null
  writeTrack(ArConstRef<ArKeyFrameTrack> track, ArRef<XmlNode> parent, const StlString &ID);
  
};

} // end of namespace AnimLib

//-------- Inline source code ------------------------------------------------



#endif // end of ANIMLIB_UTILS_H
//---End-Of-File---------------------------------------------------------------
