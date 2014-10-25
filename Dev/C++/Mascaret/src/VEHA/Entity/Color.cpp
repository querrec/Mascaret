#include "VEHA/Entity/Color.h"
#include "Tools/utils.h"
#include "MascaretApplication.h"
#include <boost/algorithm/string.hpp>

namespace VEHA{

Color::Color(double r,double g,double b)
: ValueSpecification(MascaretApplication::getInstance()->getModel()->getBasicType("color")), r(r), g(g), b(b)
{
}
Color::Color(float alpha)
: ValueSpecification(MascaretApplication::getInstance()->getModel()->getBasicType("color"))
{
	a = alpha;
}

Color::Color(const string& str)
: ValueSpecification(MascaretApplication::getInstance()->getModel()->getBasicType("color"))
{
        std::vector<std::string> strs;
        boost::split(strs,str,boost::is_any_of("\t "));
        if(strs.size()>2)
        {
                r=boost::lexical_cast<double>(strs[0]);
                g=boost::lexical_cast<double>(strs[1]);
                b=boost::lexical_cast<double>(strs[2]);
        }
}
shared_ptr<ValueSpecification> Color::clone() const
{
	// FIXME: We don't clone alpha ? Is it normal ?
	return make_shared<Color>(r, g, b);
}

}
