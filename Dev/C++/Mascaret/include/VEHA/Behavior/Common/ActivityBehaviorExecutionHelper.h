#ifndef __VEHA_BEHAVIOR_COMMON_ACTIVITY_ACTIVITYBEHAVIOREXECUTIONHELPER__
#define __VEHA_BEHAVIOR_COMMON_ACTIVITY_ACTIVITYBEHAVIOREXECUTIONHELPER__

#include "Tools/veha_plateform.h"

#include "VEHA/Kernel/Expression.h"
#include "VEHA/Behavior/Common/Parameters.h"

namespace VEHA
{
class Activity;
class ActivityEdge;
class ActivityNode;
class InstanceSpecification;
class LoopNode;

class VEHA_API ActivityBehaviorExecutionHelper
{
protected:
	ActivityBehaviorExecutionHelper() {}
	virtual ~ActivityBehaviorExecutionHelper() {}
	virtual void _buildAffectations(shared_ptr<InstanceSpecification> host, shared_ptr<Activity> activity);
	virtual void _buildAffectationsObject(vector<string> & missing, shared_ptr<ActivityNode> node);
	virtual void _buildAffectationsAction(vector<string> & missing, shared_ptr<ActivityNode> node);
	virtual void _buildAffectationsPostbone(vector<string> & missing) {};
	virtual OclParser::Context _buildOclContext(Parameters param = Parameters());

	Parameters _activityParams;
	map<string, shared_ptr<InstanceSpecification> > _affectations;
};

struct ActivityExecutionToken : enable_shared_from_this<ActivityExecutionToken>
{
	shared_ptr<ActivityNode> currentLocation; //can be action, fork or join
	vector<shared_ptr<ActivityNode> > forkStack; //forks in which the token currently is
	vector<shared_ptr<LoopNode> > loopNodeStack; //loop nodes in which the token currently is
	shared_ptr<ActivityEdge> outgoingEdge; //used to know which branch was associated with this token
};
}
#endif /* #ifndef __VEHA_BEHAVIOR_COMMON_ACTIVITY_ACTIVITYBEHAVIOREXECUTIONHELPER__ */
