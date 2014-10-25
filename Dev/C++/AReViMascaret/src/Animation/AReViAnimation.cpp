#include "AReVi/Contrib/xmlParser.h"
#include "AReVi/activity.h"
#include "AReVi/VRML/vrmlShape3D.h"
#include "AReVi/Lib3D/transform3D.h"

#include "Animation/AReViAnimation.h"

using namespace AReVi;

#include "boost/filesystem.hpp" 
namespace AReViMascaret
{
AR_CLASS_DEF(AReViAnimation,ArObject)

AReViAnimation::AReViAnimation(ArCW & arCW) :
  ArObject(arCW)
{
}

AReViAnimation::~AReViAnimation() {}

void AReViAnimation::setShape(shared_ptr<AReViShapeSpecification> shape)
{
   _shape = shape;
}

}
