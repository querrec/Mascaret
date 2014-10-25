#include "VEHA/Kernel/LiteralString.h"
#include "VEHA/Kernel/PrimitiveType.h"
#include "MascaretApplication.h"
#include "VEHA/Kernel/Model.h"

namespace VEHA
{

LiteralString::LiteralString(const string& value)
:LiteralSpecification(MascaretApplication::getInstance()->getModel()->getBasicType("string")), _value(value)
{
}

LiteralString::~LiteralString()
{
}

//bool LiteralString::setValueFromString(const string& value)
//{
//   _value = value;
//   return true;
//}

string LiteralString::getStringFromValue() const
{
	return _value;
}

LiteralString::operator string () const
        		{
	return _value;
        		}
//const ValueSpecification& LiteralString::operator= (const string& value)
//{
//	_value=value;
//	return *this;
//}
shared_ptr<ValueSpecification> LiteralString::clone() const
{
	LiteralString * vs=new LiteralString();
	vs->CLASSTYPE=CLASSTYPE;
	vs->_type=_type;
	vs->_value=_value;
	return shared_ptr<ValueSpecification>(vs);
}
//const ValueSpecification& LiteralString::operator= (const ValueSpecification&vs)
//{
//	_value=(string)vs;
//	return *this;
//}
}
