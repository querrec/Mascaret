#ifndef BEHAVE_MAILBOX_H
#define BEHAVE_MAILBOX_H

#include "Tools/veha_plateform.h"
#include "BEHAVE/ACLMessage.h"

namespace BEHAVE
{

class VEHA_API Mailbox
{
public :
	Mailbox();
	virtual ~Mailbox();

	inline void postMessage(shared_ptr<ACLMessage> message) {_messagesQueue.push_back(message);}
	shared_ptr<ACLMessage> check();
	inline void send(shared_ptr<ACLMessage> message) {_messagesSent.push_back(message);}

	inline vector<shared_ptr<ACLMessage> > getMessageQueue() {return _messagesQueue;}
	inline vector<shared_ptr<ACLMessage> > getMessagesChecked() {return _messagesChecked;}
	inline vector<shared_ptr<ACLMessage> > getMessagesSent() {return _messagesSent;}

private :
	vector<shared_ptr<ACLMessage> > _messagesQueue;
	vector<shared_ptr<ACLMessage> > _messagesChecked;
	vector<shared_ptr<ACLMessage> > _messagesSent;
};
}

#endif

