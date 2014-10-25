/*
 * PathProperty.cpp
 *
 *  Created on: 24 févr. 2011
 *      Author: soler
 */

#include "VEHA/Entity/PathProperty.h"
#include "VEHA/Kernel/PrimitiveType.h"
#include "MascaretApplication.h"
#include "VEHA/Kernel/Model.h"
#include "Tools/utils.h"


namespace VEHA
{

PathProperty::PathProperty(const string& name, Class* cl)
: TopologicalProperty(name,cl,MascaretApplication::getInstance()->getModel()->getBasicType("path"))
{
}
PathProperty::~PathProperty()
{
}

}
