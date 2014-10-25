#ifndef _v2_VEHA_Behavior_BasicAction_SignalWidgetBehaviorExecution_H
#define _v2_VEHA_Behavior_BasicAction_SignalWidgetBehaviorExecution_H

#include "VEHA/Behavior/Common/BehaviorExecution.h"
#include "VEHA/Behavior/Common/Parameters.h"
#include "VEHA/Behavior/BasicActions/SignalWidgetAction.h"

namespace VEHA
{

class Entity;

 class VEHA_API SignalWidgetBehaviorExecution : public BehaviorExecution, boost::noncopyable
  {
  public :
    SignalWidgetBehaviorExecution(shared_ptr<SignalWidgetAction> action, shared_ptr<InstanceSpecification> host, const Parameters& p);
    virtual ~SignalWidgetBehaviorExecution();    
    virtual double execute(double dt);

  private :
    shared_ptr<SignalWidgetAction> _action;
  };
 
}
#endif
