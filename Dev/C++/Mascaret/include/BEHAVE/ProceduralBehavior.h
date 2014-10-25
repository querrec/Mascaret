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
#include "VEHA/Behavior/Common/ActivityBehaviorExecutionHelper.h"
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

class VEHA_API ProceduralBehavior : public BEHAVE::CyclicBehaviorExecution
{
public:
	ProceduralBehavior(shared_ptr<Behavior> specif,shared_ptr<InstanceSpecification> host,const Parameters& p) ;
	virtual ~ProceduralBehavior();

	void action ();
	vector<shared_ptr<ProcedureExecution> >& accessRunningProcedures()
	{
		return _runningProcedures;
	}

	void pushProcedureToDo(shared_ptr<Procedure> proc, shared_ptr<OrganisationalEntity> org, shared_ptr<Role> role, Parameters params);

	PRE_DEPRECATED void onActionDone(shared_ptr<AID> agent, const string& actionName) POST_DEPRECATED;
	PRE_DEPRECATED void onActionRunning(shared_ptr<AID> agent, const string& actionName) POST_DEPRECATED;
	void onActionDone(shared_ptr<AID> agent, const shared_ptr<ActionNode> action);
	void onActionRunning(shared_ptr<AID> agent, const shared_ptr<ActionNode> action);
	void stop(string procedureName);
	void stop(shared_ptr<Procedure> procedure);
	void stopAll();
	
	inline void pushActionToDo(shared_ptr<ActionNode> action) { _actionsToDo.push_back(action); }
	
public :
	void _sendActionRealisationMessage(shared_ptr<ActionNode> action, shared_ptr<ProcedureExecution> procInfo);
	void _sendActionDoneMessage(shared_ptr<ActionNode> action, shared_ptr<ProcedureExecution> procInfo);
	void _onBehaviorStop(BehaviorExecution* be);

	void _sendProcedureDoneMessage(shared_ptr<ProcedureExecution> procInfo);
	vector<shared_ptr<ProcedureExecution> > _runningProcedures;
private :
	map<BehaviorExecution*,shared_ptr<ActionNode> >  _behaviorToNode;
	
	vector<shared_ptr<ActionNode> > _actionsToDo;
};
}
