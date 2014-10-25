#ifndef _v2_VEHA_Behavior_StateMachine_ChangeEvent_H
#define _v2_VEHA_Behavior_StateMachine_ChangeEvent_H

#include "Tools/veha_plateform.h"
#include "VEHA/Behavior/Common/Event.h"

namespace VEHA
{
class ValueSpecification;

class VEHA_API ChangeEvent : public Event
{
public :
	ChangeEvent(const string& name);
	virtual ~ChangeEvent();
	// --- Attributes ---
protected :
	shared_ptr<ValueSpecification> _changeExpression;
public :
	inline shared_ptr<ValueSpecification> getChangeExpression(void) {return _changeExpression;}
	inline void setChangeExpression(shared_ptr<ValueSpecification>  value) { _changeExpression = value;}
	// --- Relations ---
	// --- Operations ---
};

}
#endif
