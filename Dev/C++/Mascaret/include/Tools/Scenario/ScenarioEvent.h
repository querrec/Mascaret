#ifndef SCENARIOEVENT_H_
#define SCENARIOEVENT_H_

#include "Tools/veha_plateform.h"
#include "Tools/Scenario/Scenario.h"
#include <string>

using boost::shared_ptr;
using boost::enable_shared_from_this;
using std::string;

namespace VEHA {
    class Action;
    class Entity;
    class Environment;
    class Parameter;
}

namespace BEHAVE {
    class OrganisationalEntity;
    class Procedure;
}

namespace Scenario {

class VEHA_API ScenarioEvent : public enable_shared_from_this<ScenarioEvent>
{
public:
    ScenarioEvent();
    static shared_ptr<ScenarioEvent> createFromNode(shared_ptr<XmlNode> eventNode);
    virtual ~ScenarioEvent();

    virtual void parseXmlEvent(shared_ptr<XmlNode> eventNode);
    virtual void fire();

    inline string getName() {return _name;}
    inline void setName(string name) {_name = name;}

    inline TimeExpression getDate() {return _date;}
    void updateDate(void);
    inline void setDate(TimeExpression date) {_date = date;}

    inline vector< shared_ptr<ScenarioEvent> > getAllEventChildren() {return _children;}
    unsigned int getNbEventChildren();
    vector< shared_ptr<ScenarioEvent> > getEventChildren(TimeExpression date, bool recurrent = false);
    vector< shared_ptr<ScenarioEvent> > getEventChildren(string name, bool recurrent = false);
    void addEventChild(shared_ptr<ScenarioEvent> child);
    void removeEventChild(string name);
    
    inline shared_ptr<ScenarioEvent> getEventParent() {return _parent;}
    inline void setEventParent(shared_ptr<ScenarioEvent> parent) {_parent = parent;}

    virtual string writeEvent();

    string getType() {return _type;}
    void setType(string type) {_type = type;}
protected:
    string _type;
    string _name;
    TimeExpression _date;
    // ArRef<Date> _initDate;
    vector< shared_ptr<ScenarioEvent> > _children;
    shared_ptr<ScenarioEvent> _parent;
};

class ScenarioSignalEvent : public ScenarioEvent
{
public:
    ScenarioSignalEvent() : ScenarioEvent() {_type = "SendSignal";};
    virtual ~ScenarioSignalEvent() {};

    virtual void parseXmlEvent(shared_ptr<XmlNode> eventNode);
    virtual void fire();
protected:
    string _target;
    shared_ptr<VEHA::Action> _action;
    shared_ptr<VEHA::Entity> _entity;
};

class ScenarioChangeViewPointEvent : public ScenarioEvent
{
public:
    ScenarioChangeViewPointEvent() : ScenarioEvent() {_type = "ChangeViewPoint";};
    virtual ~ScenarioChangeViewPointEvent() {};

    virtual void fire();
protected:
    string _viewPointName;
};

class ScenarioCallOperationEvent : public ScenarioEvent
{
public:
    ScenarioCallOperationEvent() : ScenarioEvent() {_type = "CallOperation";};
    virtual ~ScenarioCallOperationEvent() {};

    virtual void parseXmlEvent(shared_ptr<XmlNode> eventNode);
    virtual void fire();
protected:
    string _target;
    string _classifier;
    shared_ptr<VEHA::Action> _action;
    shared_ptr<VEHA::Entity> _entity;
};

class ScenarioLaunchProcedureEvent : public ScenarioEvent
{
public:
    ScenarioLaunchProcedureEvent() : ScenarioEvent() {_type = "LaunchProcedure";};
    virtual ~ScenarioLaunchProcedureEvent() {};
    
    virtual void parseXmlEvent(shared_ptr<XmlNode> eventNode);
    virtual void fire();
protected:
    string _organisation;
    shared_ptr<BEHAVE::OrganisationalEntity> _org;
    shared_ptr<BEHAVE::Procedure> _proc;
    map< shared_ptr<VEHA::Parameter>, string > _params;
};
}

#endif /*SCENARIOEVENT_H_*/
