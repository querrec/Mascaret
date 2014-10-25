#ifndef _v2_VEHA_Kernel_Feature_H
#define _v2_VEHA_Kernel_Feature_H

#include "Tools/veha_plateform.h"
#include "VEHA/Kernel/Element.h"


namespace VEHA
{

class VEHA_API Feature : public Element
{
public :
	Feature(const string& name);
	virtual ~Feature();
	// --- Attributes ---
	// --- Relations ---
	// --- Operations ---
};

}
#endif
