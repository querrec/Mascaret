#ifndef _v2_VEHA_Behavior_StateMachine_TimeEvent_H
#define _v2_VEHA_Behavior_StateMachine_TimeEvent_H

#include "Tools/veha_plateform.h"
#include "VEHA/Behavior/Common/Event.h"

namespace VEHA
{
class ValueSpecification;

class VEHA_API TimeEvent : public Event
{
public :
	TimeEvent(const string& name);
	virtual ~TimeEvent();
	// --- Attributes ---
protected :
	shared_ptr<ValueSpecification> _when;
public :
	inline shared_ptr<ValueSpecification> getWhen(void) {return _when;}
	inline void setWhen(shared_ptr<ValueSpecification>  value) { _when = value;}
protected :
	bool _isRelative;
public :
	inline bool getIsRelative(void) {return _isRelative;}
	inline void setIsRelative(bool value) { _isRelative = value;}

	// --- Relations ---
	// --- Operations ---
};

}
#endif
