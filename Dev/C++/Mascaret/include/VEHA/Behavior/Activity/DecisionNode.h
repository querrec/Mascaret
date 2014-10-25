#ifndef _v2_VEHA_Behavior_Activity_DecisionNode_H
#define _v2_VEHA_Behavior_Activity_DecisionNode_H

#include "Tools/veha_plateform.h"
#include "VEHA/Behavior/Activity/ControlNode.h"

namespace VEHA
{

class VEHA_API DecisionNode : public ControlNode
{
public :
	DecisionNode();
	virtual ~DecisionNode();
	// --- Attributes ---
	// --- Relations ---
	// --- Operations ---
	virtual vector<shared_ptr<ActionNode> > getOutgoingActionNode();
	virtual vector<shared_ptr<ActionNode> > getOutgoingActionNodeByPartition(const string& partitionName);
};

}
#endif
