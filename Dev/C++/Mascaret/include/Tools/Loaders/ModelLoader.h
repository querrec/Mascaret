#ifndef MODELLOADER_H
#define MODELLOADER_H

#include "Tools/Loaders/XMIParser.h"
#include "VEHA/Kernel/Model.h"
#include "VEHA/Kernel/Class.h"
#include "VEHA/Kernel/Package.h"
#include "VEHA/Kernel/PrimitiveType.h"
#include "VEHA/Kernel/Parameter.h"
#include "VEHA/Behavior/Common/Trigger.h"
#include "VEHA/Behavior/Common/Signal.h"

#include "BEHAVE/OrganisationalStructure.h"
#include "BEHAVE/RoleClass.h"
#include "BEHAVE/Role.h"
#include "BEHAVE/Procedure.h"
#include "BEHAVE/Ressource.h"

#include <string>
#include <map>
#include <vector>

using namespace VEHA;
using namespace BEHAVE;

class VEHA_API ModelLoader
{
public :

	ModelLoader(shared_ptr<Model> model, string str, bool strIsFileName=true);
	ModelLoader(shared_ptr<Model> model, shared_ptr<XmlNode> xmiRootNode);

	shared_ptr<PrimitiveType> getDataType(string dataTypeRef);

	inline map< string, shared_ptr<OrganisationalStructure> > getOrganisationalStructure(){return _organisations;}

	void reload(shared_ptr<Class> classe);

protected :
	void _addDataTypes(shared_ptr<XMIPackage> package);
	void _addStereotypes(shared_ptr<XMIPackage> package);
	shared_ptr<XMIPackage> _getInitialPackage();

	shared_ptr<Package> _addClassesFromPackage(shared_ptr<XMIPackage> package);

	void _reloadClasse(shared_ptr<Class> classe, shared_ptr<XMIClasse> xmiModelClasse);

	void _parseClasse(shared_ptr<Class> classe, shared_ptr<XMIClasse> xmiModelClasse);
	void _parseSignal(shared_ptr<Signal>  signal, shared_ptr<XMISignal> xmiModelSignal);
	void _reloadAttributes(shared_ptr<Class>  classe, shared_ptr<XMIClasse> xmiModelClasse);
	void _addAttributes(shared_ptr<Class>  classe, shared_ptr<XMIClasse> xmiModelClasse);
	void _addAssociations(shared_ptr<XMIPackage> xmiPackage);
	void _addStateMachines(shared_ptr<XMIPackage> xmiPackage);
	void _addActivities(shared_ptr<XMIPackage> xmiPackage);
	void _reloadActivities(shared_ptr<XMIPackage> xmiPackage, shared_ptr<Operation> oper, string id);
	void _addGeneralisations(void);
	void _addOperations(shared_ptr<Class> classe, shared_ptr<XMIClasse> xmiModelClasse);
	void _reloadOperations(shared_ptr<Class> classe, shared_ptr<XMIClasse> xmiModelClasse);
	void _addEvents(shared_ptr<Class> classe, shared_ptr<XMIClasse> xmiModelClasse);
	void _addParameters(shared_ptr<Operation> operation, shared_ptr<XMIOperation> xmiOperation);
	void _reloadParameters(shared_ptr<Operation> operation, shared_ptr<XMIOperation> xmiOperation);
	void _setParametersType(void);

	void _addTeamsFromPackage(shared_ptr<XMIPackage> package);

	string _parseToDo(const string& toDo);
	string _parseCommandeName(const string& toDo);
	string _readValue(const string & command, int index);
	void _parsePlayAnimationCommand (const string& command, string & animName, string &ressourceName, double &speed, int &sens);

private :
	shared_ptr<XMIParser> _xmiParser;

	map<string, shared_ptr<PrimitiveType> > _basicTypes;
	map<string, string> _sBasicTypes;
	map<string, string> _stereotypes;

	map < string, shared_ptr<Package> > _packages;
	map < string, shared_ptr<Class> > _classes;
	map < string, shared_ptr<Signal> > _signals;
	map < string, shared_ptr<Classifier> > _classifiers;
	map < string, shared_ptr<Parameter> > _parameters;
	map < string, shared_ptr<Operation> > _operationType;
	map < string, string > _paramToType;
	vector < pair <string, string> > _generalisations;
	vector< shared_ptr<XMIClasse> > _xmiClasses;
	map < string, shared_ptr<Trigger> > _idEvents;
	map < string, shared_ptr<Operation> > _idOperations;

	vector < pair <string, string> > _opaqueBehaviorsCode;

	map < string, shared_ptr<OrganisationalStructure> > _organisations;
	map < string, shared_ptr<RoleClass> > _roleClasses;

	shared_ptr<Model> _model;// only to share the model between functions during parse;

};
#endif                           // Attribute
