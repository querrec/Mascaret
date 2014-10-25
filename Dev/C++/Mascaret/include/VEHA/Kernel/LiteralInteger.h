#ifndef _v2_VEHA_Kernel_LiteralInteger_H
#define _v2_VEHA_Kernel_LiteralInteger_H

#include "Tools/veha_plateform.h"
#include "VEHA/Kernel/LiteralSpecification.h"


namespace VEHA
{

class VEHA_API LiteralInteger : public LiteralSpecification
{
public :
	LiteralInteger(const int& value=0);
	LiteralInteger(const string& str);
	virtual ~LiteralInteger();
	// --- Attributes ---
	//------ value -----
protected :
	int _value;
	virtual bool setValueFromString(const string& value);
public :
	inline int getValue(void) { return _value;}
	//		inline void setValue(int value) { _value = value;}
	inline bool getIntValue(int & value) {value = _value; return true;}
	//                inline bool setIntValue(const int & value) {_value = value; return true;}
	virtual string getStringFromValue() const;

	// --- Relations ---
	// --- Operations ---
	virtual operator string () const;
	virtual operator int () const;
	//virtual const ValueSpecification& operator= (const ValueSpecification&);
	//virtual const ValueSpecification& operator= (int value);
	virtual shared_ptr<ValueSpecification> clone() const;
};

}
#endif
