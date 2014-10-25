#include "BEHAVE/AgentPlateform.h"
#include "BEHAVE/ACLMessage.h"
#include "BEHAVE/Agent.h"
#include "BEHAVE/AID.h"
#include "BEHAVE/HttpCommunicationManager.h"

namespace BEHAVE
{

AgentPlateform::AgentPlateform(const string & name, int portNumber, const string& ressourceDir, bool noServer)
: _name(name), _port(portNumber), _communicationManager(new HttpCommunicationManager(portNumber, ressourceDir, noServer))
{}

AgentPlateform::~AgentPlateform()
{}

void AgentPlateform::addAgent(shared_ptr<Agent> agent) 
{
	_agents[agent->getAID()->toString()] = agent;
}

void AgentPlateform::sendMessage(shared_ptr<ACLMessage> message)
{
	vector< shared_ptr<AID> > receivers = message->getReceivers();
	for (size_t i = 0; i < receivers.size(); i++)
	{
		if (receivers[i]->getPlateformName() == "localhost") receivers[i]->setPlateformName(_name);
		if (isLocalAgent(receivers[i]))
		{
			if (_agents.find(receivers[i]->toString()) != _agents.end())
			{
				shared_ptr<Agent> receiver=_agents[receivers[i]->toString()];
				receiver->postMessage(message);
				receiver->wakeup();
			}
		}
		else //_sendDistantMessage(receivers[i],message);
			_communicationManager->sendDistantMessage(receivers[i],message);
	}
}

bool AgentPlateform::isLocalAgent(shared_ptr<AID> aid)
{
	return ((aid->getPlateformName() == _name || aid->getPlateformName() == "localhost") && aid->getPlateformPort() == _port);
}

shared_ptr<Agent> AgentPlateform::getLocalAgentByName(string name)
{
	std::ostringstream oss;
	oss << name << "@" << _name << ":" << _port;
	return _agents[oss.str()];
}

/*void AgentPlateform::_sendDistantMessage(shared_ptr<AID> aid, shared_ptr<ACLMessage> message)
{
   char *buffer;

   unsigned long ipAddress;
   int connectPort = aid->getPlateformPort();
   ipAddress=getIpAddress(aid->getPlateformName().c_str());
   int fd = connectTcp(ipAddress,connectPort);

   string xmlMsg = message->toXML();
   int messageSize=xmlMsg.length()+1;
   buffer = new char [xmlMsg.size()+500];
   sprintf(buffer,"POST /ACLMessage HTTP/1.1\n" "Host: %s:%d\n" "Connection: close\nContent-Length: %d\n\n%s\n", aid->getPlateformName().c_str(),connectPort,messageSize,xmlMsg.c_str());
   writeFully(fd,buffer,strlen(buffer));

   close(fd);

}*/


}
