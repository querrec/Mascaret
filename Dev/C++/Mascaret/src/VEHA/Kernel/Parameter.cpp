#include "VEHA/Kernel/Parameter.h"
#include "VEHA/Kernel/Classifier.h"

namespace VEHA
{

Parameter::Parameter(const string& name,const string& direction,shared_ptr<Classifier> type)
:Element(name),_direction(direction),
 _type(type)
{
}

Parameter::~Parameter()
{
}

}
