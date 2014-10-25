#ifndef _v2_VEHA_Kernel_Constraint_H
#define _v2_VEHA_Kernel_Constraint_H
#include "Tools/veha_plateform.h"
#include "VEHA/Kernel/Expression.h"
namespace VEHA
{
class ValueSpecification;

class VEHA_API Constraint
{
public :
	Constraint();
	virtual ~Constraint();
	// --- Attributes ---
protected :
	shared_ptr<ValueSpecification>  _specification;
public :
	inline shared_ptr<ValueSpecification> getSpecification(void) {return _specification;}
	inline void setSpecification(shared_ptr<ValueSpecification> value) {_specification = value;}
	// --- Relations ---
	// --- Operations ---
	bool eval(OclParser::Context context);
};

}
#endif
