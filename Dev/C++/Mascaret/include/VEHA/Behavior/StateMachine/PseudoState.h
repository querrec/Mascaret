#ifndef _v2_VEHA_Behavior_StateMachine_PseudoState_H
#define _v2_VEHA_Behavior_StateMachine_PseudoState_H

#include "Tools/veha_plateform.h"
#include "VEHA/Behavior/StateMachine/Vertex.h"


enum PseudoStateKind { INITIAL = 0, DEEPHISTORY = 1, SHALLOWHISTORY = 2, JOIN = 3, FORK = 4, JUNCTION = 5, CHOICE =6, ENTRYPOINT = 7, EXITPOINT = 8, TERMINATE = 9  };

namespace VEHA
{
class InstanceSpecification;
class BehaviorExecution;

class VEHA_API PseudoState : public Vertex
{
public :
	PseudoState(const string& name);
	virtual ~PseudoState();
	// --- Attributes ---
	PseudoStateKind _kind;
	inline void setKind(PseudoStateKind value){_kind = value;}
	inline PseudoStateKind getKind(void) {return _kind;}
	// --- Relations ---
	// --- Operations ---
	shared_ptr<BehaviorExecution> activate(shared_ptr<InstanceSpecification> host, const Parameters & p);
	void desactivate(void);
};

}
#endif
