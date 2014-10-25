#ifndef BEHAVE_ACTIONLISTENERBEHAVIOR_H
#define BEHAVE_ACTIONLISTENERBEHAVIOR_H

#define WIN32_LEAN_AND_MEAN

#include "Tools/veha_plateform.h"
#include "VEHA/Kernel/InstanceSpecification.h"
#include "VEHA/Kernel/Classifier.h"
#include "VEHA/Kernel/Operation.h"
#include "VEHA/Kernel/Parameter.h"
#include "VEHA/Kernel/Slot.h"
#include "VEHA/Kernel/Property.h"
#include "VEHA/Kernel/LiteralReal.h"
#include "VEHA/Kernel/LiteralInteger.h"
#include "VEHA/Behavior/Common/BehaviorExecution.h"
#include "VEHA/Behavior/Common/Behavior.h"
#include "VEHA/Behavior/Activity/ActivityExecution.h"
#include "VEHA/Behavior/Activity/ActivityPartition.h"
#include "VEHA/Behavior/Activity/ActivityNode.h"
#include "VEHA/Behavior/Activity/ActionNode.h"
#include "VEHA/Entity/Vector3.h"
#include "VEHA/Entity/Entity.h"
#include "BEHAVE/OneShotBehaviorExecution.h"
#include "BEHAVE/CyclicBehaviorExecution.h"
#include "BEHAVE/Agent.h"
#include "BEHAVE/AID.h"
#include "BEHAVE/ACLMessage.h"
#include "BEHAVE/OrganisationalEntity.h"
#include "BEHAVE/Procedure.h"
#include "BEHAVE/Role.h"
#include "HAVE/VirtualHuman.h"

namespace BEHAVE
{
using namespace VEHA;
class ProcedureExecution;

class VEHA_API ActionListenerBehavior : public CyclicBehaviorExecution
{
public:
	ActionListenerBehavior(shared_ptr<Behavior> specif,shared_ptr<InstanceSpecification> host,const Parameters& p) ;

	void action ();
	vector<shared_ptr<ProcedureExecution> >& accessRunningProcedures()
	{
		return _runningProcedures;
	}

	void pushProcedureToDo(shared_ptr<Procedure> proc, shared_ptr<OrganisationalEntity> org, shared_ptr<Role> role, Parameters params);

	void onActionDone(shared_ptr<VEHA::Action> instance);

protected :
	void _buildActionsPossibles(shared_ptr<ProcedureExecution> procInfo);

	void _sendActionRealisationMessage(const string& action, shared_ptr<ProcedureExecution> procInfo);
	void _sendActionDoneMessage(const string& action, shared_ptr<ProcedureExecution> procInfo);
	void _onBehaviorStop(BehaviorExecution* be);

private :
	vector<shared_ptr<ProcedureExecution> > _runningProcedures;
	map<BehaviorExecution*,shared_ptr<ActionNode> >  _behaviorToNode;
	map<string, shared_ptr<InstanceSpecification> > _affectations;
	map<string, shared_ptr<ActivityExecution> > _actionsPossibles;
	map<string, shared_ptr<InstanceSpecification> > _actionsPossiblesParameter;
	shared_ptr<ActivityExecution> _lastActionPossible;
};
}
#endif
