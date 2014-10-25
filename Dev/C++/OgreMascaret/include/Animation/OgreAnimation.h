#ifndef OGREANIMATION_H
#define OGREANIMATION_H

#include "ogreMascaret_plateform.h"
#include "VRPlateform/OgreShapeSpecification.h"
#include <boost/filesystem.hpp>

namespace OgreMascaret
{

class OGRE_MASCARET_API OgreAnimation
{
public :
  OgreAnimation();
  virtual ~OgreAnimation();

  void setName(string name) {_name = name;}
  string getName() {return _name;}

  virtual void setPosition(double ){;}
  virtual double getDuration(void){return 0;}

  inline void setURL(string url) {_url = url;}
  inline string getURL() { return _url;}

  void setShape(shared_ptr<OgreShapeSpecification> shape);

protected :
  string _name;
  shared_ptr<OgreShapeSpecification> _shape;
  string _url;
};
}

#endif
