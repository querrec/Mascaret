#include "Tools/Scenario/Scenario.h"
#include "VEHA/Behavior/Time/TimeExpression.h"
#include "Tools/Loaders/XmlParser.h"
#include "VEHA/Behavior/Common/behaviorScheduler.h"

#include "Tools/Scenario/ScenarioEvent.h"

#include "IEHA/VRApplication.h"

#include "VEHA/Entity/Entity.h"
#include "VEHA/Behavior/BasicActions/Action.h"

#include <vector>
#include <map>
#include <string>

using boost::make_shared;
using std::vector;
using std::map;
using std::string;
using std::stringstream;

using namespace VEHA;

namespace Scenario
{
    shared_ptr<TimeExpression> extractTimeExpressionFromNode(shared_ptr<XmlNode> node)
    {
        int year, month, day, hour, minute, second;
        node->getPropertyInteger("year", year);
        node->getPropertyInteger("month", month);
        node->getPropertyInteger("day", day);
        node->getPropertyInteger("hour", hour);
        node->getPropertyInteger("minute", minute);
        node->getPropertyInteger("second", second);
        shared_ptr<TimeExpression> date = make_shared<TimeExpression>(year, month, day, hour, minute, second);
        cerr << "Date parsed = " << day << "/" << month << "/" << year << " " << hour << ":" << minute << ":" << second << endl;
        return date;
    }

    Scenario::Scenario() :
        _started(false)
    {
    }

    Scenario::~Scenario()
    {
    }

    void Scenario::start()
    {
        _started = true;
        gotoFirstEvents();
        BehaviorScheduler::getInstance()->forceCurrentVirtualTime(*_initDate);
    }

    void Scenario::stop()
    {
        _started = false;
    }

    vector< shared_ptr<ScenarioEvent> > Scenario::getEvents(TimeExpression time)
    {
        map< TimeExpression, vector< shared_ptr<ScenarioEvent> > >::iterator it = _events.find(time);

        if(it == _events.end())
            return vector< shared_ptr<ScenarioEvent> >();
        else
            return it->second;
    }

    void Scenario::gotoNextEvents()
    {
        if(_itCurrentEvents != _events.end())
            _itCurrentEvents++;
    }

    void Scenario::gotoPreviousEvents()
    {
        if(_itCurrentEvents != _events.begin())
            _itCurrentEvents--;
    }

    void Scenario::gotoFirstEvents()
    {
        _itCurrentEvents = _events.begin();
    }

    void Scenario::computeNextEvent(void)
    {
        _itCurrentEvents = _events.begin();
        TimeExpression currentTime = BehaviorScheduler::getInstance()->getCurrentVirtualTime();

        bool found = false;
        while(!found)
        {
            if(_itCurrentEvents == _events.end())
            {
                found = true;
            }

            if(_itCurrentEvents->first > currentTime)
            {
                found = true;
            }
            else
            {
                _itCurrentEvents++;
            }
        }
    }

    void Scenario::addEvent(shared_ptr<ScenarioEvent> event, bool recursive)
    {
        map< TimeExpression, vector< shared_ptr<ScenarioEvent> > >::iterator it;
        it = _events.find(event->getDate());

        if(it == _events.end())  // no key for this date
        {
            vector< shared_ptr<ScenarioEvent> > events;
            events.push_back(event);
            _events.insert(make_pair(event->getDate(), events));
        }
        else
        {
            it->second.push_back(event);
        }

        if(recursive)
        {
            for(unsigned int i = 0; i < event->getAllEventChildren().size(); i++)
            {
                addEvent(event->getAllEventChildren()[i]);
            }
        }
    }

    void Scenario::removeEvent(string name)
    {
        map< TimeExpression, vector< shared_ptr<ScenarioEvent> > >::iterator itTime = _events.begin();
        vector<TimeExpression> v;

        // Search events by name and store keys in vector v
        while(itTime != _events.end())
        {
            vector< shared_ptr<ScenarioEvent> > eventsTime = itTime->second;
            vector< shared_ptr<ScenarioEvent> >::iterator itEventsTime = eventsTime.begin();
            while(itEventsTime != eventsTime.end())
            {
                if((*itEventsTime)->getName() == name)
                {
                    // remove event from scenario -> nullRef
                    //      		*itEventsTime = ScenarioEvent::nullRef();
                    *itEventsTime = shared_ptr<ScenarioEvent>();
                }
                itEventsTime++;
            }
            //eventsTime.erase(remove(eventsTime.begin(), eventsTime.end(), ScenarioEvent::nullRef()), eventsTime.end());
            eventsTime.erase(remove(eventsTime.begin(), eventsTime.end(), shared_ptr<ScenarioEvent>()), eventsTime.end());
            if(eventsTime.size() == 0)
                v.push_back(itTime->first);
            itTime++;
        }

        // Deletes events
        for(unsigned int i = 0; i < v.size(); i++)
        {
            _events.erase(v[i]);
        }
    }

    void Scenario::removeEvent(TimeExpression time)
    {
        _events.erase(time);
    }

    void Scenario::parseXmlFile(string fileName)
    {
        cerr << "Start parsing scenario: " << fileName << endl;
        shared_ptr<XmlParser> parser = make_shared<XmlParser>();

        if(!parser->parseFile(fileName))
        {
            cerr << "Error while loading scenario XML file : " << fileName << endl;
        }
        else
        {
            shared_ptr<XmlNode> root = parser->getRoot();

            string name;
            if(root->getPropertyString("name", name))
            {
                _name = name;
            }

            string mode;
            if(root->getPropertyString("mode", mode))
                _mode = mode == "cyclic" ? 1 : 0;
            else
                _mode = 0;

            /* init date */
            shared_ptr<XmlNode> initDateNode = root->getChildByName("InitDate");
            if(initDateNode)
            {
                shared_ptr<TimeExpression> initDate = extractTimeExpressionFromNode(initDateNode);
                //BehaviorScheduler::getInstance()->forceCurrentVirtualTime(*initDate);
                _initDate = initDate;
            }
            else
            {
                shared_ptr<TimeExpression> initDate = make_shared<TimeExpression>();
                _initDate = initDate;
            }

            // TODO: Start Date Init Date, vraiment utile ?
            /* start date */
            // timer is set to initDate and scheduler is set to startDate
            shared_ptr<XmlNode> startDateNode = root->getChildByName("StartDate");
            if(startDateNode)
            {
                cerr << "Start";
                shared_ptr<TimeExpression> startDate = extractTimeExpressionFromNode(startDateNode);

                string startMode;
                if(startDateNode->getPropertyString("mode", startMode))
                {
                    if(startMode == "goto")
                        _startMode = 0;
                    else if(startMode == "immediate")
                    {
                        _startMode = 1;
                        // TODO
                        // Timer::getInstance()->setInitialDate(year, month, day, hour, minute, second);
                    }
                    else if(startMode == "execute")
                    {
                        _startMode = 2;
                        // TODO
                        // Timer::getInstance()->setInitialDate(year, month, day, hour, minute, second);
                    }
                }
                else
                    _startMode = 0;

                #if 0
                shared_ptr<Date> startDate = Date::NEW();
                startDate->setDate(day, month, year);
                startDate->setTime(hour, minute, second);
                _startDate = Timer::getInstance()->getDate(startDate);
                #else
                _startDate = *startDate;
                #endif

                cerr << "\tMode = " << startMode << endl;
            }
            else
            {
                #if 0
                _startDate = Timer::getInstance()->getDate(_initDate);
                #else
                // TODO
                #endif
            }

            /* events */
            shared_ptr<XmlNode> child = root->getFirstChild();
            while(child)
            {
                shared_ptr<ScenarioEvent> event = ScenarioEvent::createFromNode(child);
                
                if(event)
                {
                    event->setEventParent(shared_ptr<ScenarioEvent>());
                    event->parseXmlEvent(child);
                    addEvent(event);
                }
                
                child = child->getNext();
            }
            _itCurrentEvents = _events.begin();
        }

        cerr << "Parsing scenario is finished" << endl;
    }

    void Scenario::reOrderEvents(void)
    {
        map< TimeExpression , vector< shared_ptr<ScenarioEvent> > > allEvents = _events;
        _events.clear();

        map< TimeExpression , vector< shared_ptr<ScenarioEvent> > >::iterator it = allEvents.begin();
        while(it != allEvents.end())
        {
            vector<shared_ptr<ScenarioEvent> > timeEvents = it->second;
            for(unsigned int i = 0; i < timeEvents.size() ; i++)
            {
                timeEvents[i]->updateDate();
                addEvent(timeEvents[i],false);
            }
            it++;
        }

        _itCurrentEvents = _events.begin();
        while((_itCurrentEvents!=_events.end()) && (_itCurrentEvents->first < TimeExpression(0))) _itCurrentEvents++;
    }

    string Scenario::writeScenario()
    {
        stringstream scenarioBuffer;

        scenarioBuffer << "Scenario " << getName() << " begins at " << _initDate->getAsString() << endl;


        map< TimeExpression, vector< shared_ptr<ScenarioEvent> > >::iterator it = _events.begin();
        while( it != _events.end())
        {

            scenarioBuffer << "Time = " << it->first.getAsString() << "\t";

            vector< shared_ptr<ScenarioEvent> > timeEvents = it->second;
            scenarioBuffer << timeEvents.size();
            scenarioBuffer << " event(s)\n";
            for(unsigned int i = 0; i < timeEvents.size(); i++)
            {
                scenarioBuffer << timeEvents[i]->writeEvent() << endl;
            }
            it++;
        }

        scenarioBuffer << "Scenario ends";

        return scenarioBuffer.str();
    }
}
