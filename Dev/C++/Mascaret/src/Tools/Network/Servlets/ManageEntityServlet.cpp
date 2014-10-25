#include <boost/foreach.hpp>
#include <boost/algorithm/string/replace.hpp>
#include "Tools/Network/Servlets/ManageEntityServlet.h"
#include "VEHA/Kernel/InstanceSpecification.h"
#include "VEHA/Kernel/Environment.h"
#include "VEHA/Entity/Entity.h"
#include "VEHA/Entity/Vector3.h"
#include "VEHA/Behavior/Common/Behavior.h"
#include "VEHA/Behavior/Common/Event.h"
#include "VEHA/Behavior/Common/Signal.h"
#include "VEHA/Behavior/Common/Trigger.h"
#include "VEHA/Behavior/StateMachine/StateMachineBehaviorExecution.h"
#include "VEHA/Behavior/StateMachine/Vertex.h"
#include "VEHA/Behavior/StateMachine/SignalEvent.h"
#include "VEHA/Behavior/StateMachine/StateMachine.h"
#include "VEHA/Behavior/StateMachine/Transition.h"
#include "VEHA/Behavior/StateMachine/Region.h"
#include "MascaretApplication.h"
#include "HAVE/VirtualHuman.h"


using namespace VEHA;
using namespace HAVE;

string _ftoa(double val)
{
      char tmp[256];
      sprintf(tmp,"%f",val);
      string sValue(tmp);
      return sValue;
}
void ManageEntityServlet::manageRequest(HttpRequest* req)
{
	string id = req->getParameter("agent");

        shared_ptr<Environment> env = MascaretApplication::getInstance()->getEnvironment();
        shared_ptr<InstanceSpecification> entity;
        try
        {
#if GCC_VERSION > 40100
                entity=env->getInstances().at(id);
#else
                entity=MAP_AT(env->getInstances(),id);
#endif
        }
        catch (std::out_of_range&)
        {
        }
        shared_ptr<VirtualHuman> human;
        if (entity) human = shared_dynamic_cast<VirtualHuman>(entity);
        if (human) env = human->getKnowledgeBase()->getEnvironment();

	id = req->getParameter("alias");

	try
	{
#if GCC_VERSION > 40100
		entity=env->getInstances().at(id);
#else
		entity=MAP_AT(env->getInstances(),id);
#endif
	}
	catch (std::out_of_range&)
	{
		cerr <<"Can't find : "<<id<<endl;
		req->getResponse()->write("<html>");
		req->getResponse()->write("<body>");
		req->getResponse()->write("Can't find entity: " + id);
		req->getResponse()->write("</body>");
		req->getResponse()->write("</html>");
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

	req->getResponse()->write("<html>");
	req->getResponse()->write("<body>");
	req->getResponse()->write("<META HTTP-EQUIV=\"Refresh\" CONTENT=\"30\">");
	req->getResponse()->write("<META HTTP-EQUIV=\"Content-Type\" content=\"text/html; charset=UTF-8\">");
	req->getResponse()->write("<link href=\"doxygen.css\" rel=\"stylesheet\" type=\"text/css\">");
	req->getResponse()->write("<link href=\"tabs.css\" rel=\"stylesheet\" type=\"text/css\">");

	req->getResponse()->write("<H2>Description</H2>");
	req->getResponse()->write("<ul>");
	req->getResponse()->write("<li>");
	req->getResponse()->write(entity->getName());
	req->getResponse()->write("</li>");
	req->getResponse()->write("<li>");
	req->getResponse()->write(entity->getFullName());
	req->getResponse()->write("</li>");
	req->getResponse()->write("<li>");
	req->getResponse()->write(entity->getDescription());
	req->getResponse()->write("</li>");
	req->getResponse()->write("<li>");
	req->getResponse()->write(" <a href=\"Class?alias=");
	req->getResponse()->write(entity->getClassifier()->getName());
	req->getResponse()->write("\" target = \"Body\">");
	req->getResponse()->write("</a>");
	req->getResponse()->write("</li>");
	req->getResponse()->write("</ul>");
	req->getResponse()->flushBuffer();

	Vector3 position, orientation ;

	shared_ptr<Entity> entity2=shared_dynamic_cast<Entity>(entity);
	if(entity2.get() != NULL)
	{
		position = entity2->getLocalPosition();
		orientation = entity2->getLocalOrientation();

		req->getResponse()->write("<HR>");
		req->getResponse()->write("<H2>Geometrie</H2>");



		req->getResponse()->write("<FORM METHOD=GET action=\"changeGeometry\">");
		req->getResponse()->write("<input type=\"hidden\" name=\"alias\" value=\"");
		req->getResponse()->write(id);
		req->getResponse()->write("\" />");
		req->getResponse()->write("<TABLE BORDER=1>");
		req->getResponse()->write("<TR>");
		req->getResponse()->write("<TD>Position locale</TD>");
		req->getResponse()->write("<TD>");
		req->getResponse()->write(_ftoa(position.x));
		req->getResponse()->write("</TD>");
		req->getResponse()->write("<TD>");
		req->getResponse()->write(_ftoa(position.y));
		req->getResponse()->write("</TD>");
		req->getResponse()->write("<TD>");
		req->getResponse()->write(_ftoa(position.z));
		req->getResponse()->write("</TD>");
		req->getResponse()->write("</TR>");
		req->getResponse()->write("<TR>");
		req->getResponse()->write("<TD>Orientation locale</TD>");
		req->getResponse()->write("<TD>");
		req->getResponse()->write(_ftoa(orientation.x));
		req->getResponse()->write("</TD>");
		req->getResponse()->write("<TD>");
		req->getResponse()->write(_ftoa(orientation.y));
		req->getResponse()->write("</TD>");
		req->getResponse()->write("<TD>");
		req->getResponse()->write(_ftoa(orientation.z));
		req->getResponse()->write("</TD>");
		req->getResponse()->write("</TR>");
		req->getResponse()->write("</TABLE>");

		req->getResponse()->write("<INPUT TYPE=\"submit\" VALUE=\"Modifier\">");
		req->getResponse()->write("</FORM>");
		req->getResponse()->flushBuffer();
	}

	req->getResponse()->write("<HR>");
	req->getResponse()->write("<H2>Attributs</H2>");
	req->getResponse()->write("<FORM METHOD=GET action=\"changeAttributes\">");
	req->getResponse()->write("<input type=\"hidden\" name=\"alias\" value=\"");
	req->getResponse()->write(id);
	req->getResponse()->write("\" />");
	req->getResponse()->write("<ul>");

	const map<string,shared_ptr<Slot> >& attributes = entity->getSlots();
	map<string,shared_ptr<Slot> >::const_iterator it;
	for (it=attributes.begin();it!=attributes.end();it++)
		//   for (unsigned int i = 0; i < attributes.size(); i++)
	{
		//      Slot * attr = attributes[i];
		req->getResponse()->write("<li>");
		req->getResponse()->write(it->first);
		req->getResponse()->write(" = ");
		pair<string, shared_ptr<ValueSpecification> > pV;
		//string value = it->second->getValue().getStringFromValue();
		string value;
		BOOST_FOREACH(pV,it->second->getValues())
		value+="'"+pV.first+"' ";
		//_toString(attr->getValues()[0],attr->getDefiningProperty()->getType()->getName());
		req->getResponse()->write(value);
		req->getResponse()->write("</li>");
	}
	req->getResponse()->write("</ul>");
	req->getResponse()->write("<INPUT TYPE=\"submit\" VALUE=\"Modifier\">");
	req->getResponse()->write("</FORM>");
	req->getResponse()->flushBuffer();

	req->getResponse()->write("<HR>");
	req->getResponse()->write("<H2>Operations</H2>");
	req->getResponse()->write("<ul>");
	shared_ptr<Class> classifier = entity->getClassifier();
	const map<string, shared_ptr<Operation> >& operations = classifier->getOperations();
	map<string, shared_ptr<Operation> >::const_iterator ito;
	for (ito=operations.begin();ito!=operations.end();ito++)
	{
		req->getResponse()->write("<li>");
		req->getResponse()->write(" <a href=\"Operation?alias=");
		req->getResponse()->write(entity->getName());
		req->getResponse()->write("&oper=");
		req->getResponse()->write(ito->first);
		req->getResponse()->write("\" target = \"Body\">");
		req->getResponse()->write(ito->first);
		req->getResponse()->write("</a>");
		req->getResponse()->write("</li>");
	}
	req->getResponse()->write("</ul>");
	req->getResponse()->flushBuffer();

	req->getResponse()->write("<HR>");
	req->getResponse()->write("<H2>Signaux</H2>");
	req->getResponse()->write("<ul>");
	Classifier::OwnedBehaviorMap behaviors = classifier->getOwnedBehavior();
	map<string, shared_ptr<Behavior> >::iterator itB;
	for (itB = behaviors.begin(); itB != behaviors.end(); itB++)
	{
		shared_ptr<StateMachine> stateMachine = shared_dynamic_cast<StateMachine>(itB->second);
		shared_ptr<Region> region = stateMachine->getRegion_at(0);
		if (region)
		{
			vector< shared_ptr<Transition> > transitions = region->getTransition();
			for (unsigned int iTrans = 0; iTrans < transitions.size() ; iTrans++)
			{
				vector< shared_ptr<Trigger> > triggers = transitions[iTrans]->getTrigger();
				for (unsigned int iTrig = 0; iTrig < triggers.size(); iTrig++)
				{
					shared_ptr<Event> event = triggers[iTrig]->getEvent();
					if (event)
					{
						if (event->getType() == "SignalEvent")
						{
							shared_ptr<SignalEvent> signalEvent = shared_dynamic_cast<SignalEvent>(event);
							
							req->getResponse()->write("<li>");
							req->getResponse()->write(" <a href=\"Signal?alias=");
							req->getResponse()->write(entity->getName());
							req->getResponse()->write("&signal=");
							req->getResponse()->write((shared_dynamic_cast<SignalEvent>(event))->getSignalClass()->getName());
							req->getResponse()->write("\" target = \"Body\">");
							req->getResponse()->write((shared_dynamic_cast<SignalEvent>(event))->getSignalClass()->getName());
							req->getResponse()->write("</a>");
							req->getResponse()->write("</li>");
						}
					}
				}
			}
		}
	}
	req->getResponse()->write("</ul>");
	req->getResponse()->flushBuffer();

	req->getResponse()->write("<HR>");
	req->getResponse()->write("<H2>Machines d'états</H2>");
	req->getResponse()->write("<ul>");
	vector<shared_ptr<StateMachineBehaviorExecution> > stmbes = entity->getStateMachineExecutions();
	for (size_t i = 0; i < stmbes.size(); i++)
	{
		shared_ptr<Vertex> state = stmbes[i]->getCurrentState();
		string name = stmbes[i]->getStateMachine()->getName();
		string state_name = state ? state->getName() : "Inconnu";
		string tmp = "<li>$name - &Eacute;tat actif: $state</li>";
		boost::replace_all(tmp, "$name", name);
		boost::replace_all(tmp, "$state", state_name);
		req->getResponse()->write(tmp);
	}
	req->getResponse()->write("</ul>");
	req->getResponse()->write("<HR>");
	req->getResponse()->write("<a href=\"CenterView?alias="+entity->getName()+"\">Centrer la vue sur cette entité</a><br/>");
	req->getResponse()->write("<a href=\"SetRed?alias="+entity->getName()+"\">Mettre en rouge</a>");
	req->getResponse()->write("</body>");
	req->getResponse()->write("</html>");
	req->getResponse()->flushBuffer();
}
extern "C" HttpServlet* createManageEntityServlet()
		{
	return new ManageEntityServlet();
		}
extern "C" void destroyManageEntityServlet(HttpServlet* s)
{
	delete s;
}

