/*
 * PointProperty.cpp
 *
 *  Created on: 24 févr. 2011
 *      Author: soler
 */

#include "VEHA/Entity/PointProperty.h"
#include "VEHA/Kernel/PrimitiveType.h"
#include "MascaretApplication.h"
#include "VEHA/Kernel/Model.h"
#include "Tools/utils.h"

namespace VEHA
{

PointProperty::PointProperty(const string& name, Class* cl)
: TopologicalProperty(name,cl,MascaretApplication::getInstance()->getModel()->getBasicType("point"))
{
}
PointProperty::~PointProperty()
{
}

}
