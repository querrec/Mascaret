#include "VEHA/Behavior/Common/Signal.h"
#include "VEHA/Behavior/Common/Trigger.h"
#include "VEHA/Behavior/Common/behaviorScheduler.h"
#include "VEHA/Behavior/Common/behaviorScheduler.h"
#include "VEHA/Behavior/StateMachine/PseudoState.h"
#include "VEHA/Behavior/StateMachine/FinalState.h"
#include "VEHA/Behavior/StateMachine/SignalEvent.h"
#include "VEHA/Behavior/StateMachine/TimeEvent.h"
#include "VEHA/Behavior/StateMachine/ChangeEvent.h"
#include "VEHA/Behavior/StateMachine/Transition.h"
#include "VEHA/Behavior/StateMachine/StateMachine.h"
#include "VEHA/Behavior/StateMachine/Region.h"
#include "VEHA/Behavior/StateMachine/StateMachineBehaviorExecution.h"
#include "VEHA/Kernel/Constraint.h"
#include "VEHA/Kernel/InstanceSpecification.h"
#include "VEHA/Kernel/Expression.h"
#include <boost/bind.hpp>


using namespace boost;

namespace VEHA
{

class TimeCallbackBehavior : public BehaviorExecution
{
public :
	TimeCallbackBehavior(shared_ptr<StateMachine> stateMachine, shared_ptr<InstanceSpecification> host, const Parameters& p, double time,shared_ptr<BehaviorExecution> sme) :BehaviorExecution(stateMachine,host,p), _time(time), _sme(sme)
	{}
	void setTime(double time) {_time = time;}
	double execute(double dt) {(shared_dynamic_cast<StateMachineBehaviorExecution>(_sme))->addTime(_time); return 0;}
private :
	double _time;
	shared_ptr<BehaviorExecution> _sme;
};

StateMachineBehaviorExecution::StateMachineBehaviorExecution(
		shared_ptr<StateMachine> stateMachine, shared_ptr<InstanceSpecification> host, const Parameters& p, bool sync):BehaviorExecution(stateMachine,host,p,sync), _toStop (false)
{
	_first = true;
        currentActionDone = true;
}

StateMachineBehaviorExecution::~StateMachineBehaviorExecution()
{
}

void StateMachineBehaviorExecution::setCurrentState(string name)
{
   shared_ptr<Region> region = getStateMachine()->getRegion()[0];
   vector< shared_ptr<Vertex> > vertices = region->getVertices();
   for(unsigned int i = 0; i< vertices.size(); i++)
   {
      if (vertices[i]->getName() == name)
      {
	Parameters param;
        if (_be)
        {
          _be->stop();
          BehaviorScheduler::getInstance()->deleteExecutionBehavior(_be);
          _be=shared_ptr<BehaviorExecution>();
        }
        activateState(vertices[i], param);
      }
   }
}

void StateMachineBehaviorExecution::onExpressionChange(shared_ptr<Expression> exp)
{
	bool value=exp->evaluateExpression(getHost());
	if (value)
	{
		shared_ptr<Trigger> trigger( new Trigger("") );
		shared_ptr<ChangeEvent> ce( new ChangeEvent("") );
		ce->setChangeExpression(exp);
		trigger->setEvent(ce);
		_eventQueue.push_back(trigger);
		exp->removeCallbackOnMayChange(bind(&StateMachineBehaviorExecution::onExpressionChange,this,exp));
		BehaviorScheduler::getInstance()->reActivate(shared_from_this());
	}
}

void StateMachineBehaviorExecution::onBehaviorStop(BehaviorExecution * /*be*/)
{
	shared_ptr<Vertex> currentState=getCurrentState();
	vector<shared_ptr<Transition> > trans = currentState->getOutgoing();
	currentState->running = false;
        currentActionDone = true;
	for (size_t iT = 0; iT < trans.size(); iT ++)
	{
		vector<shared_ptr<Trigger> > transTriggers = trans[iT]->getTrigger();
		if (transTriggers.size() == 0)
		{
			OclParser::Context context;
                        context["self"]=getHost();
                        if(!trans[iT]->getGuard() || trans[iT]->getGuard()->eval(context))
                        {
                           // Launch transition effect
                           shared_ptr<Action> effect = trans[iT]->getEffect();
                           Parameters param;
                           if (effect)
                           {
                              BehaviorScheduler::getInstance()->executeBehavior(effect,getHost(),param);
                           }
			   activateState(trans[iT]->getTarget(), param);
                        }
		}
	}
}

bool StateMachineBehaviorExecution::activateState(shared_ptr<Vertex> state, const Parameters & param)
{
	_currentState = state;

	getHost()->fireChangeState(shared_dynamic_cast<StateMachineBehaviorExecution>(shared_from_this()));

        shared_ptr<FinalState> fs = shared_dynamic_cast<FinalState>(state);
        if (fs) _toStop = true;
	shared_ptr<Vertex> currentState=getCurrentState();
	_be = currentState->activate(getHost(), param);
	cerr << " Activate State : " << currentState->getName() << endl;
	
	if (_be)
	{
		currentState->running = true;
		_be->addCallbackOnBehaviorStop(bind(&StateMachineBehaviorExecution::onBehaviorStop,this,_1));
                currentActionDone = false;
	}

	vector<shared_ptr<Transition> > trans = currentState->getOutgoing();
	for (size_t iT = 0; iT < trans.size(); iT ++)
	{
		vector<shared_ptr<Trigger> > transTriggers = trans[iT]->getTrigger();
		for (size_t iTr = 0; iTr < transTriggers.size(); iTr ++)
		{
			shared_ptr<Event> event = transTriggers[iTr]->getEvent();
			if (event->getType() == "ChangeEvent")
			{
				shared_ptr<ChangeEvent> ce = shared_dynamic_cast<ChangeEvent>(event);
				shared_ptr<Expression> exp = shared_dynamic_cast<Expression>(ce->getChangeExpression()->clone());
				exp->addCallbackOnMayChange(bind(&StateMachineBehaviorExecution::onExpressionChange,this,exp));
				exp->evaluateExpression(getHost());
			}
/*
			else if (event->getType() == "TimeEvent")
			{
				shared_ptr<TimeEvent> te = shared_dynamic_cast<TimeEvent>(event);
				shared_ptr<LiteralReal> value = shared_dynamic_cast<LiteralReal>(te->getWhen());
				double timeValue = value->getValue();
				Parameters ps;
				shared_ptr<BehaviorExecution> be (new TimeCallbackBehavior(getStateMachine(),getHost(),ps,timeValue,shared_from_this()));
				BehaviorScheduler::getInstance()->_insert(be,timeValue);
			}
*/
		}
	}
	return true;
}

bool StateMachineBehaviorExecution::simulateState(shared_ptr<Vertex> state)
{
//cerr << "Simulate state : " << state->getName();
	_currentState = state;
	state->simulate(getHost());
	return true;
}

void StateMachineBehaviorExecution::addTime(double time)
{
	shared_ptr<Trigger> trigger( new Trigger("time") );
	shared_ptr<TimeEvent> te( new TimeEvent("time") );
	shared_ptr<LiteralReal> value (new LiteralReal(time));
	te->setWhen(value);
	trigger->setEvent(te);
	_eventQueue.push_back(trigger);
	restart();
}

void StateMachineBehaviorExecution::addSignal(shared_ptr<InstanceSpecification> signal)
{
	cerr << "recu signal : " << signal->getName() <<" par " << getHost()->getName()<< endl;
	shared_ptr<Trigger> trigger( new Trigger(signal->getName()) );
	shared_ptr<SignalEvent> se( new SignalEvent(signal->getName()) );
	se->setSignal(signal);
	trigger->setEvent(se);
	_eventQueue.push_back(trigger);
	restart();
}

void StateMachineBehaviorExecution::simulateAddSignal(shared_ptr<InstanceSpecification> signal)
{
//cerr << "Receive Signal : " << signal->getName() << endl;
	shared_ptr<Trigger> trigger( new Trigger(signal->getName()) );
	shared_ptr<SignalEvent> se( new SignalEvent(signal->getName()) );
	se->setSignal(signal);
	trigger->setEvent(se);

	shared_ptr<Vertex> currentState=getCurrentState();
	vector<shared_ptr<Transition> > trans = currentState->getOutgoing();
	for (size_t iT = 0; iT < trans.size(); iT ++)
	{
		vector<shared_ptr<Trigger> > transTriggers = trans[iT]->getTrigger();
		for (size_t i = 0; i < transTriggers.size() ; i++)
		{
			shared_ptr<Trigger> transTrigger = transTriggers[i];
			if (transTrigger->equals(trigger))
			{
				OclParser::Context context;
				context["self"]=getHost();
				if(!trans[iT]->getGuard() || trans[iT]->getGuard()->eval(context))
				{
					// Launch transition effect
					//shared_ptr<Action> effect = trans[iT]->getEffect();
					//if (effect)
					//{
				//		Parameters param;
				//		BehaviorScheduler::getInstance()->executeBehavior(effect,getHost(),param);
				//	}

					simulateState(trans[iT]->getTarget());
				}
			}
		}
	}
}

double StateMachineBehaviorExecution::execute(double dt)
{
	//cerr << " --->  " <<getHost()->getName()<<" stateMachine Behavior " << getSpecification()->getName() << endl;
	if (_first)
	{
		// TODO: Attention a gerer plus tard : possibilite de fixe l'etat en cours a l'initialisation de l'instance
		// Recherche de l'etat Initial
		vector<shared_ptr<PseudoState> > ps = getStateMachine()->getConnectionPoint();
		bool foundInitialState = false;
		for (size_t iPS = 0; iPS < ps.size(); iPS++)
		{
			if (ps[iPS]->getKind() == INITIAL)
			{
				Parameters p;
				vector<shared_ptr<Transition> > trans = ps[iPS]->getOutgoing();
				for (size_t iT = 0; iT < trans.size(); iT ++)
				{
					//if (_currentState) _currentState->desactivate();
					activateState(trans[iT]->getTarget(),p);
				}
				foundInitialState = true;
				break;
			}
		}
		_toStop = foundInitialState ? false : true;
		_first = false;
	}
	else
	{
		while (!_eventQueue.empty())
		{
			shared_ptr<Vertex> currentState=getCurrentState();
			if (!currentState || (currentState->getNonInterupt() && currentState->running)) {break;}

			shared_ptr<Trigger> trigger = _eventQueue.front();
			_eventQueue.pop_front();
			vector<shared_ptr<Transition> > trans = currentState->getOutgoing();
			for (size_t iT = 0; iT < trans.size(); iT ++)
			{
				vector<shared_ptr<Trigger> > transTriggers = trans[iT]->getTrigger();
				for (size_t i = 0; i < transTriggers.size() ; i++)
				{
					shared_ptr<Trigger> transTrigger = transTriggers[i];
					if (!transTrigger)
					{
						cerr << "[StateMachineBehaviorExecution Error] Find transition without Trigger in "
								<< getStateMachine()->getName() << endl;
					}
					if (transTrigger->equals(trigger))
					{
						Parameters param;
						OclParser::Context context;
                                                /* GetParams */
                                                shared_ptr<Event> evt = trigger->getEvent();
                                                if (evt && evt->getType() == "SignalEvent")
                                                {
                                                   shared_ptr<InstanceSpecification> signal = (shared_dynamic_cast<SignalEvent>(evt))->getSignal();
                                                   if (signal)
                                                   {
                                                      map<string, shared_ptr<Slot> > props = signal->getSlots();
                                                      map<string, shared_ptr<Slot> >::iterator it;
                                                      for (it = props.begin(); it != props.end(); it++)
                                                      {
                                                         vector<AnyValue> values;
                                                         values.push_back((AnyValue)it->second->getValue()->getStringFromValue());  
                                                         context[it->first] = CollectionAnyValue(values);
                                                         param[it->first] = it->second->getValue();
                                                         cerr << it->first << " == " << it->second->getValue()->getStringFromValue() << endl;
                                                      }
                                                   }
                                                }
						context["self"]=getHost();
      
						if(!trans[iT]->getGuard() || trans[iT]->getGuard()->eval(context))
						{
							if (currentState)
							{
								if (_be)
								{
									_be->stop();
									BehaviorScheduler::getInstance()->deleteExecutionBehavior(_be);
									_be=shared_ptr<BehaviorExecution>();
								} 
								currentState->desactivate();
							}
							// Launch transition effect
							shared_ptr<Action> effect = trans[iT]->getEffect();
							if (effect)
							{
								BehaviorScheduler::getInstance()->executeBehavior(effect,getHost(),param);
							}

							activateState(trans[iT]->getTarget(), param);
						}
					}
				}
			}
		}
		if(_eventQueue.empty())
			pause();
	}

        if (_toStop) return 0;
	else return 0.001;
}

} // Namespace
