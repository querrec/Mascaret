#include "BEHAVE/ACLMessage.h"
#include "BEHAVE/AID.h"

#include "Tools/Loaders/XmlParser.h"

namespace BEHAVE
{


ACLMessage::ACLMessage(ACLPerformative performative) : _performative(performative), _hasXMLContent(false)
{}

ACLMessage::~ACLMessage()
{}

string ACLMessage::getPerformativeText()
{
	string res;

	switch(_performative)
	{
	case ACCEPT_PROPOSAL : res = "ACCEPT_PROPOSAL";
	break;
	case AGREE : res = "AGREE";
	break;
	case CANCEL : res = "CANCEL";
	break;
	case CFP : res = "CFP";
	break;
	case CONFIRM : res = "CONFIRM";
	break;
	case DISCONFIRM : res = "DISCONFIRM";
	break;
	case FAILURE : res = "FAILURE";
	break;
	case INFORM : res = "INFORM";
	break;
	case INFORM_IF : res = "INFORM_IF";
	break;
	case INFORM_REF : res = "INFORM_REF";
	break;
	case NOT_UNDERSTOOD : res = "NOT_UNDERSTOOD";
	break;
	case PROPAGATE : res = "PROPAGATE";
	break;
	case PROPOSE :  res = "PROPOSE";
	break;
	case PROXY : res = "PROXY";
	break;
	case QUERY_IF : res = "QUERY_IF";
	break;
	case QUERY_REF : res = "QUERY_REF";
	break;
	case REFUSE : res = "REFUSE";
	break;
	case REJECT_PROPOSAL : res = "REJECT_PROPOSAL";
	break;
	case REQUEST : res = "REQUEST";
	break;
	case REQUEST_WHEN : res = "REQUEST_WHEN";
	break;
	case REQUEST_WHENEVER : res = "REQUEST_WHENEVER";
	break;
	case SUBSCRIBE : res = "SUBSCRIBE";
	break;
	case UNKNOWN : res = "UNKNOWN";
	default : res = "UNKNOWN";
	}
	return res;
}

string ACLMessage::toXML()
{
	string result;
	char portS[10];

	shared_ptr<XmlParser> parser = shared_ptr<XmlParser>(new XmlParser());
	parser->createFile(getPerformativeText());
	shared_ptr<XmlNode> root = parser->getRoot();

	shared_ptr<XmlNode> receiversNode = shared_ptr<XmlNode> (new XmlNode("Receivers"));
	root->addChild(receiversNode);
	for (unsigned int i = 0; i < _receivers.size(); i++)
	{
		shared_ptr<XmlNode> receiverNode = shared_ptr<XmlNode> (new XmlNode("Receiver"));
		receiversNode->addChild(receiverNode);
		receiverNode->addTextChild("Name",_receivers[i]->getName());
		receiverNode->addTextChild("Plateform",_receivers[i]->getPlateformName());
		sprintf(portS,"%d",_receivers[i]->getPlateformPort());
		receiverNode->addTextChild("Port",portS);
	}

	shared_ptr<XmlNode> senderNode = shared_ptr<XmlNode> (new XmlNode("Sender"));
	root->addChild(senderNode);
	senderNode->addTextChild("Name",_sender->getName());
	senderNode->addTextChild("Plateform",_sender->getPlateformName());
	sprintf(portS,"%d",_sender->getPlateformPort());
	senderNode->addTextChild("Port",portS);

	if (_conversationID == "") _conversationID = "not_set";
	root->addTextChild("ConversationID", _conversationID);

	if (_hasXMLContent)
	{
		shared_ptr<XmlNode> contentNode = shared_ptr<XmlNode> (new XmlNode("Content"));
		root->addChild(contentNode);
		contentNode->addChild(_xmlContent);
	}
	else
		root->addTextChild("Content", _content);

	result = parser->writeString();

	return result;
}

void ACLMessage::setXMLContent(shared_ptr<XmlNode> content)
{
	_hasXMLContent = true;
	_xmlContent = content;
}

shared_ptr<XmlNode> ACLMessage::getXMLContent(void)
{
	return _xmlContent;
}


}
