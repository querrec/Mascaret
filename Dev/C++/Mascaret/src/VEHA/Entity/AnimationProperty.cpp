/*
 * AnimationProperty.cpp
 *
 *  Created on: 24 févr. 2011
 *      Author: soler
 */

#include "VEHA/Entity/AnimationProperty.h"
#include "VEHA/Kernel/PrimitiveType.h"
#include "MascaretApplication.h"
#include "VEHA/Kernel/Model.h"
#include "Tools/utils.h"

namespace VEHA
{

AnimationProperty::AnimationProperty(const string& name, Class* cl)
: Property(name,cl,MascaretApplication::getInstance()->getModel()->getBasicType("animation"))
{
}
AnimationProperty::~AnimationProperty()
{
}

}
