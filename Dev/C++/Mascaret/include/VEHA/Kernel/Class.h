#ifndef _v2_VEHA_Kernel_Class_H
#define _v2_VEHA_Kernel_Class_H
#include "Tools/veha_plateform.h"
#include "VEHA/Kernel/Classifier.h"
#include "VEHA/Kernel/Package.h"
#include "Tools/utils.h"
#include <stdexcept>
#include "callbacks.h"
#include <boost/signal.hpp>

namespace VEHA
{
class Trigger;
class Operation;
class Package;
class InstanceSpecification;
class Property;
class Slot;

class VEHA_API Class : public Classifier, public boost::noncopyable
{
public :
	Class(const string& name);
	virtual ~Class();
	inline virtual string getFullName() const{return (getPackage()?getPackage()->getFullName()+"::"+getName():getName());}
	// --- Attributes ---
	// --- Relations ---
	//------ package -----
protected :
	weak_ptr<Package> _package;
	virtual void _addFeaturesToChild(shared_ptr<Classifier> child);
public :
	virtual shared_ptr<ValueSpecification> createValueFromString(const string& str);

	inline shared_ptr<Package> getPackage(void) const { return _package.lock();}
	inline void setPackage(shared_ptr<Package> package) { _package = package;}

	void addCallbackOnCreateInstance(const boost::function1<void ,InstanceSpecification* > & f);
	template <typename SigSlot>
	void removeCallbackOnCreateInstance(const SigSlot& f){_cbOnCreate.disconnect(f);}

	// callback for instances:
	void addCallbackOnRemove(const cbSlotValue& f);
	template <typename SigSlot>
	void removeCallbackOnremove(const SigSlot& f){_cbOnSlotRemove.disconnect(f);}
	//		void addCallbackOnRemove(const string& valueName, const cbSlotValue& f);
	void addCallbackOnAdd(const cbSlotValue& f);
	template <typename SigSlot>
	void removeCallbackOnAdd(const SigSlot& f){_cbOnSlotAdd.disconnect(f);}
	//		void addCallbackOnAdd(const string& valueName, const cbSlotValue& f);

	//------ instances -----
protected :
	map<string, InstanceSpecification* >    _instances;
	friend class InstanceSpecification;
	string _addInstance(InstanceSpecification * value);
	void _removeInstance(InstanceSpecification * value);
	void _restockInstance(shared_ptr<InstanceSpecification> value,const string& newName);
public :
	map< string,shared_ptr<InstanceSpecification> > getInstances(bool searchInSubClass=true) const;
	shared_ptr<InstanceSpecification> getInstance(const string& name, bool searchInSubclass=true) const;
	//------ attributes -----
protected :
	map<string, shared_ptr<Property> >  _attributes;
public :
	virtual inline map<string, shared_ptr<Property> > getAttributes(void) const { return _attributes;}
	virtual bool addAttribute(shared_ptr<Property> value);
	virtual bool removeAttribute(const string & name);
	virtual inline bool hasAttribute(const string& name) {return _attributes.find(name) != _attributes.end();}
	//------ operations -----
protected :
	map< string,shared_ptr<Operation> > _operations;
public :
	virtual inline map<string, shared_ptr<Operation> > getOperations(void) const { return _operations;}
	virtual bool addOperation(shared_ptr<Operation> value);
	virtual bool removeOperation(const string & name);
	virtual inline bool hasOperation(const string& name) {return _operations.find(name) != _operations.end();}
	//------ associations -----
	//	protected :
	//		vector< Property * >	_associations;
	//	public :
	//		inline vector< Property * > getAssociations(void) { return _associations;}
	//		inline Property * getAssociations_at(int index) { return _associations[index];}
	//		inline void addAssociations(Property * value) { _associations.push_back(value);}
	//------ trigger -----
protected :
	friend class Slot; // TODO : trouvé une technique plus propre
	void fireCBOnCreate(InstanceSpecification* is);
	void fireCBOnSlotAdd(const string& slotName,shared_ptr<ValueSpecification> vs);
	void fireCBOnSlotRemove(const string& slotName,shared_ptr<ValueSpecification> vs);

	vector<shared_ptr<Trigger> >  _triggers;
public :
	inline vector< shared_ptr<Trigger> > getTriggers(void) { return _triggers;}
	inline shared_ptr<Trigger> getTriggers_at(int index) { return _triggers[index];}
	inline void addTriggers(shared_ptr<Trigger> value) { _triggers.push_back(value);}
	// --- Operations ---
protected:
	boost::signal1<void, InstanceSpecification*> _cbOnCreate;
	boost::signal2<void, string,shared_ptr<ValueSpecification> > _cbOnSlotAdd,_cbOnSlotRemove;
};

}
#endif
