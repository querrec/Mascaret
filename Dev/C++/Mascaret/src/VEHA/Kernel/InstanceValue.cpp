#include "VEHA/Kernel/InstanceValue.h"
#include "VEHA/Kernel/Class.h"
#include "VEHA/Kernel/InstanceSpecification.h"

namespace VEHA
{

InstanceValue::InstanceValue(shared_ptr<InstanceSpecification>  val):
        		ValueSpecification(val->getClassifier()),
        		_value(val)
{
	CLASSTYPE="Instance";
}
InstanceValue::InstanceValue(shared_ptr<Classifier>  type):
        		ValueSpecification(type),
        		_value()
{
	CLASSTYPE="Instance";
}

InstanceValue::~InstanceValue()
{
}
//bool InstanceValue::setValueFromString(const string& )
//{
//	return false	;
//}
string InstanceValue::getStringFromValue() const
{
	if (_value) return _value->getName();
	else return "None";
}
InstanceValue::operator string () const
{
	return  getStringFromValue();
}
InstanceValue::operator shared_ptr<InstanceSpecification>() const
{
	return _value;
}
//const ValueSpecification& InstanceValue::operator= (shared_ptr<InstanceSpecification> value)
//{
//	_value=value;
//	return *this;
//}
shared_ptr<ValueSpecification> InstanceValue::clone() const
{
	InstanceValue * vs=new InstanceValue(getType());
	//	vs->CLASSTYPE=CLASSTYPE;
	//	vs->_type=_type;
	vs->_value=_value;
	return shared_ptr<ValueSpecification>(vs);
}
//const ValueSpecification& InstanceValue::operator= (const ValueSpecification&vs)
//{
//	_value=(shared_ptr<InstanceSpecification>)vs;
//	 return *this;
//}

}
