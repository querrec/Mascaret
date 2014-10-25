#include "Assistance/AReViColor.h"
#include "AReVi/VRML/vrmlShape3D.h"
#include "AReVi/Lib3D/transform3D.h"
#include "AReVi/Lib3D/urlSound3D.h"
#include "AReVi/Shapes/box3D.h"

using namespace AReVi;

namespace AReViMascaret
{

AReViColor::AReViColor()
{}

AReViColor::~AReViColor()
{}

AReViColor::AReViColor(float a) : Color(a)
{}

AReViColor::AReViColor(int r,int g,int b) : Color(r,g,b)
{}

AReViColor::AReViColor(StlMap<StlString,ArRef<Material3D> > surfaceMaterialMap):_surfaceMaterialMap(surfaceMaterialMap)
{}

}

