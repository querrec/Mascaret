#include "VEHA/Behavior/Common/Signal.h"
#include "VEHA/Behavior/BasicActions/ChangeViewPointBehaviorExecution.h"
#include "VEHA/Entity/Entity.h"
#include "VEHA/Entity/ShapeSpecification.h"
#include "VEHA/Entity/EntityClass.h"
#include "VEHA/Kernel/Environment.h"
#include "IEHA/VRApplication.h"
#include "IEHA/Visualisation/Camera.h"
#include <boost/bind.hpp>

using namespace IEHA;

namespace VEHA
{

ChangeViewPointBehaviorExecution::ChangeViewPointBehaviorExecution(shared_ptr<ChangeViewPointAction> action,
		shared_ptr<InstanceSpecification> host, const Parameters& p):BehaviorExecution(action,host,p), _action(action)
{
}

ChangeViewPointBehaviorExecution::~ChangeViewPointBehaviorExecution()
{
}

double ChangeViewPointBehaviorExecution::execute(double dt)
{

        cerr << "CHANGE VIEWPOINT" << endl;
        string viewPointName = _action->getViewPointName();
        if (viewPointName != "") _action->getCamera()->gotoViewPoint(viewPointName);
        else
        {
           string viewPointDirection = _action->getViewPointDirection();
           if (viewPointDirection == "next") _action->getCamera()->gotoNextViewPoint();
           else if (viewPointDirection == "previous") _action->getCamera()->gotoPreviousViewPoint();
        }
	return 0;
}

} // Namespace
