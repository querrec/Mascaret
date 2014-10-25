#ifndef _v2_VEHA_Entity_RotationVector_H
#define _v2_VEHA_Entity_RotationVector_H
#include "Tools/veha_plateform.h"
#include "Tools/utils.h"
#include "VEHA/Entity/Vector3.h"
#include "VEHA/Kernel/ValueSpecification.h"
#include "VEHA/Kernel/Model.h"
#include "VEHA/Kernel/PrimitiveType.h"
#include <boost/algorithm/string.hpp>
namespace VEHA
{

class VEHA_API RotationVector : public ValueSpecification
{
public:
	RotationVector(double x=1,double y=0,double z=0,double a=0);
	RotationVector(const Vector3& v,double a);
	RotationVector(const string& str);
	double x;
	double y;
	double z;
	double angle;
	shared_ptr<ValueSpecification> clone() const;
	RotationVector& operator= (const RotationVector& value);
};

}
#endif
