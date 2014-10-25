#ifndef _v2_VEHA_Behavior_Activity_ChoiceNode_H
#define _v2_VEHA_Behavior_Activity_ChoiceNode_H

#include "Tools/veha_plateform.h"
#include "VEHA/Behavior/Activity/ControlNode.h"

namespace VEHA
{

class VEHA_API ChoiceNode : public ControlNode
{
public :
	ChoiceNode();
	virtual ~ChoiceNode();

	virtual vector<shared_ptr<ActionNode> > getOutgoingActionNode();
	virtual vector<shared_ptr<ActionNode> > getOutgoingActionNodeByPartition(const string& partitionName);
	virtual vector<shared_ptr<ActionNode> > getPossibleOutgoingActionNode();
protected:
	static size_t choiceNodeCount;
};

}
#endif
