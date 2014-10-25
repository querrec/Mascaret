#include <sstream>
#include "VEHA/Kernel/Slot.h"
#include "VEHA/Kernel/Property.h"
#include "VEHA/Kernel/ValueSpecification.h"
#include "VEHA/Kernel/InstanceValue.h"
#include "VEHA/Kernel/InstanceSpecification.h"

#include "BackupUtils/SlotBackup.h" //implement specific backup creation
using BackupUtils::SlotBackup; //addition for BackupUtils


namespace VEHA
{

Slot::Slot(InstanceSpecification * instance, shared_ptr<Property> property)
:Element(property->getName()),_definingProperty(property),_owningInstance(instance)
{
	//TODO ...
	if (property->getDefaultValue())
		_addValue(property->getDefaultValue()->clone());
}

Slot::~Slot()
{
}
void Slot::addValueFromString(const string& str)
{
	addValue(getDefiningProperty()->createValueFromString(str));
}
void  Slot::addValue(shared_ptr<ValueSpecification> val)
{
	shared_ptr<Property> prop=getDefiningProperty();
	shared_ptr<Property>  oppositeProp;
	if (prop->getIsNavigable())
		oppositeProp=prop->getOpposite();
	if (oppositeProp)
	{
		shared_ptr<InstanceSpecification>
		//ptr =(dynamic_cast<const InstanceValue*>(&val))->getValue();
		ptr = *val;

		//	  shared_ptr<ValueSpecification> & v=
		ptr->getProperty(oppositeProp->getName())->_addValue(
				shared_ptr<ValueSpecification>(
						new InstanceValue(shared_dynamic_cast<InstanceSpecification>(_owningInstance->shared_from_this()))));
		//	v=shared_ptr<ValueSpecification>(new InstanceValue(_owningInstance));

	}
	//	cerr<<_value->getStringFromValue()<<endl;
	_addValue(val);      //TODO Gestion except
	//	cerr<<"After:"<<_value->getStringFromValue()<<endl;
	
	createBackup(); //create backup after update
}
Slot::operator const ValueSpecification&()
{
	return *getValue();
}

void Slot::_addValue(shared_ptr<ValueSpecification> val)
{
	/* ATTENTION .... Verifier quand meme si les type sont compatible (heritage ...)
            if (_definingProperty->getType()!=val->getType())
            {
                string error="Slot::addValue Error : expect type ";
                error+=_definingProperty->getType()->getName();
                error+="and value is of type : "+val->getType()->getName();
                throw runtime_error(error);
            }
	 */
	shared_ptr<Property> prop=getDefiningProperty();

	if ((prop->getMaximumNumber() == 1) && (_values.size()==1))
	{
		_values.clear(); // replace the value
	}
	else if ( _values.size() > (prop->getMaximumNumber()))
	{
		std::ostringstream strm;
		strm << "Slot::addValue Error : " << prop->getName()
	    				   << " number of values is " << _values.size()
	    				   << " maximum is " << prop->getMaximumNumber();
		throw std::range_error(strm.str());
	}

	_values.insert(make_pair<string,shared_ptr<ValueSpecification> >(val->getStringFromValue(), val));
	_cbOnAdd(getName(),val);
	_owningInstance->getClassifier()->fireCBOnSlotAdd(getName(),val);
	_cbOnChange();

	if (_owningInstance) _owningInstance->fireChangeAttribute(getName());
}

void Slot::replaceValueFromString(const string& old, const string& newValue)
{
	shared_ptr<Property> prop=getDefiningProperty();
	_removeValue(old);
	if (prop->getIsNavigable() && prop->getOpposite()) //TODO :PAS de VERIF!!!
		_getOppositeSlot(old)->_removeValue(_owningInstance->getName());
	addValueFromString(newValue);
}
void Slot::replaceValue(const string& old, const ValueSpecification& newValue)
{
	shared_ptr<Property> prop=getDefiningProperty();
	_removeValue(old);
	if (prop->getIsNavigable() && prop->getOpposite()) //TODO :PAS de VERIF!!!
		_getOppositeSlot(old)->_removeValue(_owningInstance->getName());
	addValue(newValue);
}
void Slot::addCallbackOnRemove(const cbSlotValue& f)
{
	_cbOnRemove.connect(f);
}
//		void addCallbackOnRemove(const string& valueName, const cbSlotValue& f);
void Slot::addCallbackOnAdd(const cbSlotValue& f)
{
	_cbOnAdd.connect(f);
}
//		void addCallbackOnAdd(const string& valueName, const cbSlotValue& f);


/*
 * addition for BackupUtils
 */
void Slot::setBackup(shared_ptr<Backup> bk)
{
	shared_ptr<SlotBackup> backup = shared_dynamic_cast<SlotBackup>(bk);
	
	if (backup)
	{
		shared_ptr<Slot> thisRef = shared_dynamic_cast<Slot>(shared_from_this());
		
		thisRef->_values = backup->_values;
		
		_cbOnChange();
		
		if (_owningInstance) cerr << "RESTORED: " << _owningInstance->getName() << endl;
	}
}

shared_ptr<Backup> Slot::_createBackupHook()
{
	shared_ptr<SlotBackup> backup;
	
	if (Element::shared_from_this())
	{
		shared_ptr<Slot> thisRef = shared_dynamic_cast<Slot>(shared_from_this());

		backup = shared_ptr<SlotBackup>(new SlotBackup(thisRef));
	
		backup->_values = thisRef->_values;
		
		backup->setOwnerInstance(_owningInstance);
	}
	return backup;
}
}

