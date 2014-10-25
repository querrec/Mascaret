#include "Tools/Scenario/ScenarioScheduler.h"
#include "Tools/Scenario/Scenario.h"
#include "Tools/Scenario/ScenarioEvent.h"
#include "VEHA/Behavior/Common/behaviorScheduler.h"

#include <iostream>

using boost::make_shared;

namespace Scenario {

shared_ptr<ScenarioScheduler> ScenarioScheduler::_instance = shared_ptr<ScenarioScheduler>();

ScenarioScheduler::ScenarioScheduler()
{
}

ScenarioScheduler::~ScenarioScheduler()
{
}

void ScenarioScheduler::reset()
{
	_current_scenario->stop();
	_current_scenario = shared_ptr<Scenario>();
}

void ScenarioScheduler::parseScenarioFile(string scenarioFileName)
{
    shared_ptr<Scenario> scenario = make_shared<Scenario>();
    scenario->parseXmlFile(scenarioFileName);
    _current_scenario = scenario;
    _scenarios.push_back(scenario);
    std::cerr << "Finished parse Scenario " << scenario->getName() << std::endl;
}

void ScenarioScheduler::setScenario(int id)
{
    _current_scenario = _scenarios[id];
}

void ScenarioScheduler::setScenario(string& name)
{
    for(size_t i = 0; i < _scenarios.size(); i++)
    {
        if(_scenarios[i]->getName() == name)
        {
            _current_scenario = _scenarios[i];
            return;
        }
    }
}

void ScenarioScheduler::activate()
{
    if (!_current_scenario)
        return;
    
    if (!_current_scenario->hasMoreEvents())
        return;

    if (!_current_scenario->isStarted())
    {
        _current_scenario->start();
    }

    map< TimeExpression , vector< shared_ptr<ScenarioEvent> > >::iterator itCurrentEvents = _current_scenario->getItCurrentEvents();

    if (itCurrentEvents->first <= VEHA::BehaviorScheduler::getInstance()->getCurrentVirtualTime())
    {
        //std::cerr << " " << __LINE__ << " > " << VEHA::BehaviorScheduler::getInstance()->getCurrentVirtualTime().getAsString() << " " << itCurrentEvents->first.getAsString() << std::endl;
        
        vector< shared_ptr<ScenarioEvent> > currentEvents = itCurrentEvents->second;
        vector< shared_ptr<ScenarioEvent> >::iterator it = currentEvents.begin();
        while( it != currentEvents.end())
        {
            (*it)->fire();
            it++;
        }
        
        _current_scenario->gotoNextEvents(); 
    }
    else
    {
        //std::cerr << " " << __LINE__ << " < " << VEHA::BehaviorScheduler::getInstance()->getCurrentVirtualTime().getAsString() << " " << itCurrentEvents->first.getAsString() << std::endl;
    }

#if 0

        std::cerr << " " << __LINE__ << std::endl;
    bool _reinit = false;

    if (_scenario->hasMoreEvents() && itCurrentEvents->first >= _scenario->getStartDate())
    {
        std::cerr << " " << __LINE__ << std::endl;
        std::cerr << " " << __LINE__ << itCurrentEvents->first.getAsDouble(); 
//        std::cerr << " " << __LINE__ << itCurrentEvents->second;
        vector< shared_ptr<ScenarioEvent> > currentEvents = itCurrentEvents->second;
        std::cerr << " " << __LINE__ << std::endl;
        vector< shared_ptr<ScenarioEvent> >::iterator it = currentEvents.begin();
        std::cerr << " " << __LINE__ << std::endl;
        
        while( it != currentEvents.end())
        {
            (*it)->fire();
            if (_reinit) break;
            it++;
        }
        if (!_reinit)_scenario->gotoNextEvents();
        else _reinit = false;
    }
    else if (_scenario->getStartMode() == 2) // execute all events before startDate now
    {
        while (itCurrentEvents->first <= _scenario->getStartDate())
        {
            vector< shared_ptr<ScenarioEvent> > currentEvents = itCurrentEvents->second;
            vector< shared_ptr<ScenarioEvent> >::iterator it = currentEvents.begin();
            
            while( it != currentEvents.end())
            {
                (*it)->fire();
                it++;
            }
            _scenario->gotoNextEvents();
            itCurrentEvents = _scenario->getItCurrentEvents();
        }
    }
    else
    {
        std::cerr << "Skipping events at : " << itCurrentEvents->first.getAsDouble() << std::endl;
        _scenario->gotoNextEvents();
    }

#if 0    
    if (_scenario->hasMoreEvents())
    {       
        Timer::getInstance()->wakeMeUpAt(thisRef(), _scenario->getItCurrentEvents()->first - _scenario->getStartDate());
    }
    else if (_scenario->getMode() == 1) // restart the scenario
    {
        reset();
    }
    
    _act->setSuspended(true);
#endif
#endif
}

}
