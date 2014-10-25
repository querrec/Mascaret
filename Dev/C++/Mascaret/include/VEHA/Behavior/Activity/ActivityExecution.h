#ifndef  _v2_VEHA_Behavior_Activity_ActivityExecution_H
#define  _v2_VEHA_Behavior_Activity_ActivityExecution_H

#include "Tools/veha_plateform.h"
#include "VEHA/Kernel/Element.h"
#include "VEHA/Behavior/Activity/Activity.h"
#include "VEHA/Behavior/Activity/ActivityPartition.h"
#include "VEHA/Behavior/Activity/ActionNode.h"

namespace VEHA
{
class VEHA_API ActivityExecution
{
public :
	ActivityExecution(const shared_ptr<Activity> act);
	virtual ~ActivityExecution();

	vector<shared_ptr<ActionNode> > getNextActions(const string& roleName);
	bool isFinished();
	void informActionRunning(const string& roleName,const string& actionName);
	void informActionDone(const string& roleName,const string& actionName);

	void printState();
	virtual void updateNodes();

	// Execution
protected:

	shared_ptr<Activity> _activity;
	vector < shared_ptr<ActionNode> > _runningNodes;
	vector < shared_ptr<ActionNode> > _toExecuteNodes;
	vector<pair<string,string> > _actionsDone;
	vector<pair<string,string> > _actionsRunning;
};

inline bool ActivityExecution::isFinished()
{
	return _toExecuteNodes.size() == 0 && _runningNodes.size() == 0;
}
inline void ActivityExecution::informActionRunning(const string& roleName,const string& actionName)
{
	
	_actionsRunning.push_back(make_pair(roleName, actionName));
}
inline void ActivityExecution::informActionDone(const string& roleName,const string& actionName)
{
	_actionsDone.push_back(make_pair(roleName,actionName));
}
}

#endif

