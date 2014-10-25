#include <boost/foreach.hpp>
#include "Tools/Network/Servlets/ManageAgentServlet.h"
#include "MascaretApplication.h"
#include "VEHA/Kernel/InstanceSpecification.h"
#include "VEHA/Kernel/Environment.h"
#include "VEHA/Entity/Entity.h"
#include "VEHA/Behavior/Common/Signal.h"
#include "VEHA/Behavior/Common/Trigger.h"
#include "VEHA/Behavior/StateMachine/SignalEvent.h"
#include "VEHA/Behavior/StateMachine/StateMachine.h"
#include "VEHA/Behavior/StateMachine/Transition.h"
#include "VEHA/Behavior/StateMachine/Region.h"
#include "HAVE/VirtualHuman.h"

using namespace VEHA;
using namespace HAVE;

using BEHAVE::AID;
using BEHAVE::ACLMessage;
using BEHAVE::KnowledgeBase;

void ManageAgentServlet::manageRequest(HttpRequest* req)
{
	string id = req->getParameter("alias");

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
		cerr <<"Can't find : "<<id<<endl;
		req->getResponse()->write("<html>");
		req->getResponse()->write("<body>");
		req->getResponse()->write("Can't find agent : " + id);
		req->getResponse()->write("</body>");
		req->getResponse()->write("</html>");
		return;
	}
	shared_ptr<VirtualHuman> human = shared_dynamic_cast<VirtualHuman>(entity);

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
	req->getResponse()->write(entity->getClassifier()->getName());
	req->getResponse()->write("</a>");
	req->getResponse()->write("</li>");
	req->getResponse()->write("</ul>");
	
	//take control
	
	//handle request
	
	string setControlled = req->getParameter("setControlled");
	if (setControlled == "true")
	{
		human->setControlledByHuman(true);
	}
	else if (setControlled == "false")
	{
		human->setControlledByHuman(false);
	}
	
	
	req->getResponse()->write("<HR>");
	req->getResponse()->write("<H2>Toggle controlled by human</H2>");
	
	req->getResponse()->write("<FORM METHOD=POST action=\"Agent?alias="+id+"\">");
	req->getResponse()->write("<input type=\"hidden\" name=\"alias\" value=\""+id+"\" />");
	if (human->getControlledByHuman())
	{
		req->getResponse()->write("<font color=\"darkred\">This agent is currently controlled. (will not follow procedure automatically)</font><br />");
		req->getResponse()->write("<input type=\"hidden\" name=\"setControlled\" value=\"false\" />");
		req->getResponse()->write("<input type=\"submit\" name=\"control\" value=\"Release control of this agent\" />");
	}
	else
	{
		req->getResponse()->write("<font color=\"darkgreen\">This agent is currently automatic. (will automatically follow procedures)</font><br />");
		req->getResponse()->write("<input type=\"hidden\" name=\"setControlled\" value=\"true\" />");
		req->getResponse()->write("<input type=\"submit\" name=\"control\" value=\"Take control of this agent\" />");
	}
	req->getResponse()->write("</FORM>");

	req->getResponse()->write("<HR>");
	req->getResponse()->write("<H2>Knowledge base</H2>");
	shared_ptr<KnowledgeBase> kb = human->getKnowledgeBase();
	if (kb)
	{
		req->getResponse()->write(" <a href=\"KnowledgeBase?alias=");
		req->getResponse()->write(entity->getName());
		req->getResponse()->write("\" target = \"_blank\">");
		req->getResponse()->write(kb->getName());
		req->getResponse()->write("</a>");
	}
	
	///
	
	req->getResponse()->write("<HR>");
	req->getResponse()->write("<H2>Geometrie</H2>");

	/*
	    Vector3 position, orientation ;
	    position = (shared_dynamic_cast<Entity>(entity))->getPosition();
	    orientation = (shared_dynamic_cast<Entity>(entity))->getOrientation();

	    req->getResponse()->write("<FORM METHOD=POST action=\"changeGeometry\" />");
	    req->getResponse()->write("<input type=\"hidden\" name=\"alias\" value=\"");
	    req->getResponse()->write(id);
	    req->getResponse()->write("\" />");
	    req->getResponse()->write("<TABLE BORDER=1>");
	    req->getResponse()->write("<TR>");
	    req->getResponse()->write("<TD>Position</TD>");
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
	    req->getResponse()->write("<TD>Orientation</TD>");
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
	 */

	req->getResponse()->write("<HR>");
	req->getResponse()->write("<H2>Attributs</H2>");
	req->getResponse()->write("<FORM METHOD=POST action=\"changeAttributes\">");
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
							req->getResponse()->write("<li>");
							req->getResponse()->write(" <a href=\"Signal?alias=");
							req->getResponse()->write(entity->getName());
							req->getResponse()->write("&signal=");
							req->getResponse()->write((shared_dynamic_cast<SignalEvent>(event))->getSignal()->getName());
							req->getResponse()->write("\" target = \"Body\">");
							req->getResponse()->write((shared_dynamic_cast<SignalEvent>(event))->getSignal()->getName());
							req->getResponse()->write("</a>");
							req->getResponse()->write("</li>");
						}
					}
				}
			}
		}

	}


	req->getResponse()->write("</ul>");
	req->getResponse()->write("<HR>");
	req->getResponse()->write("<H2>Messages</H2>");
	req->getResponse()->write(" AID : ");
	req->getResponse()->write(human->getAID()->toString());
	req->getResponse()->write("<H3>");
	req->getResponse()->write(" <a href=\"createMessage?alias=");
	req->getResponse()->write(human->getName());
	req->getResponse()->write("\" target = \"Body\">");
	req->getResponse()->write("Envoyer un message");
	req->getResponse()->write("</a>");
	req->getResponse()->write("</H3>");
	req->getResponse()->write("<H3>Non lus</H3>");
	vector<shared_ptr<ACLMessage> > nl=human->getMessageQueue();
	req->getResponse()->write("<TABLE BORDER=1>");
	req->getResponse()->write("<TR>");
	req->getResponse()->write("<TD>De </TD>");
	req->getResponse()->write("<TD>Performative </TD>");
	req->getResponse()->write("<TD>Contenu </TD>");
	req->getResponse()->write("</TR>");
	while (!nl.empty())
	{
		shared_ptr<ACLMessage> msg = nl.front();
		//nl.pop();
		nl.erase(nl.begin());
		req->getResponse()->write("<TR>");
		req->getResponse()->write("<TD>");
		req->getResponse()->write(msg->getSender()->toString());
		req->getResponse()->write("</TD>");
		req->getResponse()->write("<TD>");
		req->getResponse()->write(msg->getPerformativeText());
		req->getResponse()->write("</TD>");
		req->getResponse()->write("<TD>");
		req->getResponse()->write(msg->getContent());
		req->getResponse()->write("</TD>");
		req->getResponse()->write("</TR>");
	}
	req->getResponse()->write("</TABLE>");

	req->getResponse()->write("<H3>Lus</H3>");
	vector<shared_ptr<ACLMessage> > l = human->getChecked();
	req->getResponse()->write("<TABLE BORDER=1>");
	req->getResponse()->write("<TR>");
	req->getResponse()->write("<TD>De </TD>");
	req->getResponse()->write("<TD>Performative </TD>");
	req->getResponse()->write("<TD>Contenu </TD>");
	req->getResponse()->write("</TR>");
	for (unsigned int i = 0; i < l.size(); i++)
	{
		shared_ptr<ACLMessage> msg = l[i];
		req->getResponse()->write("<TR>");
		req->getResponse()->write("<TD>");
		req->getResponse()->write(msg->getSender()->toString());
		req->getResponse()->write("</TD>");
		req->getResponse()->write("<TD>");
		req->getResponse()->write(msg->getPerformativeText());
		req->getResponse()->write("</TD>");
		req->getResponse()->write("<TD>");
		req->getResponse()->write(msg->getContent());
		req->getResponse()->write("</TD>");
		req->getResponse()->write("</TR>");
	}
	req->getResponse()->write("</TABLE>");

	req->getResponse()->write("<H3>Envoyes</H3>");
	vector<shared_ptr<ACLMessage> > e = human->getSent();
	req->getResponse()->write("<TABLE BORDER=1>");
	req->getResponse()->write("<TR>");
	req->getResponse()->write("<TD>A </TD>");
	req->getResponse()->write("<TD>Performative </TD>");
	req->getResponse()->write("<TD>Contenu </TD>");
	req->getResponse()->write("</TR>");
	for (unsigned int i = 0; i < e.size(); i++)
	{
		shared_ptr<ACLMessage> msg = e[i];
		req->getResponse()->write("<TR>");
		req->getResponse()->write("<TD>");
		vector< shared_ptr<AID> > receivers = msg->getReceivers();
		for (unsigned int i = 0; i < receivers.size() ; i++)
		{
			req->getResponse()->write(receivers[i]->toString());
		}
		req->getResponse()->write("</TD>");
		req->getResponse()->write("<TD>");
		req->getResponse()->write(msg->getPerformativeText());
		req->getResponse()->write("</TD>");
		req->getResponse()->write("<TD>");
		req->getResponse()->write(msg->getContent());
		req->getResponse()->write("</TD>");
		req->getResponse()->write("</TR>");
	}
	req->getResponse()->write("</TABLE>");
	req->getResponse()->write("<HR>");
	req->getResponse()->write("<a href=\"CenterView?alias="+human->getName()+"\">Centrer la vue sur cette entité</a><br/>");
	req->getResponse()->write("<a href=\"SetRed?alias="+human->getName()+"\">Mettre en rouge</a>");
	req->getResponse()->write("</body>");
	req->getResponse()->write("</html>");
}
extern "C" HttpServlet* createManageAgentServlet()
		{
	return new ManageAgentServlet();
		}
extern "C" void destroyManageAgentServlet(HttpServlet* s)
{
	delete s;
}
