#ifndef _v2_VEHA_Behavior_BasicAction_OperationWidgetBehaviorExecution_H
#define _v2_VEHA_Behavior_BasicAction_OperationWidgetBehaviorExecution_H

#include "VEHA/Behavior/Common/BehaviorExecution.h"
#include "VEHA/Behavior/Common/Parameters.h"
#include "VEHA/Behavior/BasicActions/OperationWidgetAction.h"

namespace VEHA
{

class Entity;

 class VEHA_API OperationWidgetBehaviorExecution : public BehaviorExecution, boost::noncopyable
  {
  public :
    OperationWidgetBehaviorExecution(shared_ptr<OperationWidgetAction> action, shared_ptr<InstanceSpecification> host, const Parameters& p);
    virtual ~OperationWidgetBehaviorExecution();    
    virtual double execute(double dt);

  private :
    shared_ptr<OperationWidgetAction> _action;
  };
 
}
#endif
