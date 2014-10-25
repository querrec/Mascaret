#ifndef BEHAVE_AGENT_H
#define BEHAVE_AGENT_H


#include "Tools/veha_plateform.h"
#include "VEHA/Kernel/InstanceSpecification.h"
#include "BEHAVE/AID.h"
#include "BEHAVE/Mailbox.h"
#include "BEHAVE/KnowledgeBase.h"


namespace BEHAVE
{
using VEHA::InstanceSpecification;
class AgentClass;
class AgentPlateform;
class AgentBehavior;
class AgentBehaviorExecution;
class ACLMessage;

class VEHA_API Agent : public InstanceSpecification
{
public :
	Agent(shared_ptr<AgentPlateform> ap,const string & name, shared_ptr<AgentClass> agentclass, string KBName = "default");
	virtual ~Agent();

	inline shared_ptr<AgentPlateform> getAgentPlateform() {return _plateform.lock();}

	virtual void addBehavior(const string& behaviorName,double interval=-1.0, bool start = true);
	shared_ptr<AgentBehavior> getBehaviorByName(const string& behaviorName);
	shared_ptr<AgentBehaviorExecution> executeOneShotBehavior(const string& behaviorName);
	shared_ptr<AgentBehaviorExecution> getBehaviorExecutionByName(const string& behaviorName);
	inline shared_ptr<AID> getAID() {return _aid;}

	void postMessage(shared_ptr<ACLMessage> message);
	shared_ptr<ACLMessage> receive();
	void send(shared_ptr<ACLMessage> message);

	inline vector<shared_ptr<ACLMessage> > getMessageQueue() {return _mailbox->getMessageQueue();}
	inline vector<shared_ptr<ACLMessage> > getChecked() {return _mailbox->getMessagesChecked();}
	inline vector<shared_ptr<ACLMessage> > getSent() {return _mailbox->getMessagesSent();}

	inline void setKnowledgeBase(shared_ptr<KnowledgeBase> base) {_knowledgeBase = base;}
	inline shared_ptr<KnowledgeBase> getKnowledgeBase(void) {return _knowledgeBase;}
	
	inline void setControlledByHuman(bool c) { _controlledByHuman = c; }
	inline bool getControlledByHuman() { return _controlledByHuman; }

	void addExecution( shared_ptr<BehaviorExecution> be); // Overwrite InstanceSpecification::addExecution

	virtual void wakeup();
private :
	vector<shared_ptr<AgentBehavior> > _behaviors;
	vector<shared_ptr<AgentBehaviorExecution> > _behaviorsExecution;
	shared_ptr<AID> _aid;
	weak_ptr<AgentPlateform> _plateform;

	shared_ptr<Mailbox> _mailbox;

	shared_ptr<KnowledgeBase> _knowledgeBase;
	
	bool _controlledByHuman;
};
}

#endif

