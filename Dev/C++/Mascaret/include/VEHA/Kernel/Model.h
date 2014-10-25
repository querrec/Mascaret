#ifndef _v2_VEHA_Kernel_Model_H
#define _v2_VEHA_Kernel_Model_H

#include "Tools/veha_plateform.h"
#include "VEHA/Kernel/Element.h"
#include "Tools/utils.h"

class ModelLoader2;

namespace VEHA
{
using std::ostream;
class PrimitiveType;
class Package;
class Class;
class Signal;
class Property;
class Operation;
class StateMachine;
class Region;
class Event;
class State;
class Activity;
class Environment;
class Element;

typedef struct Asso
{
  int id;
  string propid;
  string ownClassid;
} Asso;

typedef struct Evt
{
   shared_ptr<Event> event;
   shared_ptr<StateMachine> machine;
} Evt;

typedef struct ActivityMethod
{
   string operationName;
   shared_ptr<Activity> activity;
} ActivityMethod;

class VEHA_API Model : public Element
{
public :
	Model(string name);
	inline void setModelLoader(shared_ptr<ModelLoader2> modelLoader) {_modelLoader = modelLoader;}
	inline shared_ptr<ModelLoader2> getModelLoader() {return _modelLoader;}
	virtual ~Model();
	// --- Attributes ---
	// --- Relations ---
	//------ packages -----
protected :
	//		map<string, shared_ptr<Package> >	_packages;
	shared_ptr<Package> _package;
	map<string,shared_ptr<PrimitiveType> > _basicTypes;
	map<string,shared_ptr<Class> > _allClassesFullName;
	map<string,vector<shared_ptr<Class> > > _allClasses;
	void _addClassesFromPackage(shared_ptr<Package> p);
        vector<Asso> _assoToWrite;
        vector <Evt> _eventsToWrite;
        vector <ActivityMethod> _activityToWrite;
        int _cpt; 
	shared_ptr<ModelLoader2> _modelLoader;
        map<string, string> _ids;
        string _newID(string val);
        string _getID(string val);
public :
	//inline const map<string, shared_ptr<Package> > getPackages(void) { return _packages;}
	//inline shared_ptr<Package> getPackage(const string& name) { map<string, shared_ptr<Package> >::iterator it;
	//	if ((it=_packages.find(name))!=_packages.end())return it->second;
	//		else return shared_ptr<Package>();}
	//inline void addPackage(shared_ptr<Package> value);
	inline void setPackage(shared_ptr<Package> p) {_package=p;}
	inline shared_ptr<Package> getPackage() {return _package;}
	void registerAllClasses();
	inline const map<string,shared_ptr<Class> >& getFullnameClasses() const {return _allClassesFullName;}
	inline const map<string,vector<shared_ptr<Class> > >& getAllClasses() const {return _allClasses;}
	inline shared_ptr<Class> getClassByFullname(const string& name)const {return MAP_AT(_allClassesFullName,name);}
	inline vector<shared_ptr<Class> > getClassesByName(const string& name)const {return MAP_AT(_allClasses,name);}

	inline shared_ptr<PrimitiveType> getBasicType(const string& name)const 
        {
            map<string,shared_ptr<PrimitiveType> >::const_iterator it = _basicTypes.find(name);
            if (it == _basicTypes.end()) 
            {
               string type = "real";
               return MAP_AT(_basicTypes,type);
            }
            else
               return MAP_AT(_basicTypes,name);
        }
        inline bool isPrimitiveType(string name) {return _basicTypes.find(name) != _basicTypes.end();}
	void addBasicType(shared_ptr<PrimitiveType>);
	// --- Operations ---

	// void merge(shared_ptr<Model> model2add); PAS POUR TOUT DE SUITE
	bool toXMI(ostream& os); // generate XMI from actual model in memory
	bool toXMI2(ostream& os); // Re read XMI file and write it in stream
	bool writeToXMI2(ostream& os); // write To XMI2

protected :
	string _xmiurl;
public :
	inline void setUrl(string url) {_xmiurl = url;}
	inline string getUrl(void) {return _xmiurl;}

protected :
	void _writeHeader(ostream& os);
	void _writeHeader2(ostream& os);
	void _writeFooter(ostream& os);
	void _writeContent(ostream& os);
	void _writeModel(ostream& os);
	void _writeModel2(ostream& os);
	void _writePackage(shared_ptr<Package> package, ostream& os);
	void _writePackage2(shared_ptr<Package> package, ostream& os);
	void _writeClasse(shared_ptr<Class> classe, ostream& os);
	void _writeClasse2(shared_ptr<Class> classe, ostream& os);
	void _writeSignal2(shared_ptr<Signal> signal, ostream& os);
	void _writeProperty2(shared_ptr<Class> classe, shared_ptr<Property> property, ostream& os);
	void _writeOperation2(shared_ptr<Class> classe, shared_ptr<Operation> operation, ostream& os);
	void _writeAssociation2(ostream& os);
	void _writeEvent2(ostream& os);
	void _writeBehaviorStateMachine2(shared_ptr<Class> classe, shared_ptr<StateMachine> machine, ostream& os);
	void _writeStateMachineRegion2(shared_ptr<Class> classe, shared_ptr<StateMachine> machine, shared_ptr<Region> region, ostream& os);
    void _writeActivityFrom2(string type, shared_ptr<Class> classe, shared_ptr<StateMachine> machine, shared_ptr<State> state, shared_ptr<Operation> operation, ostream& os);
    void _writeActivity2(ostream& os, shared_ptr<Activity> activity, string operationName="");
	void _writeDataType(ostream& os);
	void _writeDataType2(ostream& os);
	void _writeStereoType(ostream& os);
	void _writeDocumentation(shared_ptr<Element> element, ostream& os);

public :

	void addEnvironment( shared_ptr<Environment> curEnv);
	inline map<string,shared_ptr<Environment> > getEnvironments(){return _environments;}

	//inline vector <shared_ptr<Environment> > getEnvironments(){return _environments;}
	//inline void addEnvironment(shared_ptr <Environment> env){_environments.push_back(env);}

protected :
	map<string,shared_ptr<Environment> > _environments;
	//vector<shared_ptr<Environment> > _environments;
};

}
#endif
