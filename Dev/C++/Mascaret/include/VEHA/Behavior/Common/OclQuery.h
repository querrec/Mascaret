#ifndef VEHA_Behavior_Common_OCL_QUERY_H
#define VEHA_Behavior_Common_OCL_QUERY_H

#include "Tools/veha_plateform.h"
#include "VEHA/Behavior/Common/OpaqueBehavior.h"
#include "VEHA/Behavior/Common/BehaviorExecution.h"
#include "VEHA/Kernel/expressionParser.h"

namespace VEHA
{

class VEHA_API OclQuery : public OpaqueBehavior
{
public :
	OclQuery(const string& name);
	virtual ~OclQuery();
	// --- Attributes ---
protected:
	OclParser::ExpressionTree _ep;
	bool _evaluated;
	virtual shared_ptr<BehaviorExecution> createBehaviorExecution(shared_ptr<InstanceSpecification> host,const Parameters& p, bool sync = false);
	vector<shared_ptr<ValueSpecification> > checkAndGetValues(const AnyValue& v);
};
class OclQueryBehaviorExecution : public BehaviorExecution
{
public:
	OclQueryBehaviorExecution(shared_ptr<Behavior> specif,shared_ptr<InstanceSpecification> host, const Parameters& p):BehaviorExecution(specif,host,p){}
	virtual double execute(double dt){return -1;}
};

}


#endif
