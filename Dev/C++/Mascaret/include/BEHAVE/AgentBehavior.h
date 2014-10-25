#ifndef BEHAVE_AGENTBEHAVIOR_H
#define BEHAVE_AGENTBEHAVIOR_H

#include "Tools/veha_plateform.h"
#include "VEHA/Behavior/Common/Behavior.h"
#include "VEHA/Behavior/Common/BehaviorExecution.h"
#include "VEHA/Kernel/InstanceSpecification.h"
#include "BEHAVE/Agent.h"


namespace BEHAVE
{
using VEHA::Behavior;
using VEHA::BehaviorExecution;
using VEHA::InstanceSpecification;
using VEHA::Parameters;

enum AgentBehaviorType {Perception, Decision, Action};

typedef BehaviorExecution* (*BehaviorInitFunc)(shared_ptr<Behavior>, shared_ptr<InstanceSpecification>,const Parameters&);
typedef void (*BehaviorDeleterFunc)(BehaviorExecution*);

struct BehaviorInitDeleterFunc
{
	BehaviorInitFunc init;
	BehaviorDeleterFunc deleter;
};

class VEHA_API AgentBehavior : public Behavior
{
public :
	AgentBehavior(const string & name);
	virtual ~AgentBehavior();

public :
	shared_ptr<BehaviorExecution> createBehaviorExecution(shared_ptr<InstanceSpecification> host,const Parameters& p, bool sync = false);

	// Behavior Type
public :
	inline void setType(AgentBehaviorType type) {_type = type;}
	inline AgentBehaviorType getType(void) { return _type;}

protected:
	BehaviorInitDeleterFunc _lookForSymbols();
private :
	AgentBehaviorType _type;

};
}

#endif

