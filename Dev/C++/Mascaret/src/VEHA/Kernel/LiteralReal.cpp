#include "VEHA/Kernel/LiteralReal.h"
#include "VEHA/Kernel/PrimitiveType.h"
#include "MascaretApplication.h"
#include "VEHA/Kernel/Model.h"

#include <sstream>

namespace VEHA
{

LiteralReal::LiteralReal(const double& value)
:LiteralSpecification(MascaretApplication::getInstance()->getModel()->getBasicType("real")),_value(value)
{
}
LiteralReal::LiteralReal(const string & str)
:LiteralSpecification(MascaretApplication::getInstance()->getModel()->getBasicType("real")),_value(boost::lexical_cast<double>(str))
{
}

LiteralReal::~LiteralReal()
{
}
bool LiteralReal::getDoubleValue(double & value) {value = _value; return true;}
//bool LiteralReal::setDoubleValue(const double & value) {_value = value; return true;}

bool LiteralReal::setValueFromString(const string& value)
{
	try
	{
		_value= boost::lexical_cast<double>(value);
		return true;
	}
	catch(boost::bad_lexical_cast &)
	{
		return false;
	}
	return true;
}

string LiteralReal::getStringFromValue() const
{
	return   boost::lexical_cast<std::string>(_value);
}
LiteralReal::operator double () const
        		{
	return _value;
        		}
LiteralReal::operator string () const
        		{
	return  boost::lexical_cast<std::string>(_value);
        		}

//const ValueSpecification& LiteralReal::operator= (double value)
//{
//	cerr<<"= operator LiteralReal"<<endl;
//	_value=value;
//	cerr<<"_value="<<_value<<endl;
//	return *this;
//}
shared_ptr<ValueSpecification> LiteralReal::clone() const
{
	LiteralReal * vs=new LiteralReal();
	vs->CLASSTYPE=CLASSTYPE;
	vs->_type=_type;
	vs->_value=_value;
	return shared_ptr<ValueSpecification>(vs);
}
//const ValueSpecification& LiteralReal::operator= (const ValueSpecification&vs)
//{
//	_value=(double)vs;
//	return *this;
//}

}
