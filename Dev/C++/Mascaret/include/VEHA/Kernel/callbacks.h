#ifndef CALLBACKS_H
#define CALLBACKS_H

#include "Tools/veha_plateform.h"
#include <boost/function.hpp>
namespace VEHA
{
using boost::function;
class Entity;
class ValueSpecification;

typedef function<void (string, shared_ptr<ValueSpecification>) > cbSlotValue;
enum changeValue_t{REMOVE,REPLACE,ADD};
typedef function<void (string,changeValue_t, shared_ptr<ValueSpecification>) > cbSlotChangeValue;
//typedef void * cbEntityCreate;

}
#endif
