#include "VEHA/Entity/RotationVector.h"
#include "MascaretApplication.h"

namespace VEHA
{
RotationVector::RotationVector(double x,double y,double z,double a)
: ValueSpecification(MascaretApplication::getInstance()->getModel()->getBasicType("rotation")), x(x),y(y),z(z),angle(a)
{
}

RotationVector::RotationVector(const Vector3& v,double a)
: ValueSpecification(MascaretApplication::getInstance()->getModel()->getBasicType("rotation")),x(v.x),y(v.y),z(v.z),angle(a)
{
}
RotationVector::RotationVector(const string& str)
: ValueSpecification(MascaretApplication::getInstance()->getModel()->getBasicType("rotation"))
{
        std::vector<std::string> strs;
        boost::split(strs,str,boost::is_any_of("\t "));
        if(strs.size()>3)
        {
                x=boost::lexical_cast<double>(strs[0]);
                y=boost::lexical_cast<double>(strs[1]);
                z=boost::lexical_cast<double>(strs[2]);
                angle=boost::lexical_cast<double>(strs[3]);
        }
}
shared_ptr<ValueSpecification> RotationVector::clone() const
{
        shared_ptr<ValueSpecification> value(new RotationVector(x,y,z,angle));
        return value;
}
RotationVector& RotationVector::operator=(const RotationVector& value)
{
        this->x=((const RotationVector&)value).x;
        this->y=((const RotationVector&)value).y;
        this->z=((const RotationVector&)value).z;
        this->angle=((const RotationVector&)value).angle;
        return *this;
}
}
