#include "Tools/Scenario/ScenarioEvent.h"
#include "Tools/Loaders/XmlParser.h"
#include "VEHA/Behavior/Common/behaviorScheduler.h"

#include "IEHA/VRApplication.h"

#include "BEHAVE/ACLMessage.h"
#include "BEHAVE/Agent.h"
#include "BEHAVE/AgentPlateform.h"
#include "BEHAVE/OrganisationalEntity.h"
#include "BEHAVE/OrganisationalStructure.h"
#include "BEHAVE/Procedure.h"
#include "BEHAVE/RoleAssignement.h"

#include "VEHA/Kernel/Parameter.h"
#include "VEHA/Entity/Entity.h"
#include "VEHA/Behavior/Common/Signal.h"
#include "VEHA/Behavior/Activity/ActivityParameterNode.h"
#include "VEHA/Behavior/BasicActions/SendSignalAction.h"
#include "VEHA/Behavior/BasicActions/CallOperationAction.h"
#include "VEHA/Behavior/BasicActions/ChangeViewPointAction.h"

using namespace VEHA;
using namespace BEHAVE;

using boost::make_shared;
using std::stringstream;

namespace Scenario {
    ScenarioEvent::ScenarioEvent() :
        _type("GenericEvent")
    {
    }

    ScenarioEvent::~ScenarioEvent()
    {
    }

    shared_ptr<ScenarioEvent> ScenarioEvent::createFromNode(shared_ptr<XmlNode> eventNode)
    {
        shared_ptr<ScenarioEvent> event;
        if(eventNode->getName() == "Event")
        {
            string type;
            if(eventNode->getPropertyString("type", type))
            {
            }

            event = make_shared<ScenarioEvent>();
            cerr << "- Find one event";
            if(type != "")
            {
                event->setType(type);
                cerr << " ; type = " << type << endl;
            }
            else
                cerr << endl;
        }
        else if(eventNode->getName() == "SendSignal")
        {
            event = make_shared<ScenarioSignalEvent>();
        }
        else if(eventNode->getName() == "ChangeViewPoint")
        {
            event = make_shared<ScenarioChangeViewPointEvent>();
        }
        else if(eventNode->getName() == "CallOperation")
        {
            event = make_shared<ScenarioCallOperationEvent>();
        }
        else if(eventNode->getName() == "LaunchProcedure")
        {
            event = make_shared<ScenarioLaunchProcedureEvent>();
        }
        if(event)
        {
            cerr << "- Find one event ; " << event->getType() << endl;
        }
        return event;
    }

    void ScenarioEvent::updateDate(void)
    {
        //_date = Timer::getInstance()->getDate(_initDate);
    }

    void ScenarioEvent::parseXmlEvent(shared_ptr<XmlNode> eventNode)
    {
        // name
        string name;
        if(eventNode->getPropertyString("name",name))
        {
            _name = name;
        }

        // date
        if(getEventParent())
        {
            _date = getEventParent()->getDate();
        }

        shared_ptr<XmlNode> dateNode = eventNode->getChildByName("Date");
        cerr << dateNode << endl;
        if(dateNode)
        {
            cerr << " 2- " << dateNode << endl;
            double time;
            if(dateNode->getPropertyReal("time", time))
            {
                _date = TimeExpression(time);
            }
            else
            {
                _date = *(extractTimeExpressionFromNode(dateNode));
            }

            //cerr << " date=" << _date;
        }
        
        shared_ptr<XmlNode> deltaTimeNode = eventNode->getChildByName("DeltaTime");
        cerr << deltaTimeNode << endl;
        if(deltaTimeNode)
        {
            double delta_t;
            if(deltaTimeNode->getPropertyReal("dt", delta_t))
            {
                if(getEventParent())
                    _date = getEventParent()->getDate() + delta_t;
            }
        }

        //eventChildren
        shared_ptr<XmlNode> child = eventNode->getFirstChild();

        while(child)
        {
            shared_ptr<ScenarioEvent> event = createFromNode(child);
            
            if(event)
            {
                addEventChild(event);
                event->parseXmlEvent(child);
            }

            child = child->getNext();
        }
    }

    void ScenarioEvent::fire()
    {
        cerr << "Fire !" << endl;
        string descr = writeEvent();
    }

    string ScenarioEvent::writeEvent(void)
    {
        stringstream buff;
#if 0
        buff += Timer::getInstance()->writeDate(_date - ScenarioScheduler::access()->getScenario()->getStartDate());
#endif

        buff << " | Type = " << getType() << " Name = " << getName() << "\t";
        if(getEventParent()) // != ScenarioEvent::nullRef())
        {
            buff << "ParentName = " << getEventParent()->getName() << "\t";
        }
        buff << _children.size() << " child(ren)";
        /*
           shared_ptr<Date> date;
           unsigned int h,m,s;
           unsigned int y,mo,d;
           string yS;
           date = Timer::getInstance()->getDate(getDate());
           date->getDate(d,mo,y);
           buff +=doubleToStr(d);
           buff +="/";
           buff +=doubleToStr(mo);
           buff +="/";
           yS =doubleToStr(y);
           yS = yS.substr(2, yS.length());
           buff +=yS;
           buff +=" ";
           date->getTime(h,m,s);
           buff +=doubleToStr(h);
           buff +=":";
           buff +=doubleToStr(m);
           buff +=":";
           buff +=doubleToStr(s);
           */
        return buff.str();
    }

    vector< shared_ptr<ScenarioEvent> > ScenarioEvent::getEventChildren(string name, bool recurrent)
    {
        vector< shared_ptr<ScenarioEvent> > events;
        for(unsigned int i = 0; i < _children.size(); i++)
        {
            if(_children[i]->getName() == name)
                events.push_back(_children[i]);
            else if(recurrent && _children[i]->getNbEventChildren())
            {
                vector< shared_ptr<ScenarioEvent> > eventChildren = _children[i]->getEventChildren(name, true);
                events.insert(events.end(), eventChildren.begin(), eventChildren.end());
            }
        }
        return events;
    }

    vector< shared_ptr<ScenarioEvent> > ScenarioEvent::getEventChildren(TimeExpression date, bool recurrent)
    {
        vector< shared_ptr<ScenarioEvent> > events;
        for(unsigned int i = 0; i < _children.size(); i++)
        {
            if(_children[i]->getDate() == date)
            {
                events.push_back(_children[i]);
            }
            else if(recurrent && _children[i]->getNbEventChildren())
            {
                vector< shared_ptr<ScenarioEvent> > eventChildren = _children[i]->getEventChildren(date, true);
                events.insert(events.end(), eventChildren.begin(), eventChildren.end());
            }
        }
        return events;
    }

    unsigned int ScenarioEvent::getNbEventChildren(void)
    {
        return _children.size();
    }

    void ScenarioEvent::addEventChild(shared_ptr<ScenarioEvent> eventChild)
    {
        eventChild->setEventParent(shared_from_this());
        _children.push_back(eventChild);
    }

    void ScenarioEvent::removeEventChild(string name)
    {
        vector< shared_ptr<ScenarioEvent> >::iterator it = _children.begin();
        while(it != _children.end())
        {
            if((*it)->getName() == name)
                *it = shared_ptr<ScenarioEvent>();
            it++;
        }
        _children.erase(remove(_children.begin(), _children.end(), shared_ptr<ScenarioEvent>()), _children.end());
    }

void ScenarioSignalEvent::parseXmlEvent(shared_ptr<XmlNode> eventNode)
{
    ScenarioEvent::parseXmlEvent(eventNode);
    
    string target;
    if(eventNode->getPropertyString("target", target))
    {
        _target = target;
    }

    //cerr << " signal=" << _signalName << " entity=" << _entityName << endl;
  
    // TODO: Verification de presence et allocation à deplacer 
    shared_ptr<Entity> entity = MascaretApplication::getInstance()->getEnvironment()->getEntity(_target);
    _entity = entity;

    shared_ptr<SendSignalAction> action = make_shared<SendSignalAction>();
    action->setSignalClass(boost::make_shared<Signal>(getName()));
    action->setTarget(_entity);
    _action = action;
}

void ScenarioSignalEvent::fire()
{
    ScenarioEvent::fire();
        
    if(_action)
    {
        BehaviorScheduler::getInstance()->executeBehavior(_action, _entity, Parameters(), true);
    }
}

void ScenarioChangeViewPointEvent::fire()
{
    ScenarioEvent::fire();

    cerr << "ChangeViewPoint" << endl;

    vector< shared_ptr<IEHA::Window3D> > windows = IEHA::VRApplication::getInstance()->getWindows();
   
    for(size_t i = 0; i < windows.size(); i++)
    {
        shared_ptr<ChangeViewPointAction> action = make_shared<ChangeViewPointAction>();
        action->setCamera(windows[i]->getCamera());
        action->setViewPointName(getName());

        if(action)
        {
            shared_ptr<InstanceSpecification> instance;
            BehaviorScheduler::getInstance()->executeBehavior(action, instance, Parameters());
        }
    }
}

void ScenarioCallOperationEvent::parseXmlEvent(shared_ptr<XmlNode> eventNode)
{
    ScenarioEvent::parseXmlEvent(eventNode);

    string classifier;
    if(eventNode->getPropertyString("classifier", classifier))
    {
        _classifier = classifier;
    }
    
    string target;
    if(eventNode->getPropertyString("target", target))
    {
        _target = target;
    }
}

void ScenarioCallOperationEvent::fire()
{
    ScenarioEvent::fire();

    shared_ptr<Class> targetClass=MascaretApplication::getInstance()->getModel()->getClassByFullname(_classifier);
    shared_ptr<Entity> entity = MascaretApplication::getInstance()->getEnvironment()->getEntity(_target);
    shared_ptr<CallOperationAction> action = make_shared<CallOperationAction>();
    action->setOperation(targetClass->getOperations()[getName()]);

    _action = action;
    _entity = entity;    

    if(_action)
    {
        BehaviorScheduler::getInstance()->executeBehavior(action, entity, Parameters());
    }
}

void ScenarioLaunchProcedureEvent::parseXmlEvent(shared_ptr<XmlNode> eventNode)
{
    ScenarioEvent::parseXmlEvent(eventNode);

    string organisation;
    if(eventNode->getPropertyString("organisation", organisation))
    {
        _organisation = organisation;
    }

    vector <shared_ptr<OrganisationalEntity> > orgs = MascaretApplication::getInstance()->getAgentPlateform()->getOrganisationalEntity();
    bool found = false;
    int i = 0;
    while (!found && (unsigned)i < orgs.size())
    {
        if (orgs[i]->getName() == _organisation)
            found = true;
        else i++;
    }
    if (found) _org = orgs[i];
    else
    {
        cerr << "Erreur, Organisation " << _organisation << "non trouvé..." << endl;
        return;
    }

    shared_ptr<OrganisationalStructure> struc = _org->getStructure();
    vector< shared_ptr<Procedure> > procs = struc->getProcedures();
    found = false;
    i= 0;
    while (!found && (unsigned)i < procs.size())
    {
        if (procs[i]->getName() == getName())
            found = true;
        else i++;
    }
    if (found) _proc = procs[i];
    else
    {
        cerr << "Erreur, Procedure " << getName() << "non trouvé..." << endl;
        return;
    }

    vector<shared_ptr<Parameter> > parameters;
    vector< shared_ptr<ActivityNode> > nodes  = _proc->getActivity()->getNode();
    for (unsigned int iNode = 0; iNode < nodes.size(); iNode ++)
    {
        if (nodes[iNode]->getKind() == "parameter")
        {
            parameters.push_back((shared_dynamic_cast<ActivityParameterNode>(nodes[iNode]))->getParameter());
            shared_ptr<Parameter> param = (shared_dynamic_cast<ActivityParameterNode>(nodes[iNode]))->getParameter();
 
            string param_value;
            if(eventNode->getPropertyString(param->getName(), param_value))
            {
                _params[param] = param_value;                
            }
        }
    }

}

void ScenarioLaunchProcedureEvent::fire()
{
    ScenarioEvent::fire();

    string paramString = "";
    
    map< shared_ptr<VEHA::Parameter>, string >::iterator _params_it = _params.begin();
    while(_params_it != _params.end())
    {
        paramString += " :" + _params_it->first->getName() + " " + _params_it->second;
        _params_it++;
    }

    vector< shared_ptr<RoleAssignement> > assignements = _org->getRoleAssignements();
    for (size_t iA = 0; iA < assignements.size() ; iA++)
    {
        shared_ptr<ACLMessage> procMsg = make_shared<ACLMessage>(REQUEST);
        procMsg->addReceiver(assignements[iA]->getAgent());
        cout<<"SENDING MESSAGE TO "<<assignements[iA]->getAgent()->getName()<<endl;
        shared_ptr<AID> agentAID=MascaretApplication::getInstance()->agent->getAID();
        procMsg->setSender(agentAID);
        string content ="((action ";
        
        content += assignements[iA]->getAgent()->getName();
        content += " ";
        content += "(" + _proc->getName();
        content += paramString;
        content += ")))";
        cout << content << endl;
        procMsg->setContent(content);
        MascaretApplication::getInstance()->getAgentPlateform()->sendMessage(procMsg);
    }
}

}
