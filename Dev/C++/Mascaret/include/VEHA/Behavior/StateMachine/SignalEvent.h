#ifndef _v2_VEHA_Behavior_StateMachine_SignalEvent_H
#define _v2_VEHA_Behavior_StateMachine_SignalEvent_H

#include "Tools/veha_plateform.h"
#include "VEHA/Behavior/Common/Event.h"
#include "VEHA/Kernel/InstanceSpecification.h"

namespace VEHA
{
class Signal;

class VEHA_API SignalEvent : public Event
{
public :
	SignalEvent(const string& name);
	virtual ~SignalEvent();
	// --- Attributes ---
	// Signal
protected :
	shared_ptr<InstanceSpecification> _signal;
	shared_ptr<Signal> _signalClass;
public :
	inline shared_ptr<InstanceSpecification> getSignal(void) { return _signal;}
	inline void setSignal(shared_ptr<InstanceSpecification> value) { _signal = value;}
	inline shared_ptr<Signal> getSignalClass(void) { return _signalClass;}
	inline void setSignalClass(shared_ptr<Signal> value) { _signalClass = value;}
	// --- Relations ---
	// --- Operations ---
};

}
#endif
