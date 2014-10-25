#include "VEHA/Behavior/StateMachine/CallEvent.h"

namespace VEHA
{

CallEvent::CallEvent(const string &name)
:Event(name)
{
	setType("CallEvent");
}

CallEvent::~CallEvent()
{
}

}
