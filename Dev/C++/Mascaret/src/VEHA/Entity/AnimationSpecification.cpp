#include "VEHA/Entity/AnimationSpecification.h"
#include "VEHA/Kernel/PrimitiveType.h"
#include "MascaretApplication.h"
#include "VEHA/Kernel/Model.h"

namespace VEHA
{

AnimationSpecification::AnimationSpecification(const string & name, const string & url)
:VirtualRealitySpecification(MascaretApplication::getInstance()->getModel()->getBasicType("animation")), _name(name), _url(url)
{
}

AnimationSpecification::~AnimationSpecification()
{
}

shared_ptr<ValueSpecification> AnimationSpecification::clone() const
{
	throw std::exception();
}

}
