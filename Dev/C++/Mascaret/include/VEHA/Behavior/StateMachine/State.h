#ifndef _v2_VEHA_Behavior_StateMachine_State_H
#define _v2_VEHA_Behavior_StateMachine_State_H

#include "Tools/veha_plateform.h"
#include "VEHA/Behavior/StateMachine/Vertex.h"

namespace VEHA
{
class Operation;
class InstanceSpecification;
class BehaviorExecution;
class StateMachine;
class Region;

class VEHA_API State : public Vertex
{
public :
	State(const string& name);
	virtual ~State();
	// --- Attributes ---
protected :
        bool _isSimple;

public : 
        inline bool isSimple(void) {return _isSimple;}
        
protected :
        vector<shared_ptr<Region> >_regions;
 
public :
        inline void addRegion(shared_ptr<Region> region) {_regions.push_back(region); _isSimple = false;}
        inline vector<shared_ptr<Region> > getRegions(void) {return _regions;}
        inline bool isComposite(void) {return _regions.size() >= 1;}
        inline bool isOrthogonal(void) {return _regions.size() >= 2;}

	//doBehavior
protected :
	weak_ptr<Operation> _doBehavior;
public :
	inline shared_ptr<Operation> getDoBehavior(void) {return _doBehavior.lock();}
	inline void setDoBehavior(shared_ptr<Operation> value) {_doBehavior = value;}
	//entryBehavior
protected :
	weak_ptr<Operation> _entryBehavior;
	shared_ptr<BehaviorExecution> _currentExecution;
public :
	inline shared_ptr<Operation> getEntryBehavior(void) {return _entryBehavior.lock();}
	inline void setEntryBehavior(shared_ptr<Operation> value) {_entryBehavior = value;}
	//exitBehavior
protected :
	weak_ptr<Operation> _exitBehavior;
public :
	inline shared_ptr<Operation> getExitBehavior(void) {return _exitBehavior.lock();}
	inline void setExitBehavior(shared_ptr<Operation> value) {_exitBehavior = value;}
	// --- Relations ---
	// --- Operations ---
	shared_ptr<BehaviorExecution> activate(shared_ptr<InstanceSpecification> host, const Parameters & param);
	void simulate(shared_ptr<InstanceSpecification> host);
	void desactivate(void);

protected :
	shared_ptr<StateMachine> _submachine;
        bool _isSubMachineState;

public : 
        inline void setSubMachine(shared_ptr<StateMachine> submachine) {_isSubMachineState = true; _isSimple = false; _submachine = submachine;}
        inline shared_ptr<StateMachine> getSubMachine(void) {return _submachine;}
        inline bool isSubMachineState(void) {return _isSubMachineState;}
};

}
#endif
