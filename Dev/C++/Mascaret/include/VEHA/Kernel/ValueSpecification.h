#ifndef _v2_VEHA_Kernel_ValueSpecification_H
#define _v2_VEHA_Kernel_ValueSpecification_H


#include <boost/any.hpp>
#include <boost/lexical_cast.hpp>
#include "Tools/veha_plateform.h"
//#include "VEHA/Kernel/InstanceSpecification.h"


namespace VEHA
{
class Classifier;
class InstanceSpecification;

class VEHA_API ValueSpecification : public enable_shared_from_this<ValueSpecification> {
public :
	ValueSpecification(shared_ptr<Classifier> type);
	virtual ~ValueSpecification();
	// --- Attributes ---
	// --- Relations ---
	//------ type -----
protected :
	weak_ptr<Classifier> _type;
private:
	virtual const ValueSpecification& operator= (const ValueSpecification&);
public :
	inline shared_ptr<Classifier> getType(void) const { return _type.lock();}
	//inline void setType(Classifier * type) { _type = type;}

	virtual bool getIntValue(int & value);
	virtual bool getDoubleValue(double & value);
	virtual bool getStringValue(string & value);
	virtual bool getBoolValue(bool & value);

	//virtual bool setIntValue(const int & value);
	//virtual bool setDoubleValue(const double & value);
	//virtual bool setStringValue(const string & value);
	//virtual bool setBoolValue(const bool & value);

	virtual string getTypeName(void);
	string CLASSTYPE;

	//virtual bool setValueFromString(const string& value);
	virtual string getStringFromValue() const;

	// --- Operations ---
	virtual operator int () const;
	virtual operator double () const;
	virtual operator string () const;
	virtual operator bool () const;
	virtual operator shared_ptr<InstanceSpecification> () const;
	//virtual const ValueSpecification& operator= (bool);
	//virtual const ValueSpecification& operator= (int);
	//virtual const ValueSpecification& operator= (double);
	//virtual const ValueSpecification& operator= (const string&);
	//virtual const ValueSpecification& operator= (shared_ptr<InstanceSpecification> );
	virtual shared_ptr<ValueSpecification> clone() const=0;
};

template<class T>
class VEHA_API SubClassValueSpecification:public ValueSpecification
{

public:
	SubClassValueSpecification(const T& val=T());
	inline T getValue() const{return _value;}
	inline void setValue(T value){_value=value;}

	bool setValueFromString(const string& value);
	string getStringFromValue() const;

	virtual operator T () const {return _value;}
	virtual shared_ptr<ValueSpecification> clone() const
                		{
		shared_ptr<SubClassValueSpecification> vs=shared_ptr<SubClassValueSpecification>(new SubClassValueSpecification<T>());
		vs->CLASSTYPE=CLASSTYPE;
		vs->_type=_type;
		vs->_value=_value;
		return vs;
                		}
	virtual void operator= (boost::any value)
	{
		_value=boost:: any_cast<T>(value);
	}
protected:
	T _value;
};

}
#endif
