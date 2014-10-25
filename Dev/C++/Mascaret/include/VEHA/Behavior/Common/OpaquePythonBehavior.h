#ifndef _v2_VEHA_Behavior_Common_OpaquePythonBehavior_H
#define _v2_VEHA_Behavior_Common_OpaquePythonBehavior_H

#include "Tools/veha_plateform.h"
#include "VEHA/Behavior/Common/OpaqueBehavior.h"

namespace VEHA
{
class VEHA_API OpaquePythonBehavior : public OpaqueBehavior
{
public :
	OpaquePythonBehavior(const string& name);
	virtual ~OpaquePythonBehavior();
	// --- Attributes ---
public:
	inline const string& getCode(){return _body;}
	// --- Relations ---
	// --- Operations ---
protected:
	virtual shared_ptr<BehaviorExecution> createBehaviorExecution(shared_ptr<InstanceSpecification> host,const Parameters& p);
};

}
#endif
