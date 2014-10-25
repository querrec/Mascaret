#ifndef _v2_VEHA_Behavior_StateMachine_Transition_H
#define _v2_VEHA_Behavior_StateMachine_Transition_H

#include "Tools/veha_plateform.h"
#include "VEHA/Kernel/Element.h"
#include "VEHA/Behavior/BasicActions/Action.h"

namespace VEHA
{
class Constraint;
class Trigger;
class Behavior;
class Vertex;

class VEHA_API Transition : public Element
{
public :
	Transition();
	virtual ~Transition();
	// --- Attributes ---
	// --- Relations ---
	//------ trigger -----
protected :
	vector< shared_ptr<Trigger> >   _trigger;
public :
	inline vector< shared_ptr<Trigger> > getTrigger(void) { return _trigger;}
	inline shared_ptr<Trigger> getTrigger_at(int index) { return _trigger[index];}
	inline void addTrigger(shared_ptr<Trigger> value) { _trigger.push_back(value);}
	//------ effect -----
protected :
	shared_ptr<Action>    _effect;
public :
	inline shared_ptr<Action> getEffect(void) { return _effect;}
	inline void setEffect(shared_ptr<Action> effect) { _effect = effect;}

	//------ source -----
protected :
	weak_ptr<Vertex>  _source;
public :
	inline shared_ptr<Vertex> getSource(void) { return _source.lock();}
	inline void setSource(shared_ptr<Vertex> source) { _source = source;}
	//------ target -----
protected :
	weak_ptr<Vertex>  _target;
public :
	inline shared_ptr<Vertex> getTarget(void) { return _target.lock();}
	inline void setTarget(shared_ptr<Vertex> target) { _target = target;}

	//------ target -----
protected :
	shared_ptr<Constraint>  _guard;
public :
	inline shared_ptr<Constraint> getGuard(void) { return _guard;}
	inline void setGuard(shared_ptr<Constraint> guard) { _guard = guard;}

	// --- Operations ---
};

}
#endif
