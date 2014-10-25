#include "VEHA/Entity/ShapeProperty.h"
#include "MascaretApplication.h"
#include "VEHA/Kernel/Model.h"
#include "VEHA/Kernel/PrimitiveType.h"
namespace VEHA
{

ShapeProperty::ShapeProperty( const string& name,Class* cl)
:Property(name,cl,MascaretApplication::getInstance()->getModel()->getBasicType("shape"))
{
}

ShapeProperty::~ShapeProperty()
{
}

}
