
#ifndef OCL_SPECIAL_METHODS_H
#define OCL_SPECIAL_METHODS_H


#include "VEHA/Kernel/AnyValueSpecification.h"
#include "VEHA/Kernel/Environment.h"
namespace VEHA
{
class AnyValue;
}
namespace _oclSpecialMethods
{ 
using namespace VEHA;
//using namespace VEHA::OclParser;
AnyValue specialOpCall(const string& name,const CollectionAnyValue& params);

}
namespace VEHA {
using _oclSpecialMethods::specialOpCall;
}
#endif
