#ifndef _v2_VEHA_Behavior_Common_Trigger_H
#define _v2_VEHA_Behavior_Common_Trigger_H

#include "Tools/veha_plateform.h"
#include "VEHA/Kernel/Element.h"


namespace VEHA
{
class Event;

class VEHA_API Trigger : public Element
{
public :
	Trigger(const string& name);
	virtual ~Trigger();
	// --- Attributes ---
	// --- Relations ---
	//------ event -----
protected :
	shared_ptr<Event>   _event;
public :
	inline shared_ptr<Event> getEvent(void) { return _event;}
	inline void setEvent(shared_ptr<Event> value) { _event = value;}
	bool equals(shared_ptr<Trigger> trigger);

	// --- Operations ---
};

}
#endif
