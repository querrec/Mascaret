#include "BEHAVE/Mailbox.h"
#include "BEHAVE/ACLMessage.h"

namespace BEHAVE {

Mailbox::Mailbox()
{}

Mailbox::~Mailbox()
{}

shared_ptr<ACLMessage> Mailbox::check()
{
	shared_ptr<ACLMessage> msg;
	//if (!_messagesQueue.isEmpty())
	if (_messagesQueue.size())
	{
		msg = _messagesQueue[0];
		_messagesQueue.erase(_messagesQueue.begin());
		_messagesChecked.push_back(msg);
	}
	return msg;

}

}
