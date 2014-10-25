#ifndef _v2_VEHA_Kernel_LiteralString_H
#define _v2_VEHA_Kernel_LiteralString_H

#include "Tools/veha_plateform.h"
#include "VEHA/Kernel/LiteralSpecification.h"

namespace VEHA
{

class VEHA_API LiteralString : public LiteralSpecification
{
public :
	LiteralString(const string& value="");
	virtual ~LiteralString();
	// --- Attributes ---
	//------ value -----
protected :
	string  _value;
public :
	inline string getValue(void) { return _value;}
	//inline void setValue(string value) { _value = value;}
	inline bool getStringValue(string & value) {value = _value; return true;}
	//inline bool setStringValue(const string & value) {_value = value; return true;}
	//virtual bool setValueFromString(const string& value);
	virtual string getStringFromValue() const;
	// --- Relations ---
	// --- Operations ---
	virtual operator string () const;
	//virtual const ValueSpecification& operator= (const ValueSpecification&);
	//virtual const ValueSpecification& operator= (const string& value);
	virtual shared_ptr<ValueSpecification> clone() const;
};

}
#endif
