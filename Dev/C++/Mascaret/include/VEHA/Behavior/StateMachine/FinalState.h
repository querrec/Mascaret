#ifndef _v2_VEHA_Behavior_StateMachine_FinalState_H
#define _v2_VEHA_Behavior_StateMachine_FinalState_H

#include "Tools/veha_plateform.h"
#include "VEHA/Behavior/StateMachine/State.h"

namespace VEHA
{

class VEHA_API FinalState : public State
{
public :
	FinalState(const string& name);
	virtual ~FinalState();
	// --- Attributes ---
	// --- Relations ---
	// --- Operations ---
};

}
#endif
