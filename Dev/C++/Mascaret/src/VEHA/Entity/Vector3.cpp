#include "VEHA/Entity/Vector3.h"
#include "MascaretApplication.h"

namespace VEHA
{
Vector3::Vector3(double x,double y,double z)
: ValueSpecification(MascaretApplication::getInstance()->getModel()->getBasicType("vector3")),x(x),y(y),z(z)
{
}
Vector3::Vector3(const string& str)
: ValueSpecification(MascaretApplication::getInstance()->getModel()->getBasicType("vector3"))
{
        std::vector<std::string> strs;
        boost::split(strs,str,boost::is_any_of("\t "));
        if(strs.size()>2)
        {
                x=boost::lexical_cast<double>(strs[0]);
                y=boost::lexical_cast<double>(strs[1]);
                z=boost::lexical_cast<double>(strs[2]);
        }
}
shared_ptr<ValueSpecification> Vector3::clone() const
{
        return make_shared<Vector3>(x, y, z);
}
}
