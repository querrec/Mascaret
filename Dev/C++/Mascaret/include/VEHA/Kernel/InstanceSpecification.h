	#ifndef _v2_VEHA_Kernel_InstanceSpecification_H
#define _v2_VEHA_Kernel_InstanceSpecification_H

#include "Tools/veha_plateform.h"
#include "VEHA/Kernel/Class.h"

#include "callbacks.h"
namespace VEHA
{
class BehaviorExecution;
class StateMachineBehaviorExecution;
class Slot;
class Class;
class Property;

class VEHA_API InstanceSpecification : public Element
{
public :
	InstanceSpecification(const string& name,shared_ptr<Class>);
	virtual ~InstanceSpecification();
	inline virtual string getFullName()const{return getClassifier()->getFullName()+"::"+getName();}
	inline void rename(const string& newName)
	{
		shared_ptr<Class> c=getClassifier();
		if(c)
			c->_restockInstance(shared_dynamic_cast<InstanceSpecification>(shared_from_this()),newName);
	}
	void checkSlots();
	// --- Attributes ---
	// --- Relations ---
	//------ classifier -----
protected :
	void _createInstanceFromClass();
	weak_ptr<Class>   _classifier;
public :
	inline shared_ptr<Class> getClassifier(void) const { return _classifier.lock();}
	//------ slot -----
	typedef map<string,shared_ptr<Slot> > SlotMap;
protected :
	SlotMap _slots;
	friend class Class;
	void addSlot(shared_ptr<Property> prop);
	void removeSlot(const string& name);
public :
	inline const SlotMap& getSlots(void) { return _slots;}
	bool isActiveState(const string& state);
protected:
	//		inline void addSlot(Slot * value) { _slot.push_back(value);}
	vector<shared_ptr<BehaviorExecution> > _executions;
	vector<shared_ptr<StateMachineBehaviorExecution> > _smBehaviorExecution;
public:

	virtual void addExecution(shared_ptr<BehaviorExecution> be) {_executions.push_back(be);}
	typedef std::vector<shared_ptr<ValueSpecification> > OperationCallParameters;
	shared_ptr<Slot> getProperty(const string& name);
	shared_ptr<BehaviorExecution> executeOperation(const string& name,OperationCallParameters& p);
	bool stopOperation(const string& name);
	bool suspendOperation(const string& name);
	bool restartOperation(const string& name);
	inline vector<shared_ptr<BehaviorExecution> > getOperationExecutions() { return _operationsExecution;}
protected:
	virtual void onBehaviorStopped(BehaviorExecution* be);
	vector<shared_ptr<BehaviorExecution> > _operationsExecution;
public:
	//inline void addStateMachineExecution(shared_ptr<BehaviorExecution> behaviorExec){_smBehaviorExecution.push_back(behaviorExec);}
	inline void addStateMachineExecution(shared_ptr<StateMachineBehaviorExecution> behaviorExec){_smBehaviorExecution.push_back(behaviorExec);}
	//inline const vector<shared_ptr<BehaviorExecution> >&  getStateMachineExecutions(){return _smBehaviorExecution;}
	inline const vector<shared_ptr<StateMachineBehaviorExecution> >&  getStateMachineExecutions(){return _smBehaviorExecution;}

	//		executeQuery(shared_ptr<Operation>);
	// --- Operations ---
	// callback: ?
	// TODO: addSlo,removeSlot ? accessSlots?
	// TODO : rename ?
	/*
                void addCallbackOnSlotsChange(const cbSlotChangeValue& f);
                void removeCallbackOnSlotsChange(const SigSlot& f){property(slotName() ;
	 */
	void addCallbackOnRemoveSlotValue(const string & slotName,const cbSlotValue& f);
	template<typename SigSlot>
	inline void removeCallbackOnRemoveSlotValue(const string& slotName,const SigSlot& f);
	void addCallbackOnAddSlotValue(const string & slotName,const cbSlotValue& f);
	template<typename SigSlot>
	inline void removeCallbackOnAddSlotValue(const string& slotName,const SigSlot& f);

       	vector < void (*)(shared_ptr<InstanceSpecification>,string) > _CallbackOnChangeAttribute;
       	vector < void (*)(shared_ptr<InstanceSpecification>,shared_ptr<StateMachineBehaviorExecution>) > _CallbackOnChangeState;
       	vector < void (*)(shared_ptr<InstanceSpecification>,shared_ptr<BehaviorExecution>) > _CallbackOnExecuteOperation;

public :
        void fireChangeAttribute(string);
	void addCallbackOnChangeAttribute( void (*f)(shared_ptr<InstanceSpecification>,string) );
        void removeCallbackOnChangeAttribute( void (*f)(shared_ptr<InstanceSpecification>,string) );

        void fireChangeState(shared_ptr<StateMachineBehaviorExecution>);
	void addCallbackOnChangeState( void (*f)(shared_ptr<InstanceSpecification>,shared_ptr<StateMachineBehaviorExecution>) );
        void removeCallbackOnChangeState( void (*f)(shared_ptr<InstanceSpecification>,shared_ptr<StateMachineBehaviorExecution>) );

        void fireExecuteOperation(shared_ptr<BehaviorExecution> be);
	void addCallbackOnExecuteOperation( void (*f)(shared_ptr<InstanceSpecification>,shared_ptr<BehaviorExecution>) );
        void removeCallbackOnExecuteOperation( void (*f)(shared_ptr<InstanceSpecification>,shared_ptr<BehaviorExecution>) );

 
};

}
#endif

#ifndef _v2_VEHA_Kernel_InstanceSpecification_Ci
#define _v2_VEHA_Kernel_InstanceSpecification_Ci
#include "VEHA/Kernel/Slot.h"
namespace VEHA
{

template<typename SigSlot>
void InstanceSpecification::removeCallbackOnRemoveSlotValue(const string& slotName,const SigSlot& f)
{
	getProperty(slotName)->removeCallbackOnRemove(f);
}
template<typename SigSlot>
void InstanceSpecification::removeCallbackOnAddSlotValue(const string& slotName,const SigSlot& f)
{
	getProperty(slotName)->removeCallbackOnAdd(f);
}

}

#endif
