#ifndef _v2_VEHA_Behavior_Activity_ActionNode_H
#define _v2_VEHA_Behavior_Activity_ActionNode_H

#include "Tools/veha_plateform.h"
#include "VEHA/Behavior/Activity/ActivityNode.h"
#include "VEHA/Behavior/Common/BehaviorExecution.h"
#include "VEHA/Kernel/InstanceSpecification.h"


namespace VEHA
{

class VEHA_API ActionNode : public ActivityNode
{
public :
	ActionNode(const string& name, const string & kind = "action");
	virtual ~ActionNode();
	// --- Attributes ---
	// --- Relations ---
	//------ action -----
protected :
	shared_ptr<Action>    _action;
public :
	shared_ptr<BehaviorExecution> _currentExecution;
	inline shared_ptr<Action> getAction(void) { return _action;}
	inline void setAction(shared_ptr<Action> action) { _action = action;}
	// --- Operations ---
	virtual void start(shared_ptr<InstanceSpecification> host, map < string, shared_ptr<InstanceSpecification> > affectations, bool sync = false);
	void stop(void);
	virtual inline bool isRunning()
	{
		if(!_currentExecution)
                {
			return false;
                }
		return !_currentExecution->isFinished();
	}
	virtual inline shared_ptr<BehaviorExecution> getCurrentExecution()
	{
		return _currentExecution;
	}
protected:
	Parameters _paramsSendSignal(map<string, shared_ptr<InstanceSpecification> > affectations);
	Parameters _paramsCallOperation(map<string, shared_ptr<InstanceSpecification> > affectations);
	Parameters _paramsCallBehavior(map<string, shared_ptr<InstanceSpecification> > affectations);
};

}
#endif
