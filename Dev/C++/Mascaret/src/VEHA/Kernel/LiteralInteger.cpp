#include <cstdio>
#include "VEHA/Kernel/LiteralInteger.h"
#include "VEHA/Kernel/PrimitiveType.h"
#include "MascaretApplication.h"
#include "VEHA/Kernel/Model.h"
#include <boost/lexical_cast.hpp>

namespace VEHA
{

LiteralInteger::LiteralInteger(const int& value)
:LiteralSpecification(MascaretApplication::getInstance()->getModel()->getBasicType("integer")),_value(value)
{
}
LiteralInteger::LiteralInteger(const string & str)
:LiteralSpecification(MascaretApplication::getInstance()->getModel()->getBasicType("integer")),_value( boost::lexical_cast<int>(str))
{
}

LiteralInteger::~LiteralInteger()
{
}

bool LiteralInteger::setValueFromString(const string& value)
{
	try
	{
		_value= boost::lexical_cast<int>(value);
		return true;
	}
	catch(boost::bad_lexical_cast &)
	{
		return false;
	}

	sscanf(value.c_str(),"%d",&_value);
	return true;
}

string LiteralInteger::getStringFromValue() const
{
	return boost::lexical_cast<std::string>(_value);
}

LiteralInteger::operator string () const
		{
	return boost::lexical_cast<std::string>(_value);
		}

LiteralInteger::operator int () const
		{
	return _value;
		}
//const ValueSpecification& LiteralInteger::operator= (int  value)
//{
//	_value=value;
//	return *this;
//}
shared_ptr<ValueSpecification> LiteralInteger::clone() const
{
	LiteralInteger * vs=new LiteralInteger();
	vs->CLASSTYPE=CLASSTYPE;
	vs->_type=_type;
	vs->_value=_value;
	return shared_ptr<ValueSpecification>(vs);
}
//const ValueSpecification& LiteralInteger::operator= (const ValueSpecification&vs)
//{
//	_value=(int)vs;
//	return *this;
//}
}
