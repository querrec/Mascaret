#include "VEHA/Entity/EntityClass.h"
#include "VEHA/Entity/ShapeProperty.h"
#include "VEHA/Entity/TopologicalProperty.h"
#include "VEHA/Entity/TopologicalProperty.h"
#include "VEHA/Entity/GeometryProperty.h"
#include "VEHA/Entity/PointSpecification.h"

namespace VEHA
{

EntityClass::EntityClass(const string& name)
: Class(name),_referentialPoint()
{
	shared_ptr<Property> property(
			new ShapeProperty(
					"shape",
					this));
	addAttribute(property);
}

EntityClass::~EntityClass()
{
}

}
