#ifndef _v2_VEHA_Behavior_Activity_ActivityParameterNode_H
#define _v2_VEHA_Behavior_Activity_ActivityParameterNode_H

#include "Tools/veha_plateform.h"
#include "VEHA/Behavior/Activity/ObjectNode.h"
#include "VEHA/Kernel/Classifier.h"
#include "VEHA/Kernel/ValueSpecification.h"
#include "VEHA/Kernel/Parameter.h"

namespace VEHA
{
class VEHA_API ActivityParameterNode : public ObjectNode
{
public :
	ActivityParameterNode(const string& name);
	virtual ~ActivityParameterNode();

        inline void setParameter(shared_ptr<Parameter> parameter) {_parameter = parameter;}
        inline shared_ptr<Parameter> getParameter(void) {return _parameter ;}

protected:
	shared_ptr<Parameter> _parameter;
};

}
#endif
