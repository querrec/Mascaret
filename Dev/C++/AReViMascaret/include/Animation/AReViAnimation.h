#ifndef AREVIANIMATION_H
#define AREVIANIMATION_H
#include "AReVi/arObject.h"
#include "areviMascaret_plateform.h"
#include "VRPlateform/AReViShapeSpecification.h"

#include <boost/filesystem.hpp>

namespace AReViMascaret
{
using AReVi::ArRef;
using AReVi::ArObject;
using AReVi::StlString;

class AREVI_MASCARET_API AReViAnimation : public ArObject
{
public :
  AR_CLASS(AReViAnimation)
  AR_CONSTRUCTOR(AReViAnimation)

  void setName(StlString name) {_name = name;}
  StlString getName() {return _name;}

  virtual void setPosition(double ){;}
  virtual double getDuration(void){return 0;}

  inline void setURL(StlString url) {_url = url;}
  inline StlString getURL() { return _url;}

  void setShape(shared_ptr<AReViShapeSpecification> shape);

protected :
  StlString _name;
  shared_ptr<AReViShapeSpecification> _shape;
  StlString _url;
};
}

#endif
