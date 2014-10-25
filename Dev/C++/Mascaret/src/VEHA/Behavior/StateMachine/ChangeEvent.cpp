#include "VEHA/Behavior/StateMachine/ChangeEvent.h"

namespace VEHA
{

ChangeEvent::ChangeEvent(const string& name)
:Event(name)
{
	setType("ChangeEvent");
}

ChangeEvent::~ChangeEvent()
{
}

}
