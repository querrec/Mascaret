#include "MascaretApplication.h"
#include "VEHA/Entity/PointSpecification.h"
#include "VEHA/Kernel/PrimitiveType.h"
#include "VEHA/Kernel/Model.h"
#include "Tools/utils.h"
#include "VEHA/Entity/VirtualRealityComponentFactory.h"

namespace VEHA
{

PointSpecification::PointSpecification()
:TopologicalSpecification(MascaretApplication::getInstance()->getModel()->getBasicType("point"))
{
}

PointSpecification::~PointSpecification()
{
}
shared_ptr<ValueSpecification> PointSpecification::clone() const
{
	//throw std::runtime_error("can't clone a PointSpecification");
	shared_ptr<PointSpecification> p=MascaretApplication::getInstance()->getVRComponentFactory()->createPoint(getGlobalPosition(),getGlobalRotation());

	return p;
}

}
