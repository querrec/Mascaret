#ifndef _v2_VEHA_Entity_AReVi_Color_H
#define _v2_VEHA_Entity_AReVi_Color_H

//#define __WINDOBE__ 1

#include "VEHA/Kernel/PrimitiveType.h"
#include "VEHA/Kernel/Model.h"
#include "VEHA/Entity/Color.h"
#include "areviMascaret_plateform.h"

#include "AReVi/Lib3D/material3D.h"
#include "AReVi/Shapes/shape3D.h"
#include "AReVi/Shapes/surface3D.h"
#include "AReVi/Lib3D/object3D.h"
#include "AReVi/Lib3D/transform3D.h"
#include "AReVi/Lib3D/util3D_priv.h"

using AReVi::ArRef;
using AReVi::Material3D;
using AReVi::Transform3D;
using AReVi::StlString;

using namespace VEHA;
using namespace AReVi;

namespace AReViMascaret
{

class AREVI_MASCARET_API AReViColor : public VEHA::Color
{
public:
	AReViColor();
	AReViColor(float a);
	AReViColor(StlMap<StlString,ArRef<Material3D> > surfaceMaterialMap);
	AReViColor(int r,int g,int b);
	~AReViColor();

	virtual StlMap<StlString,ArRef<Material3D> > getSurfaceMap(){return _surfaceMaterialMap;}
	virtual void setSurfaceMap (StlMap<StlString,ArRef<Material3D> > map){_surfaceMaterialMap=map;}

protected: 
	StlMap<StlString,ArRef<Material3D> > _surfaceMaterialMap;

};

}
#endif
