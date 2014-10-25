#include "VEHA/Behavior/BasicActions/AddStructuralFeatureValueBehaviorExecution.h"
#include "VEHA/Kernel/InstanceSpecification.h"
#include <boost/bind.hpp>


namespace VEHA
{

AddStructuralFeatureValueBehaviorExecution::AddStructuralFeatureValueBehaviorExecution(shared_ptr<AddStructuralFeatureValueAction> action, shared_ptr<InstanceSpecification> host, const Parameters& p):BehaviorExecution(action,host,p), _action(action)
{
}

AddStructuralFeatureValueBehaviorExecution::~AddStructuralFeatureValueBehaviorExecution()
{
}

double AddStructuralFeatureValueBehaviorExecution::execute(double dt)
{
        string propertyName = _action->getProperty();

        shared_ptr<Slot> property = getHost()->getProperty(propertyName);
   
        shared_ptr<Expression> exp = _action->getValue();
        string val = exp->evaluateExpression(getHost()); 
        shared_ptr<ValueSpecification> valSpecif=property->getDefiningProperty()->createValueFromString(val);

        property->addValue(valSpecif);
	return 0;
}

} // Namespace
