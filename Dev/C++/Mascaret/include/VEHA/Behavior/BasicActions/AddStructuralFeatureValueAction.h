#ifndef _v2_VEHA_Behavior_BasicActions_AddStructuralFeatureValueAction_H
#define _v2_VEHA_Behavior_BasicActions_AddStructuralFeatureValueAction_H

#include "Tools/veha_plateform.h"
#include "VEHA/Kernel/Property.h"
#include "VEHA/Kernel/Expression.h"
#include "VEHA/Behavior/BasicActions/Action.h"

namespace VEHA
{
class InstanceSpecification;

class VEHA_API AddStructuralFeatureValueAction : public Action
{
public :
	AddStructuralFeatureValueAction();
	virtual ~AddStructuralFeatureValueAction();

protected :
	//shared_ptr<Property>    _property;
	shared_ptr<Expression> _value;
	shared_ptr<InstanceSpecification>    _target;
        string _property;
public :
	inline string getProperty(void) { return _property;}
	inline void setProperty(string property) { _property = property;}

	inline shared_ptr<Expression> getValue(void) { return _value;}
	inline void setValue(shared_ptr<Expression> value) { _value = value;}

	inline shared_ptr<InstanceSpecification> getTarget(void) { return _target;}
	inline void setTarget(shared_ptr<InstanceSpecification> target) { _target = target;}

	virtual shared_ptr<BehaviorExecution> createBehaviorExecution(shared_ptr<InstanceSpecification>, const Parameters&, bool sync = false);

};

}
#endif
