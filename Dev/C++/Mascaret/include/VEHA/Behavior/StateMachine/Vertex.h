#ifndef _v2_VEHA_Behavior_StateMachine_Vertex_H
#define _v2_VEHA_Behavior_StateMachine_Vertex_H

#include "Tools/veha_plateform.h"
#include "VEHA/Kernel/Element.h"
#include "VEHA/Behavior/Common/Parameters.h"

namespace VEHA
{

class InstanceSpecification;
class BehaviorExecution;
class Transition;

class VEHA_API Vertex : public Element
{
public :
	Vertex(const string& name);
	virtual ~Vertex();
	// --- Attributes ---
	// --- Relations ---
	//------ outgoing -----
protected :
	vector< shared_ptr<Transition> >    _outgoing;
public :
	inline vector< shared_ptr<Transition> > getOutgoing(void) { return _outgoing;}
	inline shared_ptr<Transition> getOutgoing_at(int index) { return _outgoing[index];}
	inline void addOutgoing(shared_ptr<Transition> value) { _outgoing.push_back(value);}
	//------ incoming -----
protected :
	vector< shared_ptr<Transition> >    _incoming;
public :
	inline vector< shared_ptr<Transition> > getIncoming(void) { return _incoming;}
	inline shared_ptr<Transition> getIncoming_at(int index) { return _incoming[index];}
	inline void addIncoming(shared_ptr<Transition> value) { _incoming.push_back(value);}
	// --- Operations ---
	virtual  shared_ptr<BehaviorExecution> activate(shared_ptr<InstanceSpecification> host, const Parameters & p) = 0;
	virtual  void simulate(shared_ptr<InstanceSpecification> host){}
	virtual  void desactivate(void) = 0;
	bool running;

	inline void setNonInterupt(bool value) {_noninterupt = value;}
	inline bool getNonInterupt(void) {return _noninterupt ;}
protected :
	bool _noninterupt;
};

}
#endif
