#include <boost/foreach.hpp>
#include "Tools/Network/Servlets/ManageEntityJSONServlet.h"
#include "VEHA/Kernel/InstanceSpecification.h"
#include "VEHA/Kernel/Environment.h"
#include "VEHA/Entity/Entity.h"
#include "VEHA/Entity/Vector3.h"
#include "VEHA/Behavior/Common/Behavior.h"
#include "VEHA/Behavior/Common/Event.h"
#include "VEHA/Behavior/Common/Signal.h"
#include "VEHA/Behavior/Common/Trigger.h"
#include "VEHA/Behavior/StateMachine/SignalEvent.h"
#include "VEHA/Behavior/StateMachine/StateMachine.h"
#include "VEHA/Behavior/StateMachine/Transition.h"
#include "VEHA/Behavior/StateMachine/Region.h"
#include "MascaretApplication.h"
#include "HAVE/VirtualHuman.h"

using namespace VEHA;
using namespace HAVE;

extern string _ftoa(double val);

void ManageEntityJSONServlet::manageRequest(HttpRequest* req)
{
  req->getResponse()->setContentType("text/json");
  string id = req->getParameter("agent");

  shared_ptr<Environment> env = MascaretApplication::getInstance()->getEnvironment();
  shared_ptr<InstanceSpecification> entity;
  try
  {
#if GCC_VERSION > 40100
    entity=env->getInstances().at(id);
#else
    entity = MAP_AT(env->getInstances(), id);
#endif
  }
  catch (std::out_of_range&)
  {
  }
  shared_ptr<VirtualHuman> human;
  if (entity) human = shared_dynamic_cast < VirtualHuman > (entity);
  if (human) env = human->getKnowledgeBase()->getEnvironment();

  id = req->getParameter("alias");

  try
  {
#if GCC_VERSION > 40100
    entity=env->getInstances().at(id);
#else
    entity = MAP_AT(env->getInstances(), id);
#endif
  }
  catch (std::out_of_range&)
  {
    cerr << "Can't find : " << id << endl;
    req->getResponse()->write("{error: \"Can't find entity" + id + "\"}");
    return;
  }
  //   vector<InstanceSpecification*> ents = env->getInstances();
  //   InstanceSpecification * entity = (InstanceSpecification*)NULL;
  //   for (unsigned int i = 0; i < ents.size(); i++)
  //   {
  //      if (ents[i]->getId() == id)
  //      {
  //         entity = ents[i];
  //         break;
  //      }
  //   }

  req->getResponse()->write("{");
  req->getResponse()->write("\"name\":\"" + entity->getName() + "\",");
  req->getResponse()->write("\"fullname\":\"" + entity->getFullName() + "\",");
  req->getResponse()->write("\"description\":\"" + entity->getDescription() + "\",");
  req->getResponse()->write("\"class\":\"" + entity->getClassifier()->getName() + "\",");
  req->getResponse()->flushBuffer();

  Vector3 position, orientation;
  shared_ptr<Entity> entity2 = shared_dynamic_cast < Entity > (entity);
  if (entity2.get() != NULL)
  {
    position = entity2->getLocalPosition();
    orientation = entity2->getLocalOrientation();

    req->getResponse()->write("\"geometry\": {");

    req->getResponse()->write("\"x\":" + _ftoa(position.x) + ",");
    req->getResponse()->write("\"y\":" + _ftoa(position.y) + ",");
    req->getResponse()->write("\"z\":" + _ftoa(position.z) + ",");
    req->getResponse()->write("\"rx\":" + _ftoa(orientation.x) + ",");
    req->getResponse()->write("\"ry\":" + _ftoa(orientation.y) + ",");
    req->getResponse()->write("\"rz\":" + _ftoa(orientation.z));
    req->getResponse()->write("},");
    req->getResponse()->flushBuffer();
  }

  req->getResponse()->write("\"properties\": {");
  const map<string, shared_ptr<Slot> >& attributes = entity->getSlots();
  map<string, shared_ptr<Slot> >::const_iterator it;
  bool first = true;
  for (it = attributes.begin(); it != attributes.end(); it++)
  {
    if (!first)
    {
      req->getResponse()->write(",");
    }
    else
    {
      first = false;
    }
    req->getResponse()->write("\"" + it->first + "\":[");
    if (it->second->getValues().size())
    {
      string values;
      pair < string, shared_ptr<ValueSpecification> > pV;
      BOOST_FOREACH(pV, it->second->getValues())
      {
        values += "\"" + pV.first + "\",";
      }
      req->getResponse()->write(values.substr(0, values.length() - 1));
    }
    req->getResponse()->write("]");
  }
  req->getResponse()->write("}");
  req->getResponse()->flushBuffer();

//
//
//  req->getResponse()->write("</ul>");
//  req->getResponse()->write("<INPUT TYPE=\"submit\" VALUE=\"Modifier\">");
//  req->getResponse()->write("</FORM>");
//  req->getResponse()->flushBuffer();
//
//  req->getResponse()->write("<HR>");
//  req->getResponse()->write("<H2>Operations</H2>");
//  req->getResponse()->write("<ul>");
//  shared_ptr<Class> classifier = entity->getClassifier();
//  const map<string, shared_ptr<Operation> >& operations = classifier->getOperations();
//  map<string, shared_ptr<Operation> >::const_iterator ito;
//  for (ito = operations.begin(); ito != operations.end(); ito++)
//  {
//    req->getResponse()->write("<li>");
//    req->getResponse()->write(" <a href=\"Operation?alias=");
//    req->getResponse()->write(entity->getName());
//    req->getResponse()->write("&oper=");
//    req->getResponse()->write(ito->first);
//    req->getResponse()->write("\" target = \"Body\">");
//    req->getResponse()->write(ito->first);
//    req->getResponse()->write("</a>");
//    req->getResponse()->write("</li>");
//  }
//  req->getResponse()->write("</ul>");
//  req->getResponse()->flushBuffer();
//
//  req->getResponse()->write("<HR>");
//  req->getResponse()->write("<H2>Signaux</H2>");
//  req->getResponse()->write("<ul>");
//  Classifier::OwnedBehaviorMap behaviors = classifier->getOwnedBehavior();
//  map<string, shared_ptr<Behavior> >::iterator itB;
//  for (itB = behaviors.begin(); itB != behaviors.end(); itB++)
//  {
//    shared_ptr<StateMachine> stateMachine = shared_dynamic_cast < StateMachine > (itB->second);
//    shared_ptr<Region> region = stateMachine->getRegion_at(0);
//    if (region)
//    {
//      vector < shared_ptr<Transition> > transitions = region->getTransition();
//      for (unsigned int iTrans = 0; iTrans < transitions.size(); iTrans++)
//      {
//        vector < shared_ptr<Trigger> > triggers = transitions[iTrans]->getTrigger();
//        for (unsigned int iTrig = 0; iTrig < triggers.size(); iTrig++)
//        {
//          shared_ptr<Event> event = triggers[iTrig]->getEvent();
//          if (event)
//          {
//            if (event->getType() == "SignalEvent")
//            {
//              shared_ptr<SignalEvent> signalEvent = shared_dynamic_cast < SignalEvent > (event);
//
//              req->getResponse()->write("<li>");
//              req->getResponse()->write(" <a href=\"Signal?alias=");
//              req->getResponse()->write(entity->getName());
//              req->getResponse()->write("&signal=");
//              req->getResponse()->write(
//                  (shared_dynamic_cast < SignalEvent > (event))->getSignalClass()->getName());
//              req->getResponse()->write("\" target = \"Body\">");
//              req->getResponse()->write(
//                  (shared_dynamic_cast < SignalEvent > (event))->getSignalClass()->getName());
//              req->getResponse()->write("</a>");
//              req->getResponse()->write("</li>");
//            }
//          }
//        }
//      }
//    }
//
//  }

  req->getResponse()->write("}");
}

extern "C" HttpServlet* createManageEntityJSONServlet()
{
  return new ManageEntityJSONServlet();
}
extern "C" void destroyManageEntityJSONServlet(HttpServlet* s)
{
  delete s;
}

