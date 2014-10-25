#ifndef _v2_VEHA_Behavior_StateMachine_StateMachineBehaviorExecution_H
#define _v2_VEHA_Behavior_StateMachine_StateMachineBehaviorExecution_H

#include "Tools/veha_plateform.h"
#include "VEHA/Behavior/Common/BehaviorExecution.h"
#include "VEHA/Behavior/Common/Parameters.h"
#include "VEHA/Behavior/StateMachine/StateMachine.h"
#include "VEHA/Behavior/Common/Signal.h"
#include "VEHA/Kernel/Expression.h"


namespace VEHA
{
class InstanceSpecification;
class Trigger;
class Vertex;
class StateMachine;

class VEHA_API StateMachineBehaviorExecution : public BehaviorExecution, boost::noncopyable
{
public :
	StateMachineBehaviorExecution(shared_ptr<StateMachine> stateMachine, shared_ptr<InstanceSpecification> host, const Parameters& p, bool sync=0);
	virtual ~StateMachineBehaviorExecution();

	inline shared_ptr<StateMachine> getStateMachine(void) { return shared_dynamic_cast<StateMachine>(getSpecification());}
	virtual double execute(double dt);

        bool currentActionDone;

	void addSignal(shared_ptr<InstanceSpecification> signal);
	void simulateAddSignal(shared_ptr<InstanceSpecification> signal);
	void addTime(double time);

	inline shared_ptr<Vertex> getCurrentState(){return _currentState.lock();}

	// Attribute specifique au comportement d'execution
protected :
	weak_ptr<Vertex> _currentState;
	bool _first;
        bool _toStop;
	list<shared_ptr<Trigger> > _eventQueue;

        shared_ptr<BehaviorExecution> _be;

	// methodes specifiques au comportement d'execution
protected :
	bool activateState(shared_ptr<Vertex> state, const Parameters &p);
	void onBehaviorStop(BehaviorExecution*);
	void onExpressionChange(shared_ptr<Expression>);

public :
	void setCurrentState(string name);
	bool simulateState(shared_ptr<Vertex> state);

};

}
#endif
