#include "VEHA/Behavior/BasicActions/SignalWidgetAction.h"
#include "VEHA/Behavior/BasicActions/SignalWidgetBehaviorExecution.h"

namespace VEHA
{

  SignalWidgetAction::SignalWidgetAction()
  {
    _kind = "SignalWidget";
  }

  SignalWidgetAction::~SignalWidgetAction()
  {
  }

  shared_ptr<BehaviorExecution> SignalWidgetAction::createBehaviorExecution(shared_ptr<InstanceSpecification> host, const Parameters& p, bool sync)
  {
    shared_ptr<SignalWidgetBehaviorExecution> behavior(new SignalWidgetBehaviorExecution(shared_dynamic_cast<SignalWidgetAction>(shared_from_this()),host,p));
    return behavior;
  }
  
}
