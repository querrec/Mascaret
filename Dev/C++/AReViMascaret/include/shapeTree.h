#include "AReVi/arSystem.h"
using namespace AReVi;
#include "AReVi/activity.h"

#include "AReVi/Lib3D/viewer3D.h"
#include "AReVi/Lib3D/simpleInteractor.h"
#include "AReVi/Lib3D/scene3D.h"
#include "AReVi/Lib3D/object3D.h"
#include "AReVi/Lib3D/boundingBox3D.h"
#include "AReVi/Lib3D/transform3D.h"
#include "AReVi/VRML/vrmlShape3D.h"

#include "AReVi/VRML/vrmlGenericInteractor.h"
#include "AReVi/VRML/vrmlInlineInteractor.h"
#include "AReVi/VRML/vrmlTransformInteractor.h"

//----------------------------------------------------------------------------

class ShapeTree : public ArObject
{
public:
  AR_CLASS(ShapeTree)
  AR_CONSTRUCTOR(ShapeTree)
  ArRef<VrmlShape3D> shape;
  ArRef<Base3D> location;
  StlVector<ArRef<ShapeTree> > sub;
};
