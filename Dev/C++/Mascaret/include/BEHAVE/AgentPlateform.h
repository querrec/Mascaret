#ifndef BEHAVE_AGENTPLATEFORM_H
#define BEHAVE_AGENTPLATEFORM_H

#include "Tools/veha_plateform.h"

namespace BEHAVE
{
class Agent;
class ACLMessage;
class AID;

class OrganisationalStructure;
class OrganisationalEntity;

class ExternalCommunicationManager;

class VEHA_API AgentPlateform
{
public :
	AgentPlateform(const string & name = "", int portNumber = 0, const string& ressourceDir="../HttpServerBaseDir", bool noServer=false);
	virtual ~AgentPlateform();

	void addAgent(shared_ptr<Agent> agent);
	inline string getName() {return _name;}
	inline int getPort() {return _port;}
	void sendMessage(shared_ptr<ACLMessage> message);
	bool isLocalAgent(shared_ptr<AID> aid);

	map<string , shared_ptr<Agent> > getAllAgents() {return _agents;} // TODO: is more a getAllLocalAgents ?
	shared_ptr<Agent> getLocalAgentByName(string name);

	inline void addOrganisationalStructure(shared_ptr<OrganisationalStructure> structure) {_structures.push_back(structure);}
	inline vector<shared_ptr<OrganisationalStructure> > getOrganisationalStructure(){return _structures;}
	inline void addOrganisationalEntity(shared_ptr<OrganisationalEntity> organisation) {_organisations.push_back(organisation);}
	inline vector<shared_ptr<OrganisationalEntity> > getOrganisationalEntity() {return _organisations;}

	inline void addAgentToInform(shared_ptr<AID> agent, string convId) {pair<string, shared_ptr<AID> > p (convId,agent); _agentsToInform.push_back(p);}
	inline vector< pair<string , shared_ptr<AID> > >  getAgentToInform(void) {return _agentsToInform;}
	inline shared_ptr<ExternalCommunicationManager> getCommunicationManager() {return _communicationManager;}

private :
	string _name;
	int _port;
	map < string, shared_ptr<Agent> > _agents;

	vector<shared_ptr<OrganisationalStructure> > _structures;
	vector<shared_ptr<OrganisationalEntity> > _organisations;

	vector <pair<string, shared_ptr<AID> > > _agentsToInform;
	shared_ptr<ExternalCommunicationManager> _communicationManager;
	//void _sendDistantMessage(shared_ptr<AID> aid, shared_ptr<ACLMessage> message);

};
}

#endif

