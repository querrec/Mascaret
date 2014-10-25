#ifndef _v2_VEHA_Behavior_Activity_ControlFlow_H
#define _v2_VEHA_Behavior_Activity_ControlFlow_H

#include "Tools/veha_plateform.h"
#include "VEHA/Behavior/Activity/ActivityEdge.h"


namespace VEHA
{

class VEHA_API ControlFlow : public ActivityEdge
{
public :
	ControlFlow(const string& name);
	virtual ~ControlFlow();
	// --- Attributes ---
	// --- Relations ---
	// --- Operations ---
};

}
#endif
