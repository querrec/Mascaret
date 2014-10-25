#include "VEHA/Entity/PathSpecification.h"
#include "VEHA/Entity/PointSpecification.h"
#include "VEHA/Kernel/PrimitiveType.h"
#include "VEHA/Kernel/Model.h"
#include "Tools/utils.h"

namespace VEHA
{

PathSpecification::PathSpecification()
:TopologicalSpecification(MascaretApplication::getInstance()->getModel()->getBasicType("path"))
{
}
PathSpecification::PathSpecification(const string& str)
:TopologicalSpecification(MascaretApplication::getInstance()->getModel()->getBasicType("path"))
{
//TODO: parse string
}
PathSpecification::~PathSpecification()
{
}

shared_ptr<ValueSpecification> PathSpecification::clone() const
{
	throw std::runtime_error("can't clone a PathSpecification");
}

}
