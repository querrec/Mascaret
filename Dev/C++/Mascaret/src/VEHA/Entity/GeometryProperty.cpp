#include "VEHA/Entity/GeometryProperty.h"

namespace VEHA
{

GeometryProperty::GeometryProperty(const string& name,Class* cl,shared_ptr<Classifier> type)
:TopologicalProperty(name,cl,type)
{
}

GeometryProperty::~GeometryProperty()
{
}

}
