#include "VEHA/Behavior/Activity/ControlFlow.h"

namespace VEHA
{

ControlFlow::ControlFlow(const string& name) : ActivityEdge(name)
{
	setKind("ControlFlow");
}

ControlFlow::~ControlFlow()
{
}

}
