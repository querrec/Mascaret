#ifndef _v2_VEHA_Behavior_Common_OpaqueBehavior_H
#define _v2_VEHA_Behavior_Common_OpaqueBehavior_H

#include "Tools/veha_plateform.h"
#include "VEHA/Behavior/Common/Behavior.h"

namespace VEHA
{

typedef BehaviorExecution* (*BehaviorInitFunc)(shared_ptr<Behavior>, shared_ptr<InstanceSpecification>,const Parameters&);
typedef void (*BehaviorDeleterFunc)(BehaviorExecution*);

struct BehaviorInitDeleterFunc
{
	BehaviorInitFunc init;
	BehaviorDeleterFunc deleter;
};

class VEHA_API OpaqueBehavior : public Behavior
{
public :
	OpaqueBehavior(const string& name);
	virtual ~OpaqueBehavior();
	// --- Attributes ---
public:
	string _language;
	string _body;
	inline void setBody(const string& body){_body=body;}
	// --- Relations ---
	// --- Operations ---
protected:
	virtual shared_ptr<BehaviorExecution> createBehaviorExecution(shared_ptr<InstanceSpecification> host,const Parameters& p, bool sync = false);
	BehaviorInitDeleterFunc _lookForSymbols(shared_ptr<Classifier> cl);
};

}
#endif
