#ifndef _v2_VEHA_Behavior_Common_Signal_H
#define _v2_VEHA_Behavior_Common_Signal_H

#include "Tools/veha_plateform.h"
#include "VEHA/Kernel/Class.h"

namespace VEHA
{

class VEHA_API Signal : public Class
{
public :
	Signal(const string& name);
	virtual ~Signal();
	// --- Attributes ---
	// --- Relations ---
	// --- Operations ---
};

}
#endif
