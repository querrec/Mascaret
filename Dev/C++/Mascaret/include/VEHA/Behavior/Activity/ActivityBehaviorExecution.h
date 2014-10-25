#ifndef _v2_VEHA_Behavior_Activity_ActivityBehaviorExecution_H
#define _v2_VEHA_Behavior_Activity_ActivityBehaviorExecution_H

#include "Tools/veha_plateform.h"
#include "VEHA/Behavior/Common/BehaviorExecution.h"
#include "VEHA/Behavior/Common/Parameters.h"
#include "VEHA/Behavior/Common/ActivityBehaviorExecutionHelper.h"
#include "VEHA/Behavior/Activity/Activity.h"


namespace VEHA
{

class VEHA_API ActivityBehaviorExecution : public BehaviorExecution, ActivityBehaviorExecutionHelper, boost::noncopyable
{
public :
	ActivityBehaviorExecution(shared_ptr<Activity> Activity, shared_ptr<InstanceSpecification> host, const Parameters& p, bool sync = false);
	virtual ~ActivityBehaviorExecution();

	inline shared_ptr<Activity> getActivity(void) { return shared_dynamic_cast<Activity>(getSpecification());}

	void fire(shared_ptr<ActivityNode> activityNode);

	void stop();

	virtual double execute(double dt);
protected:
	void buildAffectations(Parameters params, shared_ptr<InstanceSpecification> host, shared_ptr<Activity> activity);
	virtual void _buildAffectationsObject(vector<string> & missing, shared_ptr<ActivityNode> node);
	virtual void _buildAffectationsAction(vector<string> & missing, shared_ptr<ActivityNode> node);
	virtual void _buildAffectationsPostbone(vector<string> & missing);
	void _findRessources(vector<string> missing);
   
	map<BehaviorExecution*,shared_ptr<ActionNode> >  _behaviorToNode;
	vector< shared_ptr<ActionNode> > _actionsRunning;
	vector<shared_ptr<ActivityExecutionToken> > _activeTokens;
	vector< shared_ptr<ActionNode> > _actionsDone;
	bool _tryToAdvanceToken(shared_ptr<ActivityExecutionToken> token, shared_ptr<ActionNode> doneAction, bool gotTicketToUseAction=true);
	bool _actionIsCurrentlyRunning(shared_ptr<ActionNode> action);
	vector<shared_ptr<ActionNode> > getActionToExecute();
	vector<shared_ptr<ActionNode> > getAllActions();
	void informActionRunning(const shared_ptr<ActionNode> action);
	void informActionDone(const shared_ptr<ActionNode> action);
	void _onBehaviorStop(BehaviorExecution* be);

private :
	int _stateActivity;
	vector<shared_ptr<ActionNode> > _actions;
	vector<shared_ptr<ActionNode> > _execs;
	shared_ptr<Activity> _activity;
};

}
#endif
