#ifndef _v2_VEHA_Behavior_BasicActions_CallOperationAction_H
#define _v2_VEHA_Behavior_BasicActions_CallOperationAction_H

#include "Tools/veha_plateform.h"
#include "VEHA/Behavior/Common/Parameters.h"
#include "VEHA/Behavior/BasicActions/Action.h"

namespace VEHA
{

class Operation;
class InstanceSpecification;

class VEHA_API CallOperationAction : public Action
{
public :
	CallOperationAction();
	virtual ~CallOperationAction();

protected :
	shared_ptr<Operation>    _operation;
	shared_ptr<InstanceSpecification>    _target;
	string _stringarguments;
	map <string,string> _arguments;
public :
	inline shared_ptr<Operation> getOperation(void) { return _operation;}
	inline void setOperation(shared_ptr<Operation> operation) { _operation = operation;}

	inline shared_ptr<InstanceSpecification> getTarget(void) { return _target;}
	inline void setTarget(shared_ptr<InstanceSpecification> target) { _target = target;}

	Parameters getParameters();
	inline void setArguments(const string& arguments) {_stringarguments=arguments;}
	inline bool isDynamic() {return !_stringarguments.empty();}

	void addArgument(string name, string value);
	string getArgument(string name);

	virtual shared_ptr<BehaviorExecution> createBehaviorExecution(shared_ptr<InstanceSpecification>, const Parameters&, bool sync = false);

};

}
#endif
