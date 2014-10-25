/*
 * AReViCursor.cpp
 *
 *  Created on: 4 mars 2011
 *      Author: soler
 */
#include "AReVi/Lib3D/scene3D.h"

#include "Interaction/AReViCursor.h"
#include "VRPlateform/AReViEntityObject.h"

using namespace AReVi;
namespace AReViMascaret
{
AReViCursor::AReViCursor()
: Cursor("system")
{

}
AReViCursor::~AReViCursor()
{

}
void AReViCursor::getPosition(int& x, int& y) const
{
	_renderer->getLastMousePosition(x,y);
}
}
