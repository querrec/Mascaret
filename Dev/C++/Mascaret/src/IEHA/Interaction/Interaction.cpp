/*
 * Interaction.cpp
 *
 *  Created on: 4 mars 2011
 *      Author: soler
 */

#include "IEHA/Interaction/Interaction.h"
#include "IEHA/Interaction/Button.h"
#include "IEHA/Interaction/Axis.h"
#include "IEHA/Visualisation/Camera.h"
#include "VEHA/Entity/Entity.h"
#include "VEHA/Kernel/Operation.h"
#include "VEHA/Behavior/Common/BehaviorExecution.h"
#include "VEHA/Behavior/Common/Signal.h"
#include "VEHA/Behavior/BasicActions/Action.h"
#include "VEHA/Behavior/BasicActions/SendSignalAction.h"
#include "VEHA/Behavior/BasicActions/CallOperationAction.h"
#include "VEHA/Behavior/BasicActions/ChangeViewPointAction.h"
#include "VEHA/Behavior/BasicActions/OperationWidgetAction.h"
#include "VEHA/Behavior/BasicActions/SignalWidgetAction.h"
#include "VEHA/Behavior/Common/behaviorScheduler.h"
//#include "MATS/Poseidon/BasicActions/OpenDocumentAction.h"
//#include "MATS/Poseidon/BasicActions/LoadModelAction.h"
#include "IEHA/VRApplication.h"
#include "BEHAVE/AgentPlateform.h"
#include "BEHAVE/Agent.h"
#include "HAVE/Body.h"
using namespace VEHA;
using HAVE::Body;
namespace IEHA
{
Interaction::Interaction(shared_ptr<Window3D> window):_window(window)
{

}
Interaction::~Interaction()
{
}
void Interaction::addButtonAction(shared_ptr<Button> button,shared_ptr<Action> action,const string& target,bool pressed,const string& cursor, double maxdist)
{
	if(_buttonConnections.find(button) == _buttonConnections.end())
		_buttonConnections[button]=button->addCallbackPressed(shared_from_this(),&Interaction::_onButtonPressed);
	ButtonActionInfo info;
	info.button=button;
	info.action=action;
	info.target=target;
	info.pressed=pressed;
	info.cursor=cursor;
	info.maxdist=maxdist;
	_buttonActions.push_back(info);
}
void Interaction::removeButtonAction(shared_ptr<Button> button)
{
	if(_buttonConnections.find(button) != _buttonConnections.end())
	{
		_buttonConnections[button].disconnect();
		_buttonConnections.erase(button);
	}
}
void Interaction::removeAllButtonActions()
{
	for(map<shared_ptr<Button>,boost::signals::connection >::iterator i=_buttonConnections.begin();i!=_buttonConnections.end();i++)
		i->second.disconnect();
	_buttonConnections.clear();
}
void Interaction::_onButtonPressed(shared_ptr<Button> button,bool pressed)
{
	shared_ptr<Action> action;
	string target;
	string cursor;
	double maxdist;

	shared_ptr<Agent> agt ;
	shared_ptr<AgentPlateform> ap = MascaretApplication::getInstance()->getAgentPlateform();
	map<string, shared_ptr<Agent> > agents = ap->getAllAgents();
	map<string, shared_ptr<Agent> >::iterator it;
	for (it = agents.begin(); it != agents.end(); it++)
	{
		if (it->second->getControlledByHuman()) 
		{
			agt = it->second;
			cerr << " &&&&&&&&&&&& Interaction done by : " << agt->getName() << endl;
		}
	}

	for(size_t i=0;i<_buttonActions.size();i++)
	{
		if(_buttonActions[i].pressed == pressed && _buttonActions[i].button == button)
		{
            
			action=_buttonActions[i].action;
			target=_buttonActions[i].target;
			cursor=_buttonActions[i].cursor;
			maxdist=_buttonActions[i].maxdist;
			if(target == "broadcast")
			{
				//no target = broadcast. Just choose an arbitrary entity to execute
				vector<shared_ptr<Entity> > entities=MascaretApplication::getInstance()->getEnvironment()->getEntities();
				for(size_t i=0;i<entities.size();i++)
				{
					shared_ptr<Action> action2;
					if(shared_dynamic_cast<SendSignalAction>(action))
					{
						shared_ptr<Signal> signalC = shared_dynamic_cast<SendSignalAction>(action)->getSignalClass();
                                                shared_ptr<InstanceSpecification> signal (new InstanceSpecification(signalC->getName(), signalC));

						action2=shared_ptr<Action>(new SendSignalAction());
						shared_dynamic_cast<SendSignalAction>(action2)->setSignalClass(shared_dynamic_cast<SendSignalAction>(action)->getSignalClass());
						shared_dynamic_cast<SendSignalAction>(action2)->setTarget(entities[i]);
						shared_dynamic_cast<SendSignalAction>(action2)->setSignal(signal);
					}
					else if(shared_dynamic_cast<CallOperationAction>(action))
					{
						action2=shared_ptr<Action>(new CallOperationAction());
						shared_dynamic_cast<CallOperationAction>(action2)->setOperation(shared_dynamic_cast<CallOperationAction>(action)->getOperation());
						shared_dynamic_cast<CallOperationAction>(action2)->setTarget(entities[i]);
					}
					if (agt)
						BehaviorScheduler::getInstance()->executeBehavior(action2,agt,Parameters());
					else
						BehaviorScheduler::getInstance()->executeBehavior(action2,entities[i],Parameters());
					_cbActionDone(action2);
					//shared_ptr<BehaviorExecution> exec=action->createBehaviorExecution(entities[i],Parameters());
					//exec->execute(); //BasicActions are unitary
					//exec->stop(); // to set finished
				}
			}
			else if(target == "designated")
			{

				int x,y;
				_window->getCursor(cursor)->getPosition(x,y);
				double dist;
				shared_ptr<InstanceSpecification> entity=_window->getCamera()->getEntity(x,y,dist);

				if(maxdist > 0 && maxdist < dist)
					continue;
				if(shared_dynamic_cast<Body>(entity))
				{
					entity=shared_dynamic_cast<Body>(entity)->getAgent();
				}
				if(entity)
				{
					if(shared_dynamic_cast<SendSignalAction>(action))
					{
						shared_ptr<Signal> signalC = shared_dynamic_cast<SendSignalAction>(action)->getSignalClass();
                                                shared_ptr<InstanceSpecification> signal (new InstanceSpecification(signalC->getName(), signalC));
						shared_dynamic_cast<SendSignalAction>(action)->setSignal(signal);
						shared_dynamic_cast<SendSignalAction>(action)->setTarget(entity);
					}
					else if(shared_dynamic_cast<CallOperationAction>(action))
					{
                                                //cerr << "CALL OPERATION ..." << endl;
						if(!entity->getClassifier()->isA(shared_dynamic_cast<CallOperationAction>(action)->getOperation()->getClassifier()))
						{
							continue;
						}
						shared_dynamic_cast<CallOperationAction>(action)->setTarget(entity);
					}
                                        else if(shared_dynamic_cast<OperationWidgetAction>(action))
                                          {
                                            shared_dynamic_cast<OperationWidgetAction>(action)->setTarget(entity);
                                          }
                                        else if(shared_dynamic_cast<SignalWidgetAction>(action))
                                          {
                                            shared_dynamic_cast<SignalWidgetAction>(action)->setTarget(entity);
                                          }
//					else if(shared_dynamic_cast<OpenDocumentAction>(action))
//                                         {
//                                            shared_dynamic_cast<OpenDocumentAction>(action)->setTarget(entity);
//                                         }
					//else if(shared_dynamic_cast<LoadModelAction>(action))
					//{
					//	shared_dynamic_cast<LoadModelAction>(action)->setTarget(entity);
					//}

					if (agt)
						BehaviorScheduler::getInstance()->executeBehavior(action,agt,Parameters());
					else
						BehaviorScheduler::getInstance()->executeBehavior(action,entity,Parameters());
					_cbActionDone(action);
					//shared_ptr<BehaviorExecution> exec=action->createBehaviorExecution(entity,Parameters());
					//exec->execute(); //BasicActions are unitary
					//exec->stop(); // to set finished
				}
			}
			else if(target == "none")
			{
				shared_ptr<InstanceSpecification> instance;
/*
				if(shared_dynamic_cast<SendSignalAction>(action))
				{
					shared_dynamic_cast<SendSignalAction>(action)->setTarget(instance);
				}
				else if(shared_dynamic_cast<CallOperationAction>(action))
				{
					shared_dynamic_cast<CallOperationAction>(action)->setTarget(instance);
				}
				BehaviorScheduler::getInstance()->executeBehavior(action,instance,Parameters());
				_cbActionDone(action);
			}
*/
        if(shared_dynamic_cast<SendSignalAction>(action))
        {
          shared_dynamic_cast<SendSignalAction>(action)->setTarget(instance);
        }
        else if(shared_dynamic_cast<CallOperationAction>(action))
        {
          shared_dynamic_cast<CallOperationAction>(action)->setTarget(instance);
        }
        else if(shared_dynamic_cast<ChangeViewPointAction>(action))
        {
          shared_dynamic_cast<ChangeViewPointAction>(action)->setCamera(_window->getCamera());
        }
        if (agt)
		BehaviorScheduler::getInstance()->executeBehavior(action,agt,Parameters());
	else
        	BehaviorScheduler::getInstance()->executeBehavior(action,instance,Parameters());
        _cbActionDone(action);

      }
			else
			{
				shared_ptr<InstanceSpecification> instance=MascaretApplication::getInstance()->getEnvironment()->getInstance(target);
				if(instance)
				{
					if(shared_dynamic_cast<SendSignalAction>(action))
					{
						shared_dynamic_cast<SendSignalAction>(action)->setTarget(instance);
					}
					else if(shared_dynamic_cast<CallOperationAction>(action))
					{
						shared_dynamic_cast<CallOperationAction>(action)->setTarget(instance);
					}
					BehaviorScheduler::getInstance()->executeBehavior(action,instance,Parameters());
					_cbActionDone(action);
					//shared_ptr<BehaviorExecution> exec=action->createBehaviorExecution(instance,Parameters());
					//exec->execute(); //BasicActions are unitary
					//exec->stop(); // to set finished
				}
				else
				{
					cerr << "Instance "<<target << " not found"<<endl;
				}
			}
		}
	}

}


}
