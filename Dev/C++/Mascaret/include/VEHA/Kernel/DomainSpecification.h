#ifndef _v2_VEHA_Kernel_DomainSpecification_H
#define _v2_VEHA_Kernel_DomainSpecification_H

#include "Tools/veha_plateform.h"

namespace VEHA
{
class ValueSpecification;

class VEHA_API DomainSpecification
{
public :
	DomainSpecification();
	virtual ~DomainSpecification();
	// --- Attributes ---
	// --- Relations ---
	//------ lowerBound -----
protected :
	shared_ptr<ValueSpecification>  _lowerBound;
public :
	inline shared_ptr<ValueSpecification> getLowerBound(void) { return _lowerBound;}
	inline void setLowerBound(shared_ptr<ValueSpecification> lowerBound) { _lowerBound = lowerBound;}
	//------ upperBound -----
protected :
	shared_ptr<ValueSpecification>  _upperBound;
public :
	inline shared_ptr<ValueSpecification> getUpperBound(void) { return _upperBound;}
	inline void setUpperBound(shared_ptr<ValueSpecification> upperBound) { _upperBound = upperBound;}
	//------ discretisation -----
protected :
	shared_ptr<ValueSpecification>  _discretisation;
public :
	inline shared_ptr<ValueSpecification> getDiscretisation(void) { return _discretisation;}
	inline void setDiscretisation(shared_ptr<ValueSpecification> discretisation) { _discretisation = discretisation;}
	// --- Operations ---
};

}
#endif
