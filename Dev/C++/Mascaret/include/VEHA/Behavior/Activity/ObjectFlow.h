#ifndef _v2_VEHA_Behavior_Activity_ObjectFlow_H
#define _v2_VEHA_Behavior_Activity_ObjectFlow_H

#include "Tools/veha_plateform.h"
#include "VEHA/Behavior/Activity/ActivityEdge.h"


namespace VEHA
{

class VEHA_API ObjectFlow : public ActivityEdge
{
public :
	ObjectFlow(const string& name);
	virtual ~ObjectFlow();
	// --- Attributes ---
	// --- Relations ---
	// --- Operations ---
};

}
#endif
