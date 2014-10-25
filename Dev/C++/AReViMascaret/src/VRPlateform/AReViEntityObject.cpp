/*
 * AReViEntityObject.cpp
 *
 *  Created on: 4 mars 2011
 *      Author: soler
 */

#include "AReVi/VRML/vrmlShape3D.h"
#include "AReVi/Lib3D/transform3D.h"
#include "AReVi/Lib3D/urlSound3D.h"
#include "AReVi/Shapes/box3D.h"
#include "AReVi/arObject.h"
#include "AReVi/Lib3D/material3D.h"
#include "VRPlateform/AReViShapeSpecification.h"
#include "VRPlateform/AReViEntityObject.h"

using namespace AReVi;

namespace AReViMascaret
{

AR_CLASS_DEF(AReViEntityObject,Object3D)

AReViEntityObject::AReViEntityObject(ArCW& arCW) : Object3D(arCW)
{
}

AReViEntityObject::~AReViEntityObject()
{
}


bool AReViEntityObject::restaure(ArRef<AReVi::Activity> act, double)
{

/*	shared_ptr<AReViShapeSpecification> currentShapeSpec = shared_dynamic_cast<AReViShapeSpecification>(_entity->getActiveShape());
	currentShapeSpec->restaure();
	act.destroy();
	return false;*/
	return true;
}

}
