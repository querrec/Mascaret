#ifndef _v2_VEHA_Kernel_OpaqueExpression_H
#define _v2_VEHA_Kernel_OpaqueExpression_H

#include "Tools/veha_plateform.h"
#include "VEHA/Kernel/ValueSpecification.h"

namespace VEHA
{

class VEHA_API OpaqueExpression : public ValueSpecification
{
public :
	OpaqueExpression();
	virtual ~OpaqueExpression();
	// --- Attributes ---
protected :
	string _body;
public :
	inline const string& getBody(void) const {return _body;}
	inline void setBody(const string& value) {_body = value;}
protected :
	string _language;
public :
	inline const string& getLanguage(void)const {return _language;}
	inline void setLanguage(const string& value) {_language = value;}
	virtual const ValueSpecification& operator= (const ValueSpecification&);
	virtual shared_ptr<ValueSpecification> clone() const;
	// --- Relations ---
	// --- Relations ---
	// --- Operations ---
};

}
#endif
