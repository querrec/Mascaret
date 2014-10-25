#include "VEHA/Behavior/StateMachine/SignalEvent.h"

namespace VEHA
{

SignalEvent::SignalEvent(const string& name)
:Event(name)
{
	setType("SignalEvent");
}

SignalEvent::~SignalEvent()
{
}

}
