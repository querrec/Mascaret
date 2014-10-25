#include "VEHA/Kernel/Class.h"
#include "VEHA/Kernel/Package.h"
#include "VEHA/Kernel/InstanceSpecification.h"
#include "VEHA/Kernel/Property.h"
#include "VEHA/Kernel/Operation.h"
#include "VEHA/Kernel/Property.h"
#include "VEHA/Kernel/InstanceValue.h"
#include "Tools/utils.h"
#include <boost/foreach.hpp>
#include <algorithm>

namespace VEHA
{

Class::Class(const string& name)
:Classifier(name),_package()
{
}

Class::~Class()
{
}

void Class::_addFeaturesToChild(shared_ptr<Classifier> child)
{
	shared_ptr<Class> c(shared_dynamic_cast<Class>(child));
	pair<string,shared_ptr<Property> > p;
	BOOST_FOREACH(p,_attributes)
		c->addAttribute(p.second);
	pair<string,shared_ptr<Operation> > o;
	BOOST_FOREACH(o,_operations)
		c->addOperation(o.second);
}
string Class::_addInstance(InstanceSpecification* value)
{
	return MAP_ADDING(_instances,value);
}
void Class::_removeInstance(InstanceSpecification * value)
{
	_instances.erase(value->getName());
}
void Class::_restockInstance(shared_ptr<InstanceSpecification> value,const string& newName)
{
	_instances.erase(value->getName());
	_instances.insert(make_pair(newName,value.get()));
}

bool Class::addAttribute(shared_ptr<Property> value)
{
	for (size_t i=_children.size();i--;)
		shared_dynamic_cast<Class>(_children[i].lock())->addAttribute(value);
	pair<string,InstanceSpecification* > instance;
	BOOST_FOREACH(instance,_instances)
		instance.second->addSlot(value);
	return _attributes.insert(make_pair(value->getName(),value)).second;
}
bool Class::removeAttribute(const string& name)
{
	map<string,shared_ptr<Property > >::iterator it;
	if ((it=_attributes.find(name))==_attributes.end())
		return false;
	shared_ptr<Property> value =it->second;
	shared_ptr<Property> parentVal;
	for (size_t i=_parent.size();i--;)
	{
		shared_ptr<Class> c(shared_dynamic_cast<Class>(_parent[i].lock()));
		try
		{
			parentVal=MAP_AT(c->getAttributes(),name);
			if (value==parentVal)
				//On ne peut enlever un attribut du parent
				return false;
			else break;
		}
		catch(std::out_of_range&){}
	}
	_attributes.erase(name);
	pair<string,shared_ptr<InstanceSpecification> > instance;
	BOOST_FOREACH(instance,_instances)
	instance.second->removeSlot(name);

	for (size_t i=_children.size();i--;)
	{
		shared_ptr<Class> c(shared_dynamic_cast<Class>(_children[i].lock()));
		shared_ptr<Property> val=MAP_AT(c->getAttributes(),name);
		if (val==value)
			c->removeAttribute(name);
	}
	// parentVal!=null  : set parentVal
	if (parentVal)
		addAttribute(parentVal);
	return true;
}
bool Class::addOperation(shared_ptr<Operation> value)
{
	for (size_t i=_children.size();i--;)
		shared_dynamic_cast<Class>(_children[i].lock())->addOperation(value);
	return _operations.insert(make_pair(value->getName(),value)).second;
}
bool Class::removeOperation(const string &name)
{
	map<string,shared_ptr<Operation > >::iterator it;
	if ((it=_operations.find(name))==_operations.end())
		return false;
	shared_ptr<Operation> value =it->second;
	shared_ptr<Operation> parentVal;
	for (size_t i=_parent.size();i--;)
	{
		shared_ptr<Class> c(shared_dynamic_cast<Class>(_parent[i].lock()));
		try
		{
			parentVal=MAP_AT(c->getOperations(),name);
			if (value==parentVal)
				//On ne peut enlever un attribut du parent
				return false;
			else break;
		}
		catch(std::out_of_range&){}
	}
	_operations.erase(name);

	for (size_t i=_children.size();i--;)
	{
		shared_ptr<Class> c(shared_dynamic_cast<Class>(_children[i].lock()));
		shared_ptr<Operation> val=MAP_AT(c->getOperations(),name);
		if (val==value)
			c->removeOperation(name);
	}
	// parentVal!=null  : set parentVal
	if (parentVal)
		addOperation(parentVal);
	return true;
}

shared_ptr<ValueSpecification> Class::createValueFromString(const string& str)
{
	/*
            map<string, InstanceSpecification* >::iterator it = _instances.find(str);
            if (it != _instances.end())
               return shared_ptr<ValueSpecification>(new InstanceValue(shared_dynamic_cast<InstanceSpecification>(MAP_AT(_instances,str)->shared_from_this())));
            shared_ptr<ValueSpecification
	 */
	map<string,shared_ptr<InstanceSpecification> > instances = getInstances(true);
	return shared_ptr<ValueSpecification>(new InstanceValue(shared_dynamic_cast<InstanceSpecification>(MAP_AT(instances,str)->shared_from_this())));

}

map<string,shared_ptr<InstanceSpecification> > Class::getInstances( bool searchInSubclass) const
{
	map<string,shared_ptr<InstanceSpecification> > result;
	pair<string,InstanceSpecification*> p;
	BOOST_FOREACH(p, _instances)
	{
		result.insert(make_pair<string,shared_ptr<InstanceSpecification> >(p.first,shared_dynamic_cast<InstanceSpecification>(p.second->shared_from_this())));
	}
	if (searchInSubclass)
	{
		for (size_t i=_children.size();i-- ;)
		{
			map<string,shared_ptr<InstanceSpecification> > r=shared_dynamic_cast<Class>(_children[i].lock())->getInstances(true);
			result.insert(r.begin(),r.end());
		}
	}
	return result;
}
shared_ptr<InstanceSpecification> Class::getInstance(const string& name, bool searchInSubclass) const
{
	shared_ptr<InstanceSpecification> result;
	try { result=shared_dynamic_cast<InstanceSpecification>(MAP_AT(_instances,name)->shared_from_this()); }
	catch (std::out_of_range&)
	{
		if (!searchInSubclass) throw;
		for (size_t i=_children.size();i-- && !result;)
		{
			try
			{
				// TODO : dynamic cast pas sur
				result=shared_dynamic_cast<Class>(_children[i].lock())->getInstance(name,searchInSubclass);
			}
			catch (std::out_of_range&)
			{
			}

		}
	}
	if (!result)  throw std::out_of_range("Instance not found");
	else return result;
}
typedef boost::function1<void,InstanceSpecification* > cbInstanceCreate;
void Class::addCallbackOnCreateInstance(const cbInstanceCreate& f)
{
	_cbOnCreate.connect(f);
}

// callback for instances:
void Class::addCallbackOnRemove(const cbSlotValue& f)
{
	_cbOnSlotRemove.connect(f);
}
//		void addCallbackOnRemove(const string& valueName, const cbSlotValue& f);
void Class::addCallbackOnAdd(const cbSlotValue& f)
{
	_cbOnSlotAdd.connect(f);
}
//		void addCallbackOnAdd(const string& valueName, const cbSlotValue& f);
//
//

void Class::fireCBOnCreate(InstanceSpecification* is)
{
	_cbOnCreate(is);
	for (size_t i=_parent.size();i--;)
		shared_dynamic_cast<Class>(_parent[i].lock())->fireCBOnCreate(is);
}
void Class::fireCBOnSlotAdd(const string& slotName,shared_ptr<ValueSpecification> vs)
{
	_cbOnSlotAdd(slotName,vs);
	for (size_t i=_parent.size();i--;)
		shared_dynamic_cast<Class>(_parent[i].lock())->fireCBOnSlotAdd(slotName,vs);
}
void Class::fireCBOnSlotRemove(const string& slotName,shared_ptr<ValueSpecification> vs)
{
	_cbOnSlotRemove(slotName,vs);
	for (size_t i=_parent.size();i--;)
		shared_dynamic_cast<Class>(_parent[i].lock())->fireCBOnSlotRemove(slotName,vs);
}

}
