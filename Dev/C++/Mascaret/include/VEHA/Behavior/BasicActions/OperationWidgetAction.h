#ifndef _v2_VEHA_Behavior_BasicActions_OperationWidgetAction_H
#define _v2_VEHA_Behavior_BasicActions_OperationWidgetAction_H

#include "Tools/veha_plateform.h"
#include "VEHA/Behavior/BasicActions/Action.h"

using namespace std;
namespace VEHA
{
  class InstanceSpecification;
  class Signal;
  
  class VEHA_API OperationWidgetAction : public Action
  {
  public :
    OperationWidgetAction();
    virtual ~OperationWidgetAction();
  protected :
    shared_ptr<InstanceSpecification> _target;
  public :
    inline shared_ptr<InstanceSpecification> getTarget() { return _target; }
    inline void setTarget(shared_ptr<InstanceSpecification> target) { _target = target; }
    virtual shared_ptr<BehaviorExecution> createBehaviorExecution(shared_ptr<InstanceSpecification>, const Parameters&, bool sync = false);
  };
  
}
#endif
