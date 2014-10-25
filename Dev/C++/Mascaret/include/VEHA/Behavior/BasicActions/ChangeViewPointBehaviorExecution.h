#ifndef _ChangeViewPointBehaviorExecution_H
#define _ChangeViewPointBehaviorExecution_H

#include "Tools/veha_plateform.h"
#include "VEHA/Behavior/Common/BehaviorExecution.h"
#include "VEHA/Behavior/Common/Parameters.h"
#include "VEHA/Behavior/BasicActions/ChangeViewPointAction.h"


namespace VEHA
{
class Entity;

class VEHA_API ChangeViewPointBehaviorExecution : public BehaviorExecution, boost::noncopyable
{
public :
	ChangeViewPointBehaviorExecution(shared_ptr<ChangeViewPointAction> action, shared_ptr<InstanceSpecification> host, const Parameters& p);
	virtual ~ChangeViewPointBehaviorExecution();

	virtual double execute(double dt);

private :
	shared_ptr<ChangeViewPointAction> _action;
};

}
#endif
