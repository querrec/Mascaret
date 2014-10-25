#ifndef SCENARIO_H_
#define SCENARIO_H_

#include "Tools/veha_plateform.h"
#include "VEHA/Behavior/Time/TimeExpression.h"

#include <vector>
#include <map>
#include <string>

using boost::shared_ptr;
using std::vector;
using std::map;
using std::string;
using VEHA::TimeExpression;

class XmlNode;

namespace Scenario {

class ScenarioEvent;

class VEHA_API Scenario
{
public:
    Scenario();
    ~Scenario();

    void start();
    void stop();
    bool isStarted() {return _started;}

    string getName() {return _name;}

    void addEvent(shared_ptr<ScenarioEvent> event, bool recursive = true);
    void removeEvent(string name);
    void removeEvent(TimeExpression time);

    inline map< TimeExpression , vector< shared_ptr<ScenarioEvent> > > getAllEvents() {return _events;}
    vector< shared_ptr<ScenarioEvent> > getEvents(TimeExpression time);
    inline void setEvents(map< TimeExpression, vector< shared_ptr<ScenarioEvent> > > events) {_events = events;}
    inline map< TimeExpression , vector< shared_ptr<ScenarioEvent> > >::iterator getItCurrentEvents() { return _itCurrentEvents; }

    void reOrderEvents(void);
    void gotoNextEvents(void);
    void gotoPreviousEvents(void);
    void gotoFirstEvents(void);
    void computeNextEvent(void);
    inline bool hasMoreEvents(void) {return (_itCurrentEvents != _events.end());}

    void parseXmlFile(string fileName);

    string writeScenario();

    inline shared_ptr<TimeExpression> getInitDate(void) {return _initDate;}
    inline void setInitDate(shared_ptr<TimeExpression> date) {_initDate = date;}

    inline TimeExpression getStartDate(void) {return _startDate;}
    inline void setStartDate(TimeExpression startDate) {_startDate = startDate;}
    inline int getStartMode(void) {return _startMode;}

    inline int getMode(void) {return _mode;}
private:
    string _name;
    bool _started;
    shared_ptr<TimeExpression> _initDate;
    TimeExpression _startDate;
    int _mode;
    int _startMode;
    map< TimeExpression , vector< shared_ptr<ScenarioEvent> > > _events;
    map< TimeExpression , vector< shared_ptr<ScenarioEvent> > >::iterator _itCurrentEvents;
};



shared_ptr<TimeExpression> extractTimeExpressionFromNode(shared_ptr<XmlNode> node);

}
#endif /*SCENARIO_H_*/
