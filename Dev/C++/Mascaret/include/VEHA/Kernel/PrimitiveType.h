#ifndef _v2_VEHA_Kernel_PrimitiveType_H
#define _v2_VEHA_Kernel_PrimitiveType_H

#include "Tools/veha_plateform.h"
#include "VEHA/Kernel/Classifier.h"

namespace VEHA
{

class VEHA_API PrimitiveType : public Classifier
{
public :
	PrimitiveType(const string& name);
	virtual shared_ptr<ValueSpecification> createValueFromString(const string& str);
	virtual ~PrimitiveType();
	// --- Attributes ---
	//------ type -----
protected :
	string  _type;
public :
	inline string getType(void) { return _type;}
	//		inline void setType(string type) { _type = type;}
	// --- Relations ---
	// --- Operations ---
};

}
#endif
