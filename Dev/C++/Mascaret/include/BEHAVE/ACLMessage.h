#ifndef BEHAVE_ACLMESSAGE_H
#define BEHAVE_ACLMESSAGE_H

#include "Tools/veha_plateform.h"
#include "MascaretApplication.h"

#include "VEHA/Behavior/Time/TimeExpression.h"

using namespace VEHA;

namespace BEHAVE
{
class AID;
enum ACLPerformative {ACCEPT_PROPOSAL, AGREE, CANCEL, CFP, CONFIRM, DISCONFIRM, FAILURE, INFORM, INFORM_IF, INFORM_REF, NOT_UNDERSTOOD, PROPAGATE, PROPOSE, PROXY, QUERY_IF, QUERY_REF, REFUSE, REJECT_PROPOSAL, REQUEST, REQUEST_WHEN, REQUEST_WHENEVER, SUBSCRIBE, UNKNOWN};

using VEHA::TimeExpression;

class VEHA_API ACLMessage
{
public :
	ACLMessage(ACLPerformative performative);
	virtual ~ACLMessage();

	inline ACLPerformative getPerformative() {return _performative;}
	string getPerformativeText();
	inline void setPerformative(ACLPerformative performative) {_performative = performative;}

	inline void addReceiver(shared_ptr<AID> receiver) {_receivers.push_back(receiver);}
	inline vector<shared_ptr<AID> >  getReceivers() { return _receivers;}
	inline void addReplyTo(shared_ptr<AID> replyTo) {_replyTo.push_back(replyTo);}

	inline string getContent(){return _content;}
	inline void setContent(string content) {_content = content;}

	void setXMLContent(shared_ptr<XmlNode> contentNode);
	shared_ptr<XmlNode> getXMLContent(void);
	inline bool contentIsXml() {return _hasXMLContent;}

	inline string getConversationID(){return _conversationID;}
	inline void setConversationID(string conversationID) {_conversationID = conversationID;}
	inline string getLanguage(){return _language;}
	inline void setLanguage(string language) {_language = language;}
	inline string getOnthology(){return _onthology;}
	inline void setOnthology(string onthology) {_onthology = onthology;}
	inline string getProtocol(){return _protocol;}
	inline void setProtocol(string protocol) {_protocol = protocol;}
	inline string getReplyBy(){return _replyBy;}
	inline void setReplyBy(string replyBy) {_replyBy = replyBy;}
	inline shared_ptr<AID> getSender() {return _sender;}
	inline void setSender(shared_ptr<AID> sender) {_sender = sender;}
	
	inline void setTimestamp(TimeExpression& te) { _timestamp = te; }
	inline TimeExpression getTimestamp() { return _timestamp; }

	string toXML();

private :
	ACLPerformative _performative;
	vector<shared_ptr<AID> > _receivers;
	vector<shared_ptr<AID> > _replyTo;
	string _content;
	string _conversationID;
	string _language;
	string _onthology;
	string _protocol;
	string _replyBy;
	shared_ptr<AID> _sender;
	shared_ptr<XmlNode> _xmlContent;
	bool _hasXMLContent;
	
	TimeExpression _timestamp;

};
}

#endif
