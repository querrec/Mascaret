#include "VEHA/Kernel/Environment.h"
#include "VEHA/Entity/Entity.h"
#include "VEHA/Kernel/InstanceSpecification.h"
#include "VEHA/Kernel/Model.h"
#include "HAVE/VirtualHuman.h"
#include <boost/foreach.hpp>

namespace VEHA
{

Environment::Environment(shared_ptr<Model> model) : _model(model)
{
}

Environment::~Environment()
{
}
void Environment::addInstance(shared_ptr<InstanceSpecification> value)
{
	_instances[value->getName()]=value;
}
void Environment::removeInstance(shared_ptr<InstanceSpecification> value)
{
	_instances.erase(value->getName());
}

void Environment::renameEntities(const vector<shared_ptr<Entity> >& entities,const vector<string>& newName)
{
	for(size_t i=entities.size();i--;)
	{
		//instance :
		_instances.erase(entities[i]->getName());
		entities[i]->rename(newName[i]);
		addInstance(entities[i]);
	}
	//la classe
	pair<string,shared_ptr<InstanceSpecification> >p;
	BOOST_FOREACH(p,_instances)
	{
		p.second->checkSlots();
	}

	//les values dans properties
	//
	//Mieux par event

	//model.register

}
vector<shared_ptr<Entity> > Environment::getEntities()
{
	vector<shared_ptr<Entity> > entities;
	map<string, shared_ptr<InstanceSpecification> >::iterator it=_instances.begin();
	while(it != _instances.end())
	{
		shared_ptr<Entity> ent=shared_dynamic_cast<Entity>(it->second);
		if(ent)
			entities.push_back(ent);
		it++;
	}
	return entities;
}
shared_ptr<Entity> Environment::getEntity(const string& name)
{
	return shared_dynamic_cast<Entity>(getInstance(name));
}
vector<shared_ptr<VirtualHuman> > Environment::getVirtualHumans()
{
	vector<shared_ptr<VirtualHuman> > entities;
	map<string, shared_ptr<InstanceSpecification> >::iterator it=_instances.begin();
	while(it != _instances.end())
	{
		shared_ptr<VirtualHuman> ent=shared_dynamic_cast<VirtualHuman>(it->second);
		if(ent)
			entities.push_back(ent);
		it++;
	}
	return entities;
}
shared_ptr<VirtualHuman> Environment::getVirtualHuman(const string& name)
{
	return shared_dynamic_cast<VirtualHuman>(getInstance(name));
}

string Environment::getUrl()
{
	return _url;
}
void Environment::setUrl(string url)
{
	_url=url;
}

}
