#include "VEHA/Kernel/InstanceSpecification.h"
#include "VEHA/Kernel/Class.h"
#include "VEHA/Kernel/Slot.h"
#include "VEHA/Kernel/SlotDerived.h"
#include "VEHA/Kernel/Operation.h"
#include "VEHA/Behavior/Common/behaviorScheduler.h"
#include "VEHA/Behavior/StateMachine/StateMachineBehaviorExecution.h"
#include "VEHA/Behavior/StateMachine/Vertex.h"
#include <stdexcept>

#include <boost/lambda/lambda.hpp>
#include <boost/foreach.hpp>
#include <boost/bind.hpp>

//using namespace boost::lambda;
namespace VEHA
{

class KeySlotException : public std::runtime_error
{
public:
	KeySlotException(const string & msg): std::runtime_error("KeySlotException: " + msg) { }
};



InstanceSpecification::InstanceSpecification(const string& name,shared_ptr<Class> cl):
							Element(name),_classifier(cl)
{
	shared_ptr<Class> c=getClassifier();
	if (c)
	{
		_createInstanceFromClass();
		string newName=c->_addInstance(this);
		setName(newName);
		c->fireCBOnCreate(this);
	}
}

InstanceSpecification::~InstanceSpecification()
{
//	cerr << "~InstanceSpecification "<<_name << endl;
	shared_ptr<Class> c=getClassifier();
	if (c)
		c->_removeInstance(this);
}

shared_ptr<Slot> InstanceSpecification::getProperty(const string& name)
{
	SlotMap::iterator it;
	if ((it=_slots.find(name))!=_slots.end())
		return (it->second);
	//else throw KeySlotException(getName() + "." + name) ;
	return shared_ptr<Slot>();
}

void InstanceSpecification::addSlot(shared_ptr<Property> prop)
{
	Slot* s=new Slot(this, prop) ;
	_slots.insert(make_pair(prop->getName(),shared_ptr<Slot>(s)));
}
void InstanceSpecification::removeSlot(const string& name)
{                        //TODO et les opposites?
	_slots.erase(name);
}

void InstanceSpecification::_createInstanceFromClass()
{
	shared_ptr<Class> c=getClassifier();
	if (c)
	{
		const map<string,shared_ptr<Property> >& prop=c->getAttributes();
		map<string,shared_ptr<Property> >::const_iterator it;
		for (it=prop.begin();it!=prop.end();it++)
		{
			Slot *s;
			if (it->second->getIsDerived())
				s=new SlotDerived(this, it->second) ;
			else
				s=new Slot(this, it->second) ;
			_slots.insert(make_pair(it->first,shared_ptr<Slot>(s)));
		}

/*
				Classifier::OwnedBehaviorMap behaviors = _classifier->getOwnedBehavior();
				map<string, shared_ptr<Behavior> >::iterator itB;
				for (itB = behaviors.begin(); itB != behaviors.end(); itB++)
				{
				   shared_ptr<Behavior> behavior = itB->second;
				   shared_ptr<InstanceSpecification> host = shared_dynamic_cast<InstanceSpecification>(shared_from_this());
				   BehaviorScheduler::getInstance()->executeBehavior(behavior,host,Parameters());
				}
		 */
	}
}
shared_ptr<BehaviorExecution> InstanceSpecification::executeOperation(const string& name,OperationCallParameters&p)
{
	shared_ptr<BehaviorExecution> be;
	//find operation
	try
	{
		shared_ptr<Class> c=getClassifier();
		if (c)
		{
			shared_ptr<Operation> op= MAP_AT(c->getOperations(), name);
			Parameters behaviorParameters;
			if (!op->createBehaviorParameters(p,behaviorParameters))
			{
				cerr<<"Parameters are not correct"<<endl;
				return be;
			}
			//TODO Method!!!
			shared_ptr<Behavior> b= op->getMethod();
			if (b)
			{
				be=BehaviorScheduler::getInstance()->executeBehavior(b,shared_dynamic_cast<InstanceSpecification>(shared_from_this()),behaviorParameters);
				//bind(&Personnel_parler::onUtter, this,_1);
				be->addCallbackOnBehaviorStop(boost::bind( &InstanceSpecification::onBehaviorStopped, this,_1));
			}
		}
	}
	catch(std::out_of_range&)
	{
		// TODO: Error catching
	}
	if(be)
	{
		fireExecuteOperation(be);
		_operationsExecution.push_back(be);
	}
	return be;
}
void InstanceSpecification::onBehaviorStopped(BehaviorExecution* be)
{
	vector<shared_ptr<BehaviorExecution> >::iterator it=_operationsExecution.begin();
	while(it != _operationsExecution.end())
	{
		if(it->get() == be)
			break;
		it++;
	}
	if(it != _operationsExecution.end())
	{
		_operationsExecution.erase(it);
	}
}
bool InstanceSpecification::stopOperation(const string& name)
{
	vector<shared_ptr<BehaviorExecution> >::iterator it=_operationsExecution.begin();
	while(it != _operationsExecution.end())
	{
		if((*it)->getSpecification()->getName() == name)
		{
			(*it)->stop();
			return true;
		}
		it++;
	}
	return false;
}
bool InstanceSpecification::suspendOperation(const string& name)
{
	vector<shared_ptr<BehaviorExecution> >::iterator it=_operationsExecution.begin();
	while(it != _operationsExecution.end())
	{
		if((*it)->getSpecification()->getName() == name)
		{
			(*it)->pause();
			return true;
		}
		it++;
	}
	return false;
}
bool InstanceSpecification::restartOperation(const string& name)
{
	vector<shared_ptr<BehaviorExecution> >::iterator it=_operationsExecution.begin();
	while(it != _operationsExecution.end())
	{
		if((*it)->getSpecification()->getName() == name)
		{
			(*it)->restart();
			return true;
		}
		it++;
	}
	return false;
}
void InstanceSpecification::checkSlots()
{
	int modif=0;
	SlotMap::iterator it;
	for(it=_slots.begin();it!=_slots.end();it++)
	{
		const map<string,shared_ptr<ValueSpecification> >&values=it->second->getValues();
		map<string,shared_ptr<ValueSpecification> >::const_iterator it2;
		for(it2=values.begin();it2!=values.end();it2++)
			if (it2->first!=it2->second->getStringFromValue())
			{
				shared_ptr<ValueSpecification > value=it2->second;
				it->second->removeValue(it2->first);
				it->second->addValue(value);
				modif++;
			}

	}
	cerr<<getFullName()<<" slot modifications : "<<modif<<endl;
}


//void InstanceSpecification::addCallbackOnSlotsChange(const cbSlotChangeValue& f)
//{//TODO
//}
//void InstanceSpecification::removeCallbackOnSlotsChange(const cbSlotChangeValue& f)
//{//TODO
//}

void InstanceSpecification::addCallbackOnRemoveSlotValue(const string & slotName,const cbSlotValue& f)
{
	getProperty(slotName)->addCallbackOnRemove(f);
}
void InstanceSpecification::addCallbackOnAddSlotValue(const string & slotName,const cbSlotValue& f)
{
	getProperty(slotName)->addCallbackOnAdd(f);
}


bool InstanceSpecification::isActiveState(const string& state)
{

	//TODO : callback à rajouter
	for(size_t i=_smBehaviorExecution.size();i--;)
	{
		shared_ptr<Vertex> v=_smBehaviorExecution[i]->getCurrentState();
		if (v && v->getName()==state) return true;
	}
	return false;
}

void InstanceSpecification::fireChangeAttribute(string attrName)
{
     for (unsigned i = 0; i < _CallbackOnChangeAttribute.size(); i ++)
     {
             (*_CallbackOnChangeAttribute[i])(shared_dynamic_cast<InstanceSpecification>(this->shared_from_this()),attrName);
     }
}

void InstanceSpecification::addCallbackOnChangeAttribute( void (*f)(shared_ptr<InstanceSpecification>,string) )
{
	_CallbackOnChangeAttribute.push_back(f);
}

void InstanceSpecification::removeCallbackOnChangeAttribute( void (*f)(shared_ptr<InstanceSpecification>,string) )
{
        unsigned int i;

        for (i = 0; i < _CallbackOnChangeAttribute.size(); i ++)
        {
                if (_CallbackOnChangeAttribute[i] == f) break;
        }

        if (i < _CallbackOnChangeAttribute.size())
                _CallbackOnChangeAttribute.erase(_CallbackOnChangeAttribute.begin() + i);
}

void InstanceSpecification::fireChangeState(shared_ptr<StateMachineBehaviorExecution> stm)
{
     for (unsigned i = 0; i < _CallbackOnChangeState.size(); i ++)
     {
             (*_CallbackOnChangeState[i])(shared_dynamic_cast<InstanceSpecification>(this->shared_from_this()),stm);
     }
}

void InstanceSpecification::addCallbackOnChangeState( void (*f)(shared_ptr<InstanceSpecification>,shared_ptr<StateMachineBehaviorExecution>) )
{
	_CallbackOnChangeState.push_back(f);
}

void InstanceSpecification::removeCallbackOnChangeState( void (*f)(shared_ptr<InstanceSpecification>, shared_ptr<StateMachineBehaviorExecution>) )
{
        unsigned int i;

        for (i = 0; i < _CallbackOnChangeState.size(); i ++)
        {
                if (_CallbackOnChangeState[i] == f) break;
        }

        if (i < _CallbackOnChangeState.size())
                _CallbackOnChangeState.erase(_CallbackOnChangeState.begin() + i);
}

void InstanceSpecification::fireExecuteOperation(shared_ptr<BehaviorExecution> be)
{
     for (unsigned i = 0; i < _CallbackOnExecuteOperation.size(); i ++)
     {
             (*_CallbackOnExecuteOperation[i])(shared_dynamic_cast<InstanceSpecification>(this->shared_from_this()),be);
     }
}

void InstanceSpecification::addCallbackOnExecuteOperation( void (*f)(shared_ptr<InstanceSpecification>,shared_ptr<BehaviorExecution>) )
{
	_CallbackOnExecuteOperation.push_back(f);
}

void InstanceSpecification::removeCallbackOnExecuteOperation( void (*f)(shared_ptr<InstanceSpecification>,shared_ptr<BehaviorExecution>) )
{
        unsigned int i;

        for (i = 0; i < _CallbackOnExecuteOperation.size(); i ++)
        {
                if (_CallbackOnExecuteOperation[i] == f) break;
        }

        if (i < _CallbackOnExecuteOperation.size())
                _CallbackOnExecuteOperation.erase(_CallbackOnExecuteOperation.begin() + i);
}
}
