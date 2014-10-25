#include "Animation/OgreAnimation.h"

namespace OgreMascaret
{

OgreAnimation::OgreAnimation()
{
}

OgreAnimation::~OgreAnimation() {}

void OgreAnimation::setShape(shared_ptr<OgreShapeSpecification> shape)
{
   _shape = shape;
}

}
