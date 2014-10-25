#ifndef _v2_VEHA_Entity_Vector3_H
#define _v2_VEHA_Entity_Vector3_H
#include "Tools/veha_plateform.h"
#include "Tools/utils.h"
#include "VEHA/Kernel/ValueSpecification.h"
#include "VEHA/Kernel/Model.h"
#include "VEHA/Kernel/PrimitiveType.h"
#include <boost/algorithm/string.hpp>
#include <cmath>

namespace VEHA
{

class VEHA_API Vector3 : public ValueSpecification
{
public:
	Vector3(double x=0,double y=0,double z=0);
	Vector3(const string& str);
	double x;
	double y;
	double z;
	shared_ptr<ValueSpecification> clone() const;

	/** Affectation */
	inline Vector3& operator=(const Vector3&);

	/** Comparison */
	// TODO

	/** Add / Sum functions */
	inline Vector3 operator-(const Vector3& v) const;
	inline Vector3 operator+(const Vector3& v) const;
	inline Vector3& operator-=(const Vector3& v);
	inline Vector3& operator+=(const Vector3& v);

	/** Dot product */
	// TODO

	/** Normalize / get length */
	inline void normalize();
	inline Vector3 unit() const;
	inline double squareLength() const;
	inline double length() const;
};

Vector3& Vector3::operator=(const Vector3& value)
{
        this->x=((const Vector3&)value).x;
        this->y=((const Vector3&)value).y;
        this->z=((const Vector3&)value).z;
        return *this;
}

Vector3 Vector3::operator-(const Vector3& v) const
{
    return Vector3(x - v.x, y - v.y, z - v.z);
}

Vector3 Vector3::operator+(const Vector3& v) const
{
    return Vector3(x + v.x, y + v.y, z + v.z);
}

Vector3& Vector3::operator-=(const Vector3& v)
{
    x -= v.x;
    y -= v.y;
    z -= v.z;
    return *this;
}

Vector3& Vector3::operator+=(const Vector3& v)
{
    x += v.x;
    y += v.y;
    z += v.z;
    return *this;
}

double Vector3::squareLength() const
{
      return x*x + y*y + z*z;
}

double Vector3::length() const
{
    return sqrt(squareLength());
}

inline void Vector3::normalize()
{
	double l = length();
	// TODO: Maybe to change, in AReVi we use an "if(l > ARMATH_PRECISION)"
	x /= l;
	y /= l;
	z /= l;
}

inline Vector3 Vector3::unit() const
{
	double l = length();
	// TODO: Maybe to change, in AReVi we use an "if(l > ARMATH_PRECISION)"
	return Vector3(x/l, y/l, z/l);
}

inline std::ostream& operator<<(std::ostream& os, const Vector3& v)
{
    os << "{" << v.x << "," << v.y << "," << v.z << "}";
    return os;
}
}
#endif
