#include "VEHA/Behavior/BasicActions/AddStructuralFeatureValueAction.h"
#include "VEHA/Behavior/BasicActions/AddStructuralFeatureValueBehaviorExecution.h"
#include <boost/algorithm/string.hpp>

namespace VEHA
{

AddStructuralFeatureValueAction::AddStructuralFeatureValueAction()
{
	_kind = "AddStructuralFeatureValue";
}

AddStructuralFeatureValueAction::~AddStructuralFeatureValueAction()
{
}

shared_ptr<BehaviorExecution> AddStructuralFeatureValueAction::createBehaviorExecution(shared_ptr<InstanceSpecification> host, const Parameters& p, bool sync)
{
	shared_ptr<AddStructuralFeatureValueBehaviorExecution> behavior(new AddStructuralFeatureValueBehaviorExecution(shared_dynamic_cast<AddStructuralFeatureValueAction>(shared_from_this()),host,p));
	return behavior;
}

}
