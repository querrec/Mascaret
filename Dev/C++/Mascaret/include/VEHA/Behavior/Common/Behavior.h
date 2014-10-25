#ifndef _v2_VEHA_Behavior_Common_Behavior_H
#define _v2_VEHA_Behavior_Common_Behavior_H

#include "Tools/veha_plateform.h"
#include "VEHA/Kernel/Classifier.h"
#include "VEHA/Kernel/Expression.h"
#include "VEHA/Behavior/Common/Parameters.h"
#include <stdexcept>

namespace VEHA
{
class InstanceSpecification;
class Constraint;
class ValueSpecification;
class BehaviorExecution;

class VEHA_API Behavior : public Classifier
{
public :
	Behavior(const string& name);
	virtual ~Behavior();
	inline virtual string getFullName(){return (getOwner()? getOwner()->getFullName()+"::"+getName():getName());}
	// --- Attributes ---
	// --- Relations ---
	//------ precondition -----
protected :
	vector< shared_ptr<Constraint> >    _precondition;
	OclParser::Context _context;
public :
	inline virtual OclParser::Context getContext() {return _context;}
	inline virtual void setContext(const OclParser::Context& context) {_context=context;}

	inline virtual shared_ptr<ValueSpecification> createValueFromString(const string& ){throw std::runtime_error("Can't create an instance of behaviro from string (Behavior::createValueFromString)");}
	inline vector< shared_ptr<Constraint> > getPrecondition(void) { return _precondition;}
	inline shared_ptr<Constraint> getPrecondition_at(int index) { return _precondition[index];}
	inline void addPrecondition(shared_ptr<Constraint> value) { _precondition.push_back(value);}
	//------ postcondition -----
protected :
	vector< shared_ptr<Constraint> >    _postcondition;
public :
	inline vector< shared_ptr<Constraint> > getPostcondition(void) { return _postcondition;}
	inline shared_ptr<Constraint> getPostcondition_at(int index) { return _postcondition[index];}
	inline void addPostcondition(shared_ptr<Constraint> value) { _postcondition.push_back(value);}
	//------ Classifier -----
protected :
	weak_ptr<Classifier>  _owner;
public :
	inline shared_ptr<Classifier>  getOwner(void) { return _owner.lock();}
	inline void setOwner(shared_ptr<Classifier> value) { _owner = value;}
	// --- Operations ---
	virtual shared_ptr<BehaviorExecution> createBehaviorExecution(shared_ptr<InstanceSpecification> host,const Parameters& p,bool sync=false) =0;

	virtual bool equalsTo(shared_ptr<Behavior>) {cerr << "To be defined" << endl; return false;}

};

}
#endif
