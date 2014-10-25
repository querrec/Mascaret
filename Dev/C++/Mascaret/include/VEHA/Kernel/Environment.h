#ifndef _v2_VEHA_Kernel_Environment_H
#define _v2_VEHA_Kernel_Environment_H
#include "Tools/veha_plateform.h"
//#include "VEHA/Kernel/Model.h"
namespace HAVE
{
class VirtualHuman;
}
namespace VEHA
{
class Entity;
class Zone;
class InstanceSpecification;
class Model;

using HAVE::VirtualHuman;
class VEHA_API Environment
{
public :
	Environment(shared_ptr<Model> model);
	virtual ~Environment();

	void renameEntities(const vector<shared_ptr<Entity> >& entities,const vector<string>& newName);

	inline const map<string, shared_ptr<InstanceSpecification> >& getInstances();
	inline shared_ptr<InstanceSpecification> getInstance(const string& name);
	void addInstance(shared_ptr<InstanceSpecification> value) ;
	void removeInstance(shared_ptr<InstanceSpecification> value);

	inline shared_ptr<Zone> getMainZone();
	inline void setMainZone(shared_ptr<Zone> zone);

	vector<shared_ptr<Entity> > getEntities();
	shared_ptr<Entity> getEntity(const string& name);

	vector<shared_ptr<VirtualHuman> > getVirtualHumans();
	shared_ptr<VirtualHuman> getVirtualHuman(const string& name);

	string getUrl();
	void setUrl(string url);

	inline shared_ptr<Model> getModel() {return _model;}

protected :
	map<string, shared_ptr<InstanceSpecification> > _instances;
	shared_ptr<Model> _model;
	shared_ptr<Zone> _mainZone;
	string _url;
};
const map<string, shared_ptr<InstanceSpecification> >& Environment::getInstances()
{
	return _instances;
}
shared_ptr<InstanceSpecification> Environment::getInstance(const string& name)
{
	map<string, shared_ptr<InstanceSpecification> >::iterator it;
	if ((it=_instances.find(name))!=_instances.end())
		return it->second;
	return shared_ptr<InstanceSpecification>();
}
shared_ptr<Zone> Environment::getMainZone()
{
	return _mainZone;
}
void Environment::setMainZone(shared_ptr<Zone> zone)
{
	_mainZone=zone;
}
}
#endif
