#include "VEHA/Kernel/LiteralBoolean.h"
#include "VEHA/Kernel/PrimitiveType.h"
#include "MascaretApplication.h"
#include "VEHA/Kernel/Model.h"

namespace VEHA
{

LiteralBoolean::LiteralBoolean(bool b)
:LiteralSpecification(MascaretApplication::getInstance()->getModel()->getBasicType("boolean")), _value(b)
{
}
LiteralBoolean::LiteralBoolean(const string & str)
:LiteralSpecification(MascaretApplication::getInstance()->getModel()->getBasicType("boolean")),_value(false)
{
	setValueFromString(str);
}

LiteralBoolean::~LiteralBoolean()
{
}

bool LiteralBoolean::setValueFromString(const string& value)
{
	if (value == "TRUE" || value == "true") _value = true;
	else if (value == "FALSE" || value == "false") _value = false;
	else return false;
	return true;
}

string LiteralBoolean::getStringFromValue() const
{
	return (_value?"true":"false");
}
LiteralBoolean::operator string () const
        		{
	return  boost::lexical_cast<std::string>(_value);
        		}

LiteralBoolean::operator bool () const
        		{
	return _value;
        		}
//const ValueSpecification& LiteralBoolean::operator= (bool value)
//{
//	_value=value;
//	return *this;
//}
shared_ptr<ValueSpecification> LiteralBoolean::clone() const
{
	LiteralBoolean * vs=new LiteralBoolean();
	vs->CLASSTYPE=CLASSTYPE;
	vs->_type=_type;
	vs->_value=_value;
	return shared_ptr<ValueSpecification>(vs);
}
//const ValueSpecification& LiteralBoolean::operator= (const ValueSpecification&vs)
//{
//	_value=(bool)vs;
//	return *this;
//}

}
