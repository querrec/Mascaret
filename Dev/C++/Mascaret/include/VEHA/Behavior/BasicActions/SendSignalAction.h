#ifndef _v2_VEHA_Behavior_BasicActions_SendSignalAction_H
#define _v2_VEHA_Behavior_BasicActions_SendSignalAction_H

#include "Tools/veha_plateform.h"
#include "VEHA/Behavior/BasicActions/Action.h"

namespace VEHA
{
class InstanceSpecification;
class Signal;

typedef struct _SendSignalTarget
{
	shared_ptr<InstanceSpecification> target;
	shared_ptr<Classifier> targetClass;
	string targetName;
} SendSignalTarget;

class VEHA_API SendSignalAction : public Action
{
public :
	SendSignalAction();
	virtual ~SendSignalAction();
	protected :
	shared_ptr<InstanceSpecification>    _signal;
	shared_ptr<Signal>    _signalClass;
	//shared_ptr<InstanceSpecification>    _target;
	SendSignalTarget _target;
	public :
	inline shared_ptr<InstanceSpecification> getSignal(void) { return _signal;}
	inline void setSignal(shared_ptr<InstanceSpecification> signal) { _signal = signal;}

	inline shared_ptr<Signal> getSignalClass(void) { return _signalClass;}
	inline void setSignalClass(shared_ptr<Signal> signalClass) { _signalClass = signalClass;}

	inline shared_ptr<InstanceSpecification> getTarget(void) { return _target.target;}
	inline void setTarget(shared_ptr<InstanceSpecification> target) { _target.target = target;}

	inline shared_ptr<Classifier> getTargetClass(void) { return _target.targetClass;}
	inline void setTargetClass(shared_ptr<Classifier> targetclass) { _target.targetClass = targetclass;}

	inline string getTargetName(void) { return _target.targetName;}
	inline void setTargetName(string name) { _target.targetName = name;}

	bool equalsTo(shared_ptr<Behavior>);

	virtual shared_ptr<BehaviorExecution> createBehaviorExecution(shared_ptr<InstanceSpecification>, const Parameters&, bool sync = false);

};

}
#endif
