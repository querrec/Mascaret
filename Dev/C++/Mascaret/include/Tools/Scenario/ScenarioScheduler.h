#ifndef SCENARIOSCHEDULER_H_
#define SCENARIOSCHEDULER_H_

#include "Tools/veha_plateform.h"
#include <string>
#include <vector>

using boost::shared_ptr;
using boost::make_shared;
using std::string;
using std::vector;

namespace Scenario {

class Scenario;

class VEHA_API ScenarioScheduler
{
public:
    static inline shared_ptr<ScenarioScheduler> getInstance();
    static void destroyInstance();
    virtual ~ScenarioScheduler();
protected:
    ScenarioScheduler();
    static shared_ptr<ScenarioScheduler> _instance;
public:
    void parseScenarioFile(string scenarioFileName);
    void reset();
    void activate();
    vector< shared_ptr<Scenario> > getScenarios() {return _scenarios;};
    shared_ptr<Scenario> getScenario() {return _current_scenario;};
    int getNbScenario() {return _scenarios.size();}
    void setScenario(int id);
    void setScenario(string& name);
protected:
    vector< shared_ptr<Scenario> > _scenarios;
    shared_ptr<Scenario> _current_scenario;
};

inline shared_ptr<ScenarioScheduler> ScenarioScheduler::getInstance()
{
    if (!_instance)
        _instance = shared_ptr<ScenarioScheduler>(new ScenarioScheduler());
    return _instance;
}

}

#endif /*SCENARIOSCHEDULER_H_*/
