#ifndef _v2_VEHA_Kernel_LiteralReal_H
#define _v2_VEHA_Kernel_LiteralReal_H

#include "Tools/veha_plateform.h"
#include "VEHA/Kernel/LiteralSpecification.h"


namespace VEHA
{

class VEHA_API LiteralReal : public LiteralSpecification
{
public :
	LiteralReal(const double& =0.0);
	LiteralReal(const string& str);
	//      LiteralReal(const ValueSpecification& ){cerr <<"copy Real"<<endl;}
	virtual ~LiteralReal();
	// --- Attributes ---
	//------ value -----
protected :
	double  _value;
	virtual bool setValueFromString(const string& value);
public :
	inline double getValue(void) { return _value;}
	//		inline void setValue(double value) { _value = value;}
	virtual bool getDoubleValue(double & value);
	//                virtual bool setDoubleValue(const double & value);
	virtual string getStringFromValue() const;
	// --- Relations ---
	// --- Operations ---
	virtual operator string () const;
	virtual operator double () const;
	//virtual const ValueSpecification& operator= (double value);
	//virtual const ValueSpecification& operator= (const ValueSpecification&);
	virtual shared_ptr<ValueSpecification> clone() const;
};

}
#endif
