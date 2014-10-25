#ifndef _v2_VEHA_Kernel_LiteralBoolean_H
#define _v2_VEHA_Kernel_LiteralBoolean_H

#include "Tools/veha_plateform.h"
#include "VEHA/Kernel/LiteralSpecification.h"


namespace VEHA
{

class VEHA_API LiteralBoolean : public LiteralSpecification
{
public :
	LiteralBoolean(bool b=false);
	LiteralBoolean(const string& str);
	virtual ~LiteralBoolean();
	// --- Attributes ---
	//------ value -----
protected :
	bool    _value;
	virtual bool setValueFromString(const string& value);
public :
	inline bool getValue(void) { return _value;}
	//		inline void setValue(bool value) { _value = value;}
	inline bool getBoolValue(bool & value) {value = _value; return true;}
	//                inline bool setBoolValue(const bool & value) {_value = value; return true;}
	virtual string getStringFromValue() const;
	// --- Relations ---
	// --- Operations ---
	virtual operator string () const;
	virtual operator bool () const;
	//virtual const ValueSpecification& operator= (const ValueSpecification&);
	//virtual const ValueSpecification& operator= (bool value);
	virtual shared_ptr<ValueSpecification> clone() const;
};

}
#endif
