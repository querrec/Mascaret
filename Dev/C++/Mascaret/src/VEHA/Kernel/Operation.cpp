#include "VEHA/Kernel/Operation.h"
#include "VEHA/Kernel/Class.h"
#include "VEHA/Kernel/Classifier.h"
#include "VEHA/Kernel/Parameter.h"
#include "VEHA/Kernel/ValueSpecification.h"
#include "VEHA/Behavior/Common/Parameters.h"
#include "Tools/utils.h"

namespace VEHA
{

Operation::Operation(const string& name,shared_ptr<Class> owner,shared_ptr<Behavior> behavior)
:BehavioralFeature(name,behavior),_classifier(owner),_type()
{
}

Operation::~Operation()
{
}

bool Operation::createBehaviorParameters(const OperationCallParameters& p,Parameters & behaviorParameters)
{
	if (p.size()!=_parameter.size())
		return false;
	for(size_t i=p.size();i--;)
	{
		if (!p[i]->getType()->isA(_parameter[i]->getType()))
			return false;
		behaviorParameters[_parameter[i]->getName()]=p[i];
	}
	return true;
}

}
