#ifndef _v2_VEHA_Behavior_StateMachine_StateMachine_H
#define _v2_VEHA_Behavior_StateMachine_StateMachine_H

#include "Tools/veha_plateform.h"
#include "VEHA/Behavior/Common/Behavior.h"

namespace VEHA
{
class Region;
class State;
class PseudoState;

class VEHA_API StateMachine : public Behavior
{
public :
	StateMachine(const string& name);
	virtual ~StateMachine();

	// --- Attributes ---

	// --- Relations ---
	//------ _region -----
protected :
	vector< shared_ptr<Region> >    _region;
public :
	inline vector< shared_ptr<Region> > getRegion(void) { return _region;}
	inline shared_ptr<Region> getRegion_at(int index) { return _region[index];}
	inline void addRegion(shared_ptr<Region> value) { _region.push_back(value);}
	//------ _connectionPoint -----
protected :
	vector< shared_ptr<PseudoState> >   _connectionPoint;
public :
	inline vector< shared_ptr<PseudoState> > getConnectionPoint(void) { return _connectionPoint;}
	inline shared_ptr<PseudoState> getConnectionPoint_at(int index) { return _connectionPoint[index];}
	inline void addConnectionPoint(shared_ptr<PseudoState> value) { _connectionPoint.push_back(value);}
	// --- Operations ---
	virtual shared_ptr<BehaviorExecution> createBehaviorExecution(shared_ptr<InstanceSpecification> host,const Parameters& p,bool sync = false);

protected :
	vector< shared_ptr<State> > _submachinestates;
public :
	inline void addSubMachineState(shared_ptr<State> state) {_submachinestates.push_back(state);}
        inline vector< shared_ptr<State> > getSubMachineStates(void) {return _submachinestates;}
};

}
#endif
