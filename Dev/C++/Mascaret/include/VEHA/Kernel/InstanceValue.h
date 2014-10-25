#ifndef _v2_VEHA_Kernel_InstanceValue_H
#define _v2_VEHA_Kernel_InstanceValue_H

#include "Tools/veha_plateform.h"
#include "VEHA/Kernel/ValueSpecification.h"

namespace VEHA
{
class InstanceSpecification;

class VEHA_API InstanceValue : public ValueSpecification
{
public :
	//=shared_ptr<InstanceSpecification>());
	InstanceValue(shared_ptr<InstanceSpecification> instance);
	InstanceValue(shared_ptr<Classifier> type);
	virtual ~InstanceValue();
	// --- Attributes ---
protected :
	shared_ptr<InstanceSpecification> _value;
public :
	inline shared_ptr<InstanceSpecification> getValue(void) const {return _value;}
	// --- Relations ---
	// --- Operations ---
	virtual operator string () const;
	//      virtual bool setValueFromString(const string& value);
	virtual string getStringFromValue() const;
	virtual operator shared_ptr<InstanceSpecification> () const;
	//virtual const ValueSpecification& operator= (const ValueSpecification&);
	//virtual const ValueSpecification& operator= ( shared_ptr<InstanceSpecification> value);
	virtual shared_ptr<ValueSpecification> clone() const;
};

}
#endif
