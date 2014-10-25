#ifndef _v2_VEHA_Behavior_Activity_ControlNode_H
#define _v2_VEHA_Behavior_Activity_ControlNode_H

#include "Tools/veha_plateform.h"
#include "VEHA/Behavior/Activity/ActivityNode.h"

namespace VEHA
{

class VEHA_API ControlNode : public ActivityNode
{
public :
	ControlNode(const string & name, const string & kind);
	virtual ~ControlNode();
};

}
#endif
