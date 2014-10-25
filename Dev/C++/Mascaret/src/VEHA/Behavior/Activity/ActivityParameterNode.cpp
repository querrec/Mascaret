#include "VEHA/Behavior/Activity/ActivityParameterNode.h"

namespace VEHA
{

ActivityParameterNode::ActivityParameterNode(const string& name) : ObjectNode(name)
{
setKind("parameter");
}

ActivityParameterNode::~ActivityParameterNode()
{
}

}
