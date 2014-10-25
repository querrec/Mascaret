#ifndef BEHAVE_PROCEDURE_EXECUTION_H
#define BEHAVE_PROCEDURE_EXECUTION_H


#include "Tools/veha_plateform.h"
#include "VEHA/Kernel/Element.h"
#include "VEHA/Behavior/Activity/Activity.h"
#include "VEHA/Behavior/Activity/ActivityPartition.h"
#include "VEHA/Behavior/Activity/ActionNode.h"
#include "VEHA/Behavior/Activity/LoopNode.h"
#include "VEHA/Kernel/Constraint.h"
#include "BEHAVE/AID.h"
#include "VEHA/Behavior/Time/TimeExpression.h"
#include "VEHA/Kernel/Expression.h"
#include "VEHA/Behavior/Common/ActivityBehaviorExecutionHelper.h"

namespace VEHA
{
class ActivityExecutionToken;
}

namespace BEHAVE
{
using VEHA::Element;
using VEHA::Activity;
using VEHA::ActivityExecutionToken;
using VEHA::ActivityPartition;
using VEHA::ActivityNode;
using VEHA::ActivityEdge;
using VEHA::ActionNode;
using VEHA::LoopNode;
using VEHA::Constraint;
using VEHA::TimeExpression;
using VEHA::InstanceSpecification;
using VEHA::Parameters;

class Agent;
class Procedure;
class OrganisationalEntity;
class RessourceAssignement;
class RoleAssignement;

class VEHA_API ProcedureExecution : public enable_shared_from_this<ProcedureExecution>, VEHA::ActivityBehaviorExecutionHelper
{
public :
	ProcedureExecution(const shared_ptr<Procedure> procedure, shared_ptr<OrganisationalEntity> organisation, shared_ptr<Agent> agent);
	virtual ~ProcedureExecution();

	inline shared_ptr<Procedure> getProcedure(void) {return _procedure;}
	inline shared_ptr<OrganisationalEntity> getOrganisation(void) {return _organisation;}

private :
	shared_ptr<Procedure> _procedure;
	shared_ptr<OrganisationalEntity> _organisation;
	shared_ptr<Agent> _agent;
	
public:
	vector<shared_ptr<ActionNode> > getAllActions();
	vector<shared_ptr<ActionNode> > getAllActionsFor(shared_ptr<AID> agent);
	shared_ptr<ActionNode> getActionByNameFor(shared_ptr<AID> agent, string action);
	
	vector<shared_ptr<ActionNode> > getActionToExecute();
	vector<shared_ptr<ActionNode> > getActionToExecuteFor(shared_ptr<AID> agent);
	vector<shared_ptr<ActionNode> > getActionToExecuteForAll();
	
	vector<string> getAllActionsDoneBy(shared_ptr<AID> agent);
	vector<TimeExpression> getAllActionsDoneTimestampsBy(shared_ptr<AID> agent);
	
	vector<shared_ptr<AID> > getOtherAgents();
	
	bool isFinished();
	
	PRE_DEPRECATED void informActionRunning(shared_ptr<AID> agent,const string& actionName) POST_DEPRECATED;
	PRE_DEPRECATED void informActionDone(shared_ptr<AID> agent,const string& actionName) POST_DEPRECATED;
	void informActionRunning(shared_ptr<AID> agent,const shared_ptr<ActionNode> action);
	void informActionDone(shared_ptr<AID> agent,const shared_ptr<ActionNode> action);
	
	vector< pair<string, shared_ptr<ActionNode> > > getActionsRunning() { return _actionsRunning; }
	
	void printState();

	void buildAffectations(Parameters params, shared_ptr<InstanceSpecification> host);
	void setAffectations(map<string, shared_ptr<InstanceSpecification> > affectations) {_affectations = affectations;}
	map<string, shared_ptr<InstanceSpecification> > getAffectations() const {return _affectations;}

// Execution
	void stop();
protected:
	
	void _findRessources(vector<string> missing); 
	void _buildAffectationsObject(vector<string> & missing, shared_ptr<ActivityNode> node);
	void _buildAffectationsAction(vector<string> & missing, shared_ptr<ActivityNode> node);
	void _buildAffectationsPostbone(vector<string> & missing);
	VEHA::OclParser::Context _buildOclContext(Parameters param = Parameters());

	bool _tryToAdvanceToken(shared_ptr<ActivityExecutionToken> token, shared_ptr<ActionNode> doneAction, bool gotTicketToUseAction=true);
	
	bool _actionIsCurrentlyRunning(shared_ptr<ActionNode> action);
	
	vector< shared_ptr<ActionNode> > _actionsDone;
	vector<pair<string,shared_ptr<ActionNode> > > _actionsRunning;
	
	map<string, vector< shared_ptr<ActionNode> > > _allActionsDone; // key: AID
	map<string, vector<TimeExpression> > _allActionsDoneTimestamps;
	
	map<string, shared_ptr<ActivityPartition> > _agentToPartition; // Maybe we need to use weak_ptr 
	map<shared_ptr<ActivityPartition>, shared_ptr<AID> > _partitionToAgent; // Maybe we need to use weak_ptr 

	Parameters _activityParams;
	map<string, shared_ptr<InstanceSpecification> > _affectations;
	
	//----------------
public :
	
	vector<shared_ptr<ActivityExecutionToken> > _activeTokens;
};
}

#endif

