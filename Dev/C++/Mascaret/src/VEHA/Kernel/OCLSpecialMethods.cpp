#include "VEHA/Kernel/OCLSpecialMethods.h"
#include "VEHA/Entity/Entity.h"

namespace _oclSpecialMethods
{ 

//method: parameters contain self on back()

#define assertParameters(X) 	if (!(X) )throw runtime_error("OCL:Error in calling a special method: wrong parameters :"#X" - WARNING : last parameter is owner"  );

// convert all CollectionAnyValue with only one element to AnyValue
CollectionAnyValue convertParam(const CollectionAnyValue& parameters)
{
	CollectionAnyValue param=parameters;
	for (unsigned i=0;i<parameters.size();i++) {
		AnyValue obj=parameters[i];
		//obj must be an AnyValue, not a Collection
		//Sometimes, the selection by OID in VRXOCL returns a collection with only one element!
		if (obj.type() == typeid(CollectionAnyValue)) {
			CollectionAnyValue col=obj;
			obj = col[0];
		}
		param[i]=obj;
	}
	return param;
}

/** --- OCL default operators **/
AnyValue oclAsType(const CollectionAnyValue& parameters)
{
	//TODO: convert params??
	assertParameters(parameters.size()==2);
	string type=parameters[0];
	if (type=="int") return (int)parameters[1];
	if (type=="bool") return (bool)parameters[1];
	if (type=="real") return (double)parameters[1];
	return (string)parameters[1];
}
AnyValue oclIsTypeOf(const CollectionAnyValue& parameters)
{
	//TODO: convert params??
	assertParameters(parameters.size()==2);
	string type=parameters[0];
	return (toValueSpecification(parameters[1])->getType()->getName()==type);
}

AnyValue oclIsKindOf(const CollectionAnyValue& parameters)
{
	//TODO: convert params??
	assertParameters(parameters.size()==2);
	string type=parameters[0];
	return toValueSpecification(parameters[1])->getType()->isAByName(type);
}
AnyValue oclIsInState(const CollectionAnyValue& parameters)
{
	//TODO: convert params??
	assertParameters(parameters.size()==2);
	string state=parameters[0];
        cerr << " ---> " << state << endl;
	
	assertParameters(shared_ptr<InstanceSpecification>(parameters[1]));
	return shared_ptr<InstanceSpecification>(parameters[1])->isActiveState(state);
}
AnyValue vehaName(const CollectionAnyValue& parameters)
{
	assertParameters(parameters.size()==1);	
	assertParameters(shared_ptr<InstanceSpecification>(parameters[0]));
	return shared_ptr<InstanceSpecification>(parameters[0])->getName();
}

AnyValue vehaAllInstances(const CollectionAnyValue& parameters)
{
	assertParameters(parameters.size()==1);	
	assertParameters(shared_ptr<InstanceSpecification>(parameters[0]));

	map<string,shared_ptr<InstanceSpecification> > m = 
		(shared_ptr<InstanceSpecification>(parameters[0]))->getClassifier()->getInstances();
	map<string,shared_ptr<InstanceSpecification> >::const_iterator it;

	CollectionAnyValue cav;
	cav.type = CollectionAnyValue::Set;
	for (it= m.begin(); it!= m.end(); it++) {
		cav.push_back(it->second);
		//cerr << it->second->getName();
	}
	return cav; 
}


typedef function<AnyValue (const CollectionAnyValue&)> specialCallMethodType;
map<string,specialCallMethodType> _mapSpecialMethod;


void addMethods(){
_mapSpecialMethod.insert(make_pair<string,specialCallMethodType>("oclAsType",&oclAsType)); 
_mapSpecialMethod.insert(make_pair<string,specialCallMethodType>("oclIsKindOf",&oclIsKindOf)); 
_mapSpecialMethod.insert(make_pair<string,specialCallMethodType>("oclIsTypeOf",&oclIsTypeOf)); 
_mapSpecialMethod.insert(make_pair<string,specialCallMethodType>("oclIsInState",&oclIsInState)); 
_mapSpecialMethod.insert(make_pair<string,specialCallMethodType>("oclName",&vehaName));
_mapSpecialMethod.insert(make_pair<string,specialCallMethodType>("allInstances",&vehaAllInstances));
 }


AnyValue specialOpCall(const string& name,const CollectionAnyValue& params)
{
	if (_mapSpecialMethod.empty()) addMethods();
	return _mapSpecialMethod[name](params);
}


}
