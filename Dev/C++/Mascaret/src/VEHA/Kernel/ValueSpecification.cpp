#include "VEHA/Kernel/ValueSpecification.h"
#include "VEHA/Kernel/Classifier.h"
#include<stdexcept>

namespace VEHA
{

ValueSpecification::ValueSpecification(shared_ptr<Classifier> type):
_type(type),CLASSTYPE("ValueSpecification")
{
}

ValueSpecification::~ValueSpecification()
{
}

string ValueSpecification::getTypeName(void)
{
	shared_ptr<Classifier> t=getType();
	if (t)
		return t->getName();
	else
		return "undef";
}

bool ValueSpecification::getIntValue(int & /*value*/) {return false;}
bool ValueSpecification::getDoubleValue(double & /*value*/) {return false;}
bool ValueSpecification::getStringValue(string & /*value*/) {return false;}
bool ValueSpecification::getBoolValue(bool & /*value*/) {return false;}

//bool ValueSpecification::setIntValue(const int & /*value*/) {return false;}
//bool ValueSpecification::setDoubleValue(const double & /*value*/) {return false;}
//bool ValueSpecification::setStringValue(const string & /*value*/) {return false;}
//bool ValueSpecification::setBoolValue(const bool & /*value*/) {return false;}

//bool ValueSpecification::setValueFromString(const string& /*value*/) {return false;}
string ValueSpecification::getStringFromValue() const
{
	//cerr<<"ValueSpecification::getStringFromValue"<<endl;
	return "";
}

ValueSpecification::operator int () const
{
	throw std::bad_cast();
}
ValueSpecification::operator double () const
{
	throw std::bad_cast();
}
ValueSpecification::operator string () const
{
	throw std::bad_cast();
}
ValueSpecification::operator bool () const
{
	throw std::bad_cast();
}
ValueSpecification::operator shared_ptr<InstanceSpecification> () const
{
	throw std::bad_cast();
}
const ValueSpecification& ValueSpecification::operator= (const ValueSpecification&)
{
	cerr<<"Forbidden copy"<<endl;
	throw std::runtime_error("operation=");
}
//const ValueSpecification& ValueSpecification::operator= (bool )
//{
//	throw std::bad_cast();
//}
//const ValueSpecification& ValueSpecification::operator= (int )
//{
//	throw std::bad_cast();
//}
//const ValueSpecification& ValueSpecification::operator= (double)
//{
//	throw std::bad_cast();
//}
//const ValueSpecification& ValueSpecification::operator= (const string&)
//{
//	throw std::bad_cast();
//}
//const ValueSpecification& ValueSpecification::operator= (shared_ptr<InstanceSpecification> )
//{
//	throw std::bad_cast();
//}

}
