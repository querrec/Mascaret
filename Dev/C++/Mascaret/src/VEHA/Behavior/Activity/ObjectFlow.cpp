#include "VEHA/Behavior/Activity/ObjectFlow.h"

namespace VEHA
{

ObjectFlow::ObjectFlow(const string& name) : ActivityEdge(name)
{
	setKind("ObjectFlow");
}

ObjectFlow::~ObjectFlow()
{
}

}
