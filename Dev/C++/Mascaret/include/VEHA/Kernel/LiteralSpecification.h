#ifndef _v2_VEHA_Kernel_LiteralSpecification_H
#define _v2_VEHA_Kernel_LiteralSpecification_H

#include "Tools/veha_plateform.h"
#include "VEHA/Kernel/ValueSpecification.h"
#include "Tools/utils.h"

namespace VEHA
{

class VEHA_API LiteralSpecification : public ValueSpecification
{
public :
	LiteralSpecification(shared_ptr<Classifier> type);
	virtual ~LiteralSpecification();
	// --- Attributes ---
	// --- Relations ---
	// --- Operations ---
	virtual shared_ptr<ValueSpecification> clone() const=0;
	//virtual const ValueSpecification& operator= (const ValueSpecification&)=0;
};

}
#endif
