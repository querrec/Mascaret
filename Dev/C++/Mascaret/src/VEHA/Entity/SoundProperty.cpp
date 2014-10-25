/*
 * SoundProperty.cpp
 *
 *  Created on: 24 févr. 2011
 *      Author: soler
 */

#include "VEHA/Entity/SoundProperty.h"
#include "VEHA/Kernel/PrimitiveType.h"
#include "MascaretApplication.h"
#include "VEHA/Kernel/Model.h"
#include "Tools/utils.h"


namespace VEHA
{

SoundProperty::SoundProperty(const string& name, Class* cl)
: Property(name,cl,MascaretApplication::getInstance()->getModel()->getBasicType("sound"))
{
}
SoundProperty::~SoundProperty()
{
}

}
