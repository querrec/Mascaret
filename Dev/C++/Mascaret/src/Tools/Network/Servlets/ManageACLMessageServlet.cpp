#include "Tools/Network/Servlets/ManageACLMessageServlet.h"
#include "VEHA/Kernel/Class.h"
#include "VEHA/Kernel/Environment.h"
#include "BEHAVE/ACLMessage.h"
#include "BEHAVE/AID.h"
#include "MascaretApplication.h"
#include "Tools/Loaders/XmlParser.h"
#include "BEHAVE/Agent.h"
#include "BEHAVE/AgentPlateform.h"

using namespace VEHA;
using namespace BEHAVE;

void ManageACLMessageServlet::manageRequest(HttpRequest* req)
{
	shared_ptr<XmlParser> parser = shared_ptr<XmlParser>(new XmlParser());
	if (!parser->parseString(req->getContent()))
	{
		cerr << "Erreur de Parse..." << endl;
	}
	else
	{
		shared_ptr<XmlNode> root = parser->getRoot();
		string performative = root->getName();
		ACLPerformative perf;
		if (performative == "ACCEPT_PROPOSAL") perf =ACCEPT_PROPOSAL;
		if (performative == "AGREE") perf =AGREE;
		if (performative == "CANCEL") perf =CANCEL;
		if (performative == "CFP") perf =CFP;
		if (performative == "CONFIRM") perf =CONFIRM;
		if (performative == "DISCONFIRM") perf =DISCONFIRM;
		if (performative == "FAILURE") perf =FAILURE;
		if (performative == "INFORM") perf =INFORM;
		if (performative == "INFORM_IF") perf =INFORM_IF;
		if (performative == "INFORM_REF") perf =INFORM_REF;
		if (performative == "NOT_UNDERSTOOD") perf =NOT_UNDERSTOOD;
		if (performative == "PROPAGATE") perf =PROPAGATE;
		if (performative == "PROPOSE") perf =PROPOSE;
		if (performative == "PROXY") perf =PROXY;
		if (performative == "QUERY_IF") perf =QUERY_IF;
		if (performative == "QUERY_REF") perf =QUERY_REF;
		if (performative == "REFUSE") perf =REFUSE;
		if (performative == "REJECT_PROPOSAL") perf =REJECT_PROPOSAL;
		if (performative == "REQUEST") perf =REQUEST;
		if (performative == "REQUEST_WHEN") perf =REQUEST_WHEN;
		if (performative == "REQUEST_WHENEVER") perf =REQUEST_WHENEVER;
		if (performative == "SUBSCRIBE") perf =SUBSCRIBE;
		if (performative == "UNKNOWN") perf =UNKNOWN;

		shared_ptr<ACLMessage> dummyMsg = shared_ptr<ACLMessage>(new ACLMessage(perf));
		shared_ptr<XmlNode> recs = root->getChildByName("Receivers");
		shared_ptr<XmlNode> rec = recs->getFirstChild();
		while(rec)
		{
			if (rec->getName() == "Receiver")
			{
				shared_ptr<XmlNode> nameNode = rec->getChildByName("Name");
				string nameString = nameNode->getChildByName("text")->getString();

				shared_ptr<XmlNode> plateNode = rec->getChildByName("Plateform");
				string plateString = plateNode->getChildByName("text")->getString();

				shared_ptr<XmlNode> portNode = rec->getChildByName("Port");
				string portString = portNode->getChildByName("text")->getString();

				shared_ptr<AID> aid = shared_ptr<AID>(new AID(nameString, plateString, atoi(portString.c_str())));
				if (MascaretApplication::getInstance()->getAgentPlateform()->isLocalAgent(aid)) {
					dummyMsg->addReceiver(aid);
				}
			}
			rec = rec->getNext();
		}

		shared_ptr<XmlNode> sender = root->getChildByName("Sender");
		shared_ptr<XmlNode> nameNode = sender->getChildByName("Name");
		string nameString = nameNode->getChildByName("text")->getString();

		shared_ptr<XmlNode> plateNode = sender->getChildByName("Plateform");
		string plateString = plateNode->getChildByName("text")->getString();

		shared_ptr<XmlNode> portNode = sender->getChildByName("Port");
		string portString = portNode->getChildByName("text")->getString();

		shared_ptr<AID> aid = shared_ptr<AID>(new AID(nameString, plateString, atoi(portString.c_str())));
		dummyMsg->setSender(aid);

		shared_ptr<XmlNode> content = root->getChildByName("Content");
		shared_ptr<XmlNode> childContent = content->getFirstChild();

		shared_ptr<XmlNode> childC = content->getFirstChild();
		while (childC)
		{
			if (childC && childC->getName() != "text") childContent = childC;
			childC = childC->getNext();
		}

		string conversationID = root->getChildByName("ConversationID")->getChildByName("text")->getString();
		dummyMsg->setConversationID(conversationID);

		if (childContent && childContent->getName() != "text")
		{
			//cerr << "XML content"  << endl;
			//cerr << parser->writeString() << endl;
			dummyMsg->setXMLContent(childContent);
		}
		else {
			string contentString = content->getChildByName("text")->getString();
			dummyMsg->setContent(contentString);
		}

		/*
	      if (dummyMsg->getPerformative() == INFORM)
	      {
	         cerr << " DUMMY MSG INFORM ......" << endl;
	         shared_ptr<XmlNode> dummyContent = dummyMsg->getXMLContent();

	         shared_ptr<Model> model = shared_ptr<Model>(new Model());
	         shared_ptr<ModelLoader> modelLoader = shared_ptr<ModelLoader>(new ModelLoader(model, (dummyMsg->getXMLContent())));

	         cerr << dummyContent->getName() << endl;
	      }
		 */

		MascaretApplication::getInstance()->getAgentPlateform()->sendMessage(dummyMsg);
	}
}
extern "C" HttpServlet* createManageACLMessageServlet()
{
	return new ManageACLMessageServlet();
}
extern "C" void destroyManageACLMessageServlet(HttpServlet* s)
{
	delete s;
}
