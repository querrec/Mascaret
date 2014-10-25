#ifndef _v2_VEHA_Behavior_StateMachine_CallEvent_H
#define _v2_VEHA_Behavior_StateMachine_CallEvent_H

#include "Tools/veha_plateform.h"
#include "VEHA/Behavior/Common/Event.h"

namespace VEHA
{
class Operation;

class VEHA_API CallEvent : public Event
{
public :
	CallEvent(const string& name);
	virtual ~CallEvent();
	// --- Attributes ---
	// --- operation
protected :
	shared_ptr<Operation> _operation;
public :
	inline shared_ptr<Operation> getOperation(void) {return _operation;}
	inline void setOperation( shared_ptr<Operation> value) {_operation = value;}
	// --- Relations ---
	// --- Operations ---
};

}
#endif
