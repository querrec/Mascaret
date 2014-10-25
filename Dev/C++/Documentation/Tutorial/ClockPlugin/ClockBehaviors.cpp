#include "VEHA/Behavior/Common/BehaviorExecution.h"
#include "VEHA/Behavior/Common/Behavior.h"
#include "VEHA/Entity/Entity.h"
#include "VEHA/Entity/RotationVector.h"
#include "VEHA/Kernel/LiteralInteger.h"
#include "VEHA/Kernel/Environment.h"

#include "MascaretApplication.h"

#include <sys/time.h>
using namespace VEHA;
class Clock_updateHands : public BehaviorExecution
{
public:
	Clock_updateHands(shared_ptr<Behavior> specif, shared_ptr<InstanceSpecification> host, const Parameters& p)
	: BehaviorExecution(specif, host, p)
	{
		// Get currentHour and currentMinute slots
		_currentHour=getHost()->getProperty("currentHour");
		_currentMinute=getHost()->getProperty("currentMinute");
		// Get hands from slots
		_hourHand=shared_dynamic_cast<Entity>((shared_ptr<InstanceSpecification>)*getHost()->getProperty("hourHand")->getValue());
		_minHand=shared_dynamic_cast<Entity>((shared_ptr<InstanceSpecification>)*getHost()->getProperty("minuteHand")->getValue());
	}
	virtual ~Clock_updateHands()
	{
	}

	double execute(double dt)
	{
		// Get currentHour and currentMinute values from slots
		int hour=*_currentHour->getValue() % 12;
		int min=*_currentMinute->getValue();
		// Compute hands angles
		double hourAngle=-hour/12.0*2*M_PI;
		double minAngle=-min/60.0*2*M_PI;
		// Set hands angles
		_hourHand->setLocalRotation(RotationVector(1,0,0,hourAngle));
		_minHand->setLocalRotation(RotationVector(1,0,0,minAngle));
		return 0;
	}
	shared_ptr<Slot> _currentHour;
	shared_ptr<Slot> _currentMinute;
	shared_ptr<Entity> _hourHand;
	shared_ptr<Entity> _minHand;
};

class Clock_addOneHour : public BehaviorExecution
{
public:
	Clock_addOneHour(shared_ptr<Behavior> specif, shared_ptr<InstanceSpecification> host, const Parameters& p)
	: BehaviorExecution(specif, host, p)
	{
		// Get currentHour slot
		_currentHour=getHost()->getProperty("currentHour");
	}
	virtual ~Clock_addOneHour()
	{
	}

	double execute(double dt)
	{
		// Get currentHour value from slot
		int hour=*_currentHour->getValue();
		hour+=1; // Add one
		hour%=24;
		// Set new hour value
		_currentHour->addValue(LiteralInteger(hour));
		return 0;
	}
	shared_ptr<Slot> _currentHour;
};


class Clock_addOneMinute : public BehaviorExecution
{
public:
	Clock_addOneMinute(shared_ptr<Behavior> specif, shared_ptr<InstanceSpecification> host, const Parameters& p)
	: BehaviorExecution(specif, host, p)
	{
		// Get currentMinute slot
		_currentMinute=getHost()->getProperty("currentMinute");
	}
	virtual ~Clock_addOneMinute()
	{
	}

	double execute(double dt)
	{
		// Get currentMinute value from slot
		int min=*_currentMinute->getValue();
		min+=1;
		min%=60;
		// Set new minute value
		_currentMinute->addValue(LiteralInteger(min));
		return 0;
	}
	shared_ptr<Slot> _currentMinute;
};


extern "C"
VEHA_PLUGIN BehaviorExecution* Clock_updateHands_init(shared_ptr<Behavior> specif, shared_ptr<InstanceSpecification> host, const Parameters& p)
{
	return new Clock_updateHands(specif, host,p);
}
extern "C"
VEHA_PLUGIN BehaviorExecution* Clock_addOneHour_init(shared_ptr<Behavior> specif, shared_ptr<InstanceSpecification> host, const Parameters& p)
{
	return new Clock_addOneHour(specif, host,p);
}
extern "C"
VEHA_PLUGIN BehaviorExecution* Clock_addOneMinute_init(shared_ptr<Behavior> specif, shared_ptr<InstanceSpecification> host, const Parameters& p)
{
	return new Clock_addOneMinute(specif, host,p);
}

extern "C"
VEHA_PLUGIN void MASCARET_MODEL_LOADED()
{
	// Get system time
	time_t now;
	struct tm* tm;
	now = time(0);
	tm = localtime(&now);

	// Get the clock
	shared_ptr<InstanceSpecification> clock=MascaretApplication::getInstance()->getEnvironment()->getInstance("Clock");
	// Set currentHour and currentMinute properties with system time
	clock->getProperty("currentHour")->addValue(LiteralInteger(tm->tm_hour));
	clock->getProperty("currentMinute")->addValue(LiteralInteger(tm->tm_min));

	// We could launch the updateHands operation but it's not necessary (will be launch by state machine)
	//InstanceSpecification::OperationCallParameters params;
	//clock->executeOperation("updateHands",params);
}
