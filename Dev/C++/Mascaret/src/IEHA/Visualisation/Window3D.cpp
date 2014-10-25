/*
 * Window3D.cpp
 *
 *  Created on: 27 févr. 2011
 *      Author: ping
 */
#include "VEHA/Behavior/Common/Signal.h"
#include "VEHA/Behavior/Common/Trigger.h"
#include "VEHA/Behavior/Common/Event.h"
#include "VEHA/Kernel/InstanceSpecification.h"
#include "VEHA/Behavior/StateMachine/StateMachine.h"
#include "VEHA/Behavior/StateMachine/Region.h"
#include "VEHA/Behavior/StateMachine/Transition.h"
#include "VEHA/Behavior/StateMachine/SignalEvent.h"
#include "VEHA/Behavior/StateMachine/StateMachineBehaviorExecution.h"
#include "VEHA/Behavior/BasicActions/Action.h"
#include "VEHA/Behavior/BasicActions/SendSignalAction.h"

using namespace VEHA;

#include "IEHA/Visualisation/Window3D.h"
#include "IEHA/Interaction/Interaction.h"
#include "IEHA/Interaction/Navigation.h"
#include "MascaretApplication.h"
#include "BEHAVE/OrganisationalEntity.h"
#include "BEHAVE/OrganisationalStructure.h"
#include "BEHAVE/RoleAssignement.h"
#include "BEHAVE/Procedure.h"
#include "BEHAVE/Agent.h"
#include "BEHAVE/AgentPlateform.h"
//#include <sys/time.h>
#include "VEHA/Behavior/Activity/ObjectNode.h"
#include "VEHA/Behavior/BasicActions/CallOperationAction.h"
#include "VEHA/Kernel/Operation.h"
#include "iostream"
#include "fstream"
using namespace std;

namespace IEHA
{
Window3D::Window3D()
{
}
Window3D::~Window3D()
{
	if(_navigation)
		_navigation->removeAllNavigationAxes();
}
}
