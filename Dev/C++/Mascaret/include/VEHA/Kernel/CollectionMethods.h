#ifndef COLLECTION_METHODS_H
#define COLLECTION_METHODS_H


#include "VEHA/Kernel/AnyValueSpecification.h"

namespace VEHA
{
class AnyValue;
}
namespace _collectionMethods
{
using namespace VEHA::OclParser;
using namespace VEHA;
	AnyValue callCollectionMethod(const string& name, const CollectionAnyValue& collection, const CollectionAnyValue& parameters);
	AnyValue callCollectionIterMethod(const string& name, const CollectionAnyValue& c, const vector<string>& iterators, iter_t expression,const Context& ctx);
}
namespace VEHA {
using _collectionMethods::callCollectionMethod;
using _collectionMethods::callCollectionIterMethod;
}
#endif
