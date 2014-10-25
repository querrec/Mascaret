#include "VEHA/Behavior/Common/OpaquePythonBehavior.h"
#include "VEHA/Behavior/Common/PythonBehaviorExecution.h"
#include "VEHA/Behavior/Common/pluginManager.h"

namespace VEHA
{

OpaquePythonBehavior::OpaquePythonBehavior(const string& name)
:OpaqueBehavior(name)
{
}

OpaquePythonBehavior::~OpaquePythonBehavior()
{
}

//	typedef BehaviorExecution * (*BehaviorInitFunc)(InstanceSpecification *,const Parameters&);
shared_ptr<BehaviorExecution> OpaquePythonBehavior::createBehaviorExecution(shared_ptr<InstanceSpecification> host,const Parameters& p)
{
	return
			shared_ptr<PythonBehaviorExecution>
	(new PythonBehaviorExecution(shared_dynamic_cast<OpaquePythonBehavior>(shared_from_this()),host,p));
}

}
