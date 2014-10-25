#ifndef _v2_VEHA_Kernel_Slot_H
#define _v2_VEHA_Kernel_Slot_H

#include "Tools/veha_plateform.h"
#include "VEHA/Kernel/Element.h"
#include "VEHA/Kernel/InstanceValue.h"
#include "VEHA/Kernel/Property.h"
#include <stdexcept>
#include "Tools/utils.h"
#include <boost/signals.hpp>
#include "callbacks.h"
#include "CallbackManager.h"

#include "BackupUtils/IBackupEnabled.h" //addition for BackupUtils

namespace VEHA
{

using BackupUtils::IBackupEnabled; //addition for BackupUtils
using BackupUtils::Backup; //addition for BackupUtils

class Property;
class ValueSpecification;
class InstanceSpecification;
//class InstanceValue;

class VEHA_API Slot : public Element, public boost::noncopyable, public IBackupEnabled //addition for BackupUtils
{
public :
	//using virtual_shared_from_this<IBackupEnabled>::shared_from_this;

	// Voir si on pourait pas mettre les Slot en ptr
	Slot():Element("")
	{
	}
	//pass en ptr car creer dans le constructeur de InstanceSpecification
	Slot(InstanceSpecification * instance,shared_ptr<Property> property);
	virtual ~Slot();
	//unique/vector/map/set
	// --- Attributes ---
	// --- Relations ---
	//------ definingProperty -----
	
	void setBackup(shared_ptr<Backup>); //addition for BackupUtils
private:
	shared_ptr<Backup> _createBackupHook(); //addition for BackupUtils
	
protected :
	weak_ptr<Property>    _definingProperty;
	InstanceSpecification * _owningInstance;
public :
	inline shared_ptr<Property> getDefiningProperty(void) const { return _definingProperty.lock();}
	inline shared_ptr<ValueSpecification> getValueByName(const string& name)
			{
		CallbackManager_changesMethod::addChangeMethodSignal(&_cbOnChange); //TODO pb si le slot disparait
		_update();
#if __GCC_VERSION__ > 40100
		return _values.at(name);
#else
		return MAP_AT(_values,name);
#endif
			}
	inline bool exist(const string& name)
	{
		CallbackManager_changesMethod::addChangeMethodSignal(&_cbOnChange); //TODO pb si le slot disparait
		_update();
		return (_values.find(name)!=_values.end());
	}
	inline const shared_ptr<ValueSpecification> getValue()
	{
		CallbackManager_changesMethod::addChangeMethodSignal(&_cbOnChange); //TODO pb si le slot disparait
		_update();
		if (_values.size()==1)
			return _values.begin()->second;
		else if(_values.empty())
			return shared_ptr<ValueSpecification>();
		else throw std::range_error("Slot::getValue "+getDefiningProperty()->getName()+" - Error : value is not unique");
	}
	//------ value -----
protected :
	void _addValue(shared_ptr<ValueSpecification> value);
	//		shared_ptr<ValueSpecification> _value;
	map<string,shared_ptr<ValueSpecification> > _values;
	virtual inline bool _canRemoveValue()
	{
		return (_values.size()>getDefiningProperty()->getMinimumNumber());
	}
	inline shared_ptr<Slot> _getOppositeSlot(const string& name);
	inline void _removeValue(const string& name);
	//		vector< shared_ptr<ValueSpecification> >	_value;
public :

	inline const map<string, shared_ptr<ValueSpecification> >& getValues(void){
		CallbackManager_changesMethod::addChangeMethodSignal(&_cbOnChange); //TODO pb si le slot disparait
		_update(); return _values;}
	//		inline ValueSpecification * getValue_at(int index) { return _value[index];}
	//		inline ValueSpecification * getValue(void) { if(_value.size()) return _value[0]; else return NULL;}
	inline bool removeValue(const string& name);

	inline void clearValues()
	{
		_values.clear();
	}
	void addValueFromString(const string& str);
	virtual void addValue(const ValueSpecification& value){return addValue(value.clone());}
	void addValue(shared_ptr<ValueSpecification> value);
	void replaceValueFromString(const string& old, const string& newValue);
	void replaceValue(const string& old, const ValueSpecification& newValue);

	// --- Operations ---
	inline void      //const ValueSpecification&
	operator=(const ValueSpecification& val)
	{
		addValue(val);
	}
	virtual operator const ValueSpecification&();

	inline virtual bool operator ==(const Slot& s) const {return getName()==s.getName();}
	void addCallbackOnRemove(const cbSlotValue& f);
	template<typename SigSlot>
	void removeCallbackOnRemove(const SigSlot& f){_cbOnRemove.disconnect(f);}
	//		void addCallbackOnRemove(const string& valueName, const cbSlotValue& f);
	void addCallbackOnAdd(const cbSlotValue& f);
	template<typename SigSlot>
	void removeCallbackOnAdd(const SigSlot& f){_cbOnAdd.disconnect(f);}
	//		void addCallbackOnAdd(const string& valueName, const cbSlotValue& f);
protected:
	boost::signal<void (string,shared_ptr<ValueSpecification >) >	_cbOnAdd,_cbOnRemove;
	boost::signal0<void> _cbOnChange;


protected:
	inline virtual void _update(){}
};

}
#endif


#ifndef _v2_VEHA_Kernel_Slot_Ci
#define _v2_VEHA_Kernel_Slot_Ci
#include "VEHA/Kernel/InstanceSpecification.h"
namespace VEHA
{
shared_ptr<Slot> Slot::_getOppositeSlot(const string& name)
{
	shared_ptr<InstanceSpecification> ptr=*MAP_AT(_values,name);
	return ptr->getProperty(getDefiningProperty()->getOpposite()->getName());
}
void Slot::_removeValue(const string& name)
{
	_owningInstance->getClassifier()->fireCBOnSlotRemove(getName(),MAP_AT(_values,name));
	_cbOnRemove(getName(),MAP_AT(_values,name));
	_values.erase(name);
	_cbOnChange();
}
bool Slot::removeValue(const string& name)
{
	if (_canRemoveValue() &&
			!(getDefiningProperty()->getOpposite()&& !_getOppositeSlot(name)->_canRemoveValue() ) )
	{
		if (getDefiningProperty()->getOpposite())
			_getOppositeSlot(name)->_removeValue(_owningInstance->getName());
		_removeValue(name);
		return true;
	}
	else return false;

}

}
#endif
