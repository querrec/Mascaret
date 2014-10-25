#ifndef _v2_VEHA_Kernel_Operation_H
#define _v2_VEHA_Kernel_Operation_H

#include "Tools/veha_plateform.h"
#include "VEHA/Kernel/BehavioralFeature.h"
#include "VEHA/Behavior/Common/Parameters.h"
#include "VEHA/Kernel/Class.h"
class ModelLoader;

namespace VEHA
{
class Class;
class Classifier;
class Parameter;
class Constraint;

class VEHA_API Operation : public BehavioralFeature
{
public :
	Operation(const string& name,shared_ptr<Class> owner,shared_ptr<Behavior> behavior);
	virtual ~Operation();
	inline virtual string getFullName() const{return getClassifier()->getFullName()+"."+getName();}
	// --- Attributes ---
	// --- Relations ---
	//------ classifier -----
protected :
	weak_ptr<Class>   _classifier;
public :
	typedef std::vector<shared_ptr<ValueSpecification> > OperationCallParameters;
	bool createBehaviorParameters(const OperationCallParameters& p, Parameters & behaviorParameters);
	inline shared_ptr<Class> getClassifier(void) const{ return _classifier.lock();}
	//inline void setClassifier(shared_ptr<Class> classifier) { _classifier = classifier;}
	//------ type -----
protected :
	//DEPRECATED ???
	weak_ptr<Classifier>  _type;
public :
	inline shared_ptr<Classifier> getType(void) const { return _type.lock();}
	//inline void setType(shared_ptr<Classifier> type) { _type = type;}
	//------ parameter -----
	inline void addParameter(shared_ptr<Parameter> value) { _parameter.push_back(value);}
protected :
	vector< shared_ptr<Parameter> > _parameter;
	friend class ::ModelLoader;
public :
	inline const vector< shared_ptr<Parameter> >& getParameter(void) { return _parameter;}
	inline shared_ptr<Parameter> getParameter_at(int index) { return _parameter[index];}

protected :
	vector< shared_ptr<Constraint> > _preconditions;
	vector< shared_ptr<Constraint> > _postconditions;
	shared_ptr<Constraint> _bodycondition;
public :
	inline const vector< shared_ptr<Constraint> >& getPreconditions(void) { return _preconditions;}
	inline shared_ptr<Constraint> getPrecondition_at(int index) { return _preconditions[index];}
        inline void addPrecondition(shared_ptr<Constraint> pre) { _preconditions.push_back(pre);}
	inline const vector< shared_ptr<Constraint> >& getPostconditions(void) { return _postconditions;}
	inline shared_ptr<Constraint> getPostcondition_at(int index) { return _postconditions[index];}
        inline void addPostcondition(shared_ptr<Constraint> post) { _postconditions.push_back(post);}
	inline const shared_ptr<Constraint>& getBodycondition(void) { return _bodycondition;}
        inline void setBodycondition(shared_ptr<Constraint> body) { _bodycondition=body;}
	// --- Operations ---
};

}
#endif
