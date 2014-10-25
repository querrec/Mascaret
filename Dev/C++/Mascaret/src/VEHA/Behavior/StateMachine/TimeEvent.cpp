#include "VEHA/Behavior/StateMachine/TimeEvent.h"

namespace VEHA
{

TimeEvent::TimeEvent(const string& name)
:Event(name)
{
	setType("TimeEvent");
}

TimeEvent::~TimeEvent()
{
}

}
