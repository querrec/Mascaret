#include "VEHA/Kernel/Property.h"
#include "VEHA/Kernel/Class.h"
#include "VEHA/Kernel/ValueSpecification.h"
#include "VEHA/Kernel/DomainSpecification.h"
#include "VEHA/Kernel/Property.h"
#include "VEHA/Kernel/Classifier.h"

#include "VEHA/Kernel/LiteralReal.h"
#include "VEHA/Kernel/LiteralBoolean.h"
#include "VEHA/Kernel/LiteralString.h"
#include "VEHA/Kernel/LiteralInteger.h"
#include "VEHA/Kernel/InstanceValue.h"

namespace VEHA
{

Property::Property(const string& name,
		Class* cl,
		shared_ptr<Classifier> type,
		shared_ptr<Property> opposite,
		shared_ptr<ValueSpecification> def,
		shared_ptr<DomainSpecification> domain)
:Feature(name),
 _type(type), _classifier(cl), _opposite(opposite), _defaultValue(), _domain(domain),
 _isComposite(false), _isNavigable(false), _isReadOnly(false), _isDerived(false), _isUnique(false), _isOrdered(false),
 _minimumNumber(0), _maximumNumber(1)
{
	if (def) _defaultValue=def->clone();
	else
	{
		string typeName=type->getName();
		if (typeName=="real")
			_defaultValue=shared_ptr<ValueSpecification>(new LiteralReal());
		else if (typeName=="integer")
			_defaultValue=shared_ptr<ValueSpecification>(new LiteralInteger());
		else if (typeName=="string")
			_defaultValue=shared_ptr<ValueSpecification>(new LiteralString());
		else if (typeName=="bool")
			_defaultValue=shared_ptr<ValueSpecification>(new LiteralBoolean());
		else if (typeName=="boolean")
			_defaultValue=shared_ptr<ValueSpecification>(new LiteralBoolean());
		//else
		//	_defaultValue=shared_ptr<ValueSpecification>(new InstanceValue(_type));
	}

}

Property::~Property()
{
}

}
