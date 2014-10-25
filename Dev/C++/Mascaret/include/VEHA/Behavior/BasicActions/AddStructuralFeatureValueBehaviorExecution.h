#ifndef _v2_VEHA_Behavior_BasicAction_AddStructuralFeatureValueBehaviorExecution_H
#define _v2_VEHA_Behavior_BasicAction_AddStructuralFeatureValueBehaviorExecution_H

#include "Tools/veha_plateform.h"
#include "VEHA/Behavior/Common/BehaviorExecution.h"
#include "VEHA/Behavior/BasicActions/AddStructuralFeatureValueAction.h"

namespace VEHA
{
class InstanceSpecification;

class VEHA_API AddStructuralFeatureValueBehaviorExecution : public BehaviorExecution, boost::noncopyable
{
public :
	AddStructuralFeatureValueBehaviorExecution(shared_ptr<AddStructuralFeatureValueAction> action, shared_ptr<InstanceSpecification> host, const Parameters& p);
	virtual ~AddStructuralFeatureValueBehaviorExecution();

/*
	virtual void stop();
	virtual void restart();
	virtual void pause();
*/

	virtual double execute(double dt);
private :
	shared_ptr<AddStructuralFeatureValueAction> _action;

};

}
#endif
