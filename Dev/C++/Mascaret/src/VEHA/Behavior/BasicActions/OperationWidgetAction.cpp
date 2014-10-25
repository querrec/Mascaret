#include "VEHA/Behavior/BasicActions/OperationWidgetAction.h"
#include "VEHA/Behavior/BasicActions/OperationWidgetBehaviorExecution.h"

namespace VEHA
{

  OperationWidgetAction::OperationWidgetAction()
  {
    _kind = "OperationWidget";
  }

  OperationWidgetAction::~OperationWidgetAction()
  {
  }

  shared_ptr<BehaviorExecution> OperationWidgetAction::createBehaviorExecution(shared_ptr<InstanceSpecification> host, const Parameters& p, bool sync)
  {
    shared_ptr<OperationWidgetBehaviorExecution> behavior(new OperationWidgetBehaviorExecution(shared_dynamic_cast<OperationWidgetAction>(shared_from_this()),host,p));
    return behavior;
  }
  
}
