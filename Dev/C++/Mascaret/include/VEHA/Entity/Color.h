#ifndef _v2_VEHA_Entity_Color_H
#define _v2_VEHA_Entity_Color_H

#include "Tools/veha_plateform.h"
#include "VEHA/Kernel/ValueSpecification.h"
#include "VEHA/Kernel/PrimitiveType.h"
#include "VEHA/Kernel/Model.h"
// #include "MascaretApplication.h" // Name conflict

namespace VEHA
{

class VEHA_API Color : public ValueSpecification
{
public:
	Color(const string& str);
	Color(double r=0,double g=0,double b=0);
	Color(float a);
	double r,g,b;
	float a;
	shared_ptr<ValueSpecification> clone() const;
	Color& operator=(const Color& value);
};

#if 0 // Need MascaretApplication.h meaning Name conflict
inline Color::Color(double r,double g,double b)
: ValueSpecification(MascaretApplication::getInstance()->getModel()->getBasicType("color")), r(r), g(g), b(b)
{
}
inline Color::Color(float alpha)
: ValueSpecification(MascaretApplication::getInstance()->getModel()->getBasicType("color"))
{
	a = alpha;
}
#endif

inline Color& Color::operator=(const Color& value)
{
	// FIXME: We don't change alpha ? Is it normal ?
	this->r=((const Color&)value).r;
	this->g=((const Color&)value).g;
	this->b=((const Color&)value).b;
	return *this;
}
}
#endif
