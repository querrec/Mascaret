#include "VEHA/Entity/TopologicalProperty.h"

namespace VEHA
{

TopologicalProperty::TopologicalProperty(const string& name,Class* cl,shared_ptr<Classifier> type)
:Property(name,cl,type)
{
}

TopologicalProperty::~TopologicalProperty()
{
}

}
