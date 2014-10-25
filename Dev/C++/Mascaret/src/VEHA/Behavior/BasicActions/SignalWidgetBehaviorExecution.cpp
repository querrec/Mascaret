#include "VEHA/Behavior/Common/Signal.h"
#include "VEHA/Behavior/BasicActions/SignalWidgetBehaviorExecution.h"
#include "VEHA/Behavior/StateMachine/StateMachineBehaviorExecution.h"
#include "VEHA/Kernel/InstanceSpecification.h"
#include "VEHA/Kernel/Environment.h"
#include "VEHA/Entity/Entity.h"
#include "HAVE/VirtualHuman.h"
#include "IEHA/VRApplication.h"
#include "HAVE/Body.h"
#include "MascaretApplication.h"
#include <boost/bind.hpp>

using namespace std;
using namespace IEHA;

namespace VEHA
{
  
  SignalWidgetBehaviorExecution::SignalWidgetBehaviorExecution(shared_ptr<SignalWidgetAction> action,
							       shared_ptr<InstanceSpecification> host, const Parameters& p):BehaviorExecution(action,host,p), _action(action)
  {}
  
  SignalWidgetBehaviorExecution::~SignalWidgetBehaviorExecution()
  {}
  
  double SignalWidgetBehaviorExecution::execute(double dt)
  {
    shared_ptr<Entity> entity = shared_dynamic_cast<Entity>(_action->getTarget());
    
    if(entity && entity->getActiveShape())
      {
        // TODO: We only show operation en window 0. how to find a method for
        // detecting clicked window ?
        cout << entity->getFullName() << endl;
        VRApplication::getInstance()->getWindows()[0]->showSignals(entity, MascaretApplication::getInstance()->getEnvironment());
      }
    
    return 0;
  }

} // Namespace
