#include "VEHA/Behavior/Common/Trigger.h"
#include "VEHA/Behavior/Common/Event.h"
#include "VEHA/Behavior/Common/Signal.h"
#include "VEHA/Behavior/StateMachine/SignalEvent.h"
#include "VEHA/Behavior/StateMachine/ChangeEvent.h"
#include "VEHA/Behavior/StateMachine/TimeEvent.h"
#include "VEHA/Kernel/Expression.h"

namespace VEHA
{

Trigger::Trigger(const string& name)
:Element(name)
{
}

Trigger::~Trigger()
{
}

bool Trigger::equals(shared_ptr<Trigger> trigger)
{
	if (_event->getType() != trigger->_event->getType()) return false;
	if ( _event->getType() == "SignalEvent")
	{
//cerr << (shared_dynamic_cast<SignalEvent>  (_event))->getSignal()->getName();
//cerr << " == ";
//cerr << (shared_dynamic_cast<SignalEvent>  (trigger->_event))->getSignal()->getName();
//cerr << endl; 
                //cerr << (shared_dynamic_cast<SignalEvent>  (_event))->getSignal() << endl;
		if ((shared_dynamic_cast<SignalEvent>  (_event))->getSignalClass()->getName() == (shared_dynamic_cast<SignalEvent>  (trigger->_event))->getSignal()->getClassifier()->getName()) return true;
		else return false;
	}
	else if (_event->getType() == "ChangeEvent")
	{
		shared_ptr<Expression> exp1 = shared_dynamic_cast<Expression>((shared_dynamic_cast<ChangeEvent>(_event))->getChangeExpression());
		shared_ptr<Expression> exp2 = shared_dynamic_cast<Expression>((shared_dynamic_cast<ChangeEvent>(trigger->_event))->getChangeExpression());
		if ( exp1->getExpression() == exp2->getExpression() )
			return true;
		else return false;
	}
	else if (_event->getType() == "TimeEvent")
	{
		shared_ptr<TimeEvent> te1 = shared_dynamic_cast<TimeEvent>(_event);
		shared_ptr<TimeEvent> te2 = shared_dynamic_cast<TimeEvent>(trigger->_event);
		shared_ptr<LiteralReal> value1 = shared_dynamic_cast<LiteralReal>(te1->getWhen());
		double timeValue1 = value1->getValue();
		shared_ptr<LiteralReal> value2 = shared_dynamic_cast<LiteralReal>(te2->getWhen());
		double timeValue2 = value2->getValue();

		if ( timeValue1 == timeValue2 )
			return true;
		else return false;
	}
	else
		return false;
}

}
