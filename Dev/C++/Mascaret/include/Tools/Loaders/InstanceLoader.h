#ifndef INSTANCELOADER_H
#define INSTANCELOADER_H

#include <string>

#include "VEHA/Kernel/Environment.h"
#include "VEHA/Kernel/Class.h"
#include "VEHA/Kernel/Package.h"
#include "VEHA/Entity/Entity.h"
#include "HAVE/VirtualHuman.h"
#include "BEHAVE/AgentPlateform.h"
#include "BEHAVE/OrganisationalEntity.h"

using namespace VEHA;
using namespace HAVE;
using namespace BEHAVE;

class XmlNode;

class VEHA_API InstanceLoader
{
public :
	InstanceLoader(shared_ptr<AgentPlateform> agentPlateform,shared_ptr<Environment> environment, string filename, bool toRun = true);
	
void parseInstances(shared_ptr<AgentPlateform> agentPlateform, shared_ptr<Environment> environment, string filename, bool toRun= true);

protected:
	string _basedir;
	shared_ptr<Environment> _environment;
	shared_ptr<Model> _model;
	shared_ptr<AgentPlateform> _agentPlateform;
	bool _toRun;
	//   VEHAApplication * _application;

	void _parseInstances(shared_ptr<XmlNode> node);
	shared_ptr<VEHA::Entity> _createEntity(shared_ptr<XmlNode> node, shared_ptr<VEHA::Entity> parent=shared_ptr<VEHA::Entity>());
	shared_ptr<VirtualHuman> _createHuman(shared_ptr<XmlNode> node);
	shared_ptr<OrganisationalEntity> _createOrganisation(shared_ptr<XmlNode> node);
	void _readEntity(shared_ptr<XmlNode> node, shared_ptr<VEHA::Entity> entity);
	void _readGeometryEntity(shared_ptr<XmlNode> node, shared_ptr<VEHA::Entity> entity);
	shared_ptr<Class>  _findClass(const string& className);
	shared_ptr<Class>  _findClassPackage(shared_ptr<Package> package, const string& className);
	void _addRelations();

	shared_ptr<Environment> parseEnvironment(string url);
	shared_ptr<Model> parseModel(string url);

	bool _initializeStateMachine(shared_ptr<XmlNode> node, shared_ptr<Entity> entity);

        static int _id;

	struct Relation
	{
		shared_ptr<VEHA::Entity> e;
		string property;
		string value;
	};
	vector<Relation> _relations;
	//   void _createInstance(Class * classe, InstanceSpecification * instance);

	//   vector <Entity *> _entities;
};
#endif
