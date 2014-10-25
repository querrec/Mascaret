#include "VEHA/Kernel/Model.h"
#include "VEHA/Kernel/Environment.h"
#include "VEHA/Kernel/Package.h"
#include "VEHA/Kernel/PrimitiveType.h"
#include "VEHA/Kernel/Class.h"
#include "VEHA/Kernel/Property.h"
#include "VEHA/Kernel/Operation.h"
#include "VEHA/Kernel/Parameter.h"
#include "VEHA/Behavior/StateMachine/StateMachine.h"
#include "VEHA/Behavior/StateMachine/Region.h"
#include "VEHA/Behavior/StateMachine/Vertex.h"
#include "VEHA/Behavior/StateMachine/State.h"
#include "VEHA/Behavior/StateMachine/PseudoState.h"
#include "VEHA/Behavior/StateMachine/Transition.h"
#include "VEHA/Behavior/StateMachine/SignalEvent.h"
#include "VEHA/Behavior/StateMachine/CallEvent.h"
#include "VEHA/Behavior/Activity/Activity.h"
#include "VEHA/Behavior/Activity/ActionNode.h"
#include "VEHA/Behavior/Activity/ObjectNode.h"
#include "VEHA/Behavior/Activity/ActivityEdge.h"
#include "VEHA/Behavior/Activity/ActivityPartition.h"
#include "VEHA/Behavior/BasicActions/Action.h"
#include "VEHA/Behavior/BasicActions/CallOperationAction.h"
#include "VEHA/Behavior/BasicActions/SendSignalAction.h"
#include "VEHA/Behavior/BasicActions/PlayAnimationAction.h"
#include "VEHA/Behavior/Common/Trigger.h"
#include "VEHA/Behavior/Common/Event.h"
#include "VEHA/Behavior/Common/Signal.h"
#include "Tools/Loaders/XmlParser.h"
#include <boost/foreach.hpp>

#include <sstream>

namespace VEHA
{

Model::Model(string name):Element(name)
{
	_basicTypes["boolean"]=shared_ptr<PrimitiveType>(new PrimitiveType("boolean"));
	_basicTypes["integer"]=shared_ptr<PrimitiveType>(new PrimitiveType("integer"));
	_basicTypes["real"]=shared_ptr<PrimitiveType>(new PrimitiveType("real"));
	_basicTypes["float"]=shared_ptr<PrimitiveType>(new PrimitiveType("real"));
	_basicTypes["string"]=shared_ptr<PrimitiveType>(new PrimitiveType("string"));

	_basicTypes["undefined"]=shared_ptr<PrimitiveType>(new PrimitiveType("undefined"));

	_basicTypes["char"]=shared_ptr<PrimitiveType>(new PrimitiveType("char"));
	_basicTypes["shape"]=shared_ptr<PrimitiveType>(new PrimitiveType("shape"));
	_basicTypes["point"]=shared_ptr<PrimitiveType>(new PrimitiveType("point"));
	_basicTypes["path"]=shared_ptr<PrimitiveType>(new PrimitiveType("path"));
	_basicTypes["sound"]=shared_ptr<PrimitiveType>(new PrimitiveType("sound"));
	_basicTypes["animation"]=shared_ptr<PrimitiveType>(new PrimitiveType("animation"));

	_basicTypes["vector3"]=shared_ptr<PrimitiveType>(new PrimitiveType("vector3"));
	_basicTypes["rotation"]=shared_ptr<PrimitiveType>(new PrimitiveType("rotation"));
	_basicTypes["color"]=shared_ptr<PrimitiveType>(new PrimitiveType("color"));
	// pas utiliser pour l'instant
	_basicTypes["quaternion"]=shared_ptr<PrimitiveType>(new PrimitiveType("quaternion"));

}

void Model::addEnvironment( shared_ptr<Environment> curEnv)
{
	_environments[curEnv->getUrl()]=curEnv;
}

Model::~Model()
{
}
void Model::addBasicType(shared_ptr<PrimitiveType> type)
{
cerr << type->getName() << endl;
	_basicTypes.insert(make_pair(type->getName(),type));
}
void Model::registerAllClasses()
{
	if (!_package)
		return;
	_allClassesFullName.clear();
	_allClasses.clear();
	_addClassesFromPackage(_package);
}
void Model::_addClassesFromPackage(shared_ptr<Package> p)
{
	BOOST_FOREACH(shared_ptr<Class> cl,p->getClasses())
	{
		_allClassesFullName.insert(make_pair(cl->getFullName(),cl));
		_allClasses[cl->getName()].push_back(cl);
    }
	BOOST_FOREACH(shared_ptr<Package> p2,p->getPackages())
		_addClassesFromPackage(p2);

}

bool Model::toXMI2(ostream & os)
{
	shared_ptr<XmlParser> parser = shared_ptr<XmlParser>(new XmlParser());
        cerr << " ****** " << _xmiurl << endl;
	parser->parseFile(_xmiurl);
	os << parser->writeString();
	return true;
}

bool Model::toXMI(ostream & os)
{
	_writeHeader(os);
	_writeContent(os);
	_writeFooter(os);
	return true;
}

void Model::_writeHeader(ostream & os)
{
	os << "<XMI xmi.version = '1.1' xmlns:UML='//org.omg/UML/1.4'>" << endl;
	os << "<XMI.header> <XMI.documentation> <XMI.exporter>Mascaret Xporter</XMI.exporter> <XMI.exporterVersion>1</XMI.exporterVersion> </XMI.documentation> </XMI.header>" << endl;
}

void Model::_writeFooter(ostream & os)
{
	os << "</XMI>" << endl;
}

void Model::_writeContent(ostream & os)
{
	os << "<XMI.content>" << endl;

	_writeModel(os);

	os << "</XMI.content>" << endl;
}

void Model::_writeModel(ostream & os)
{
	shared_ptr<Package> rootPackage = getPackage();
	os << "<UML:Model xmi.id= \'model\'>" << endl;

	os << "<UML:ModelElement.Name>";
	os << rootPackage->getName();
	os << "</UML:ModelElement.Name>" << endl;

	os << "<UML:ModelElement.visibility xmi.value = 'public'/>"<<endl;
	os << "<UML:GeneralizableElement.isRoot xmi.value = 'false'/>"<<endl;
	os << "<UML:GeneralizableElement.isLeaf xmi.value = 'false'/>"<<endl;
	os << "<UML:GeneralizableElement.isAbstract xmi.value = 'false'/>"<<endl;

	//   _writeDocumentation(rootPackage, os);

	os << "<UML:Namespace.ownedElement>" << endl;

	_writePackage(rootPackage, os);

	_writeStereoType(os);
	_writeDataType(os);

	os << "</UML:Namespace.ownedElement>" << endl;

	os << "</UML:Model>" << endl;
}

void Model::_writePackage(shared_ptr<Package> package, ostream & os)
{
	os << "<UML:Package xmi.id= 'package_" << package->getName() << "'>" << endl;

	os << "<UML:ModelElement.Name>";
	os << package->getName();
	os << "</UML:ModelElement.Name>" << endl;

	os << "<UML:ModelElement.visibility xmi.value = 'public'/>"<<endl;
	os << "<UML:GeneralizableElement.isRoot xmi.value = 'false'/>"<<endl;
	os << "<UML:GeneralizableElement.isLeaf xmi.value = 'false'/>"<<endl;
	os << "<UML:GeneralizableElement.isAbstract xmi.value = 'false'/>"<<endl;

	_writeDocumentation(package, os);

	os << "<UML:Namespace.ownedElement>" << endl;

	// ------- packages ... --------
	vector< shared_ptr<Package> > packages = package->getPackages();
	for (unsigned int i =0; i <  packages.size(); i++)
		_writePackage(packages[i], os);

	// ------- classes ... --------
	vector< shared_ptr<Class> > classes = package->getClasses();
	for (unsigned int i =0; i <  classes.size(); i++)
		_writeClasse(classes[i], os);


	os << "</UML:Namespace.ownedElement>" << endl;

	os << "</UML:Package>" << endl;
}

void Model::_writeClasse(shared_ptr<Class> classe, ostream & os)
{
	os << "<UML:Class xmi.id= '" << classe->getFullName() << "'>" << endl;

	os << "<UML:ModelElement.Name>";
	os << classe->getName();
	os << "</UML:ModelElement.Name>" << endl;

	os << "<UML:ModelElement.visibility xmi.value = 'public'/>"<<endl;
	os << "<UML:GeneralizableElement.isRoot xmi.value = 'false'/>"<<endl;
	os << "<UML:GeneralizableElement.isLeaf xmi.value = 'false'/>"<<endl;
	os << "<UML:GeneralizableElement.isAbstract xmi.value = 'false'/>"<<endl;
	os << "<UML:Class.isActive xmi.value = 'false'/>"<<endl;

	_writeDocumentation(classe, os);

	os << "<UML:Namespace.ownedElement>" << endl;

	os << "</UML:Namespace.ownedElement>" << endl;

	os << "<UML:Classifier.feature>" << endl;

	// -------- Operations ... ----------

	os << "</UML:Classifier.feature>" << endl;

	os << "</UML:Class>" << endl;
}

void Model::_writeDocumentation(shared_ptr<Element> element, ostream & os)
{
	os << "<UML:ModelElement.taggedValue>"<<endl;
	os << "<UML:TaggedValue xmi.id = 'a1437340096-2'>"<<endl;
	os << "<UML:TaggedValue.type>"<<endl;
	os << "<UML:TagDefinition>"<<endl;
	os << "<UML:TagDefinition.tagType>summary</UML:TagDefinition.tagType>"<<endl;
	os << "</UML:TagDefinition>"<<endl;
	os << "</UML:TaggedValue.type>"<<endl;
	os << "<UML:TaggedValue.dataValue>";
	os << element->getSummary() ;
	os << "</UML:TaggedValue.dataValue>"<<endl;
	os << "</UML:TaggedValue>"<<endl;

	os << "<UML:TaggedValue xmi.id = 'a1437340096-3'>"<<endl;
	os << "<UML:TaggedValue.type>"<<endl;
	os << "<UML:TagDefinition>"<<endl;
	os << "<UML:TagDefinition.tagType>documentation</UML:TagDefinition.tagType>"<<endl;
	os << "</UML:TagDefinition>"<<endl;
	os << "</UML:TaggedValue.type>"<<endl;
	os << "<UML:TaggedValue.dataValue>";
	os << element->getDescription() ;
	os << "</UML:TaggedValue.dataValue>"<<endl;
	os << "</UML:TaggedValue>"<<endl;
	os << "</UML:ModelElement.taggedValue>"<<endl;
}

void Model::_writeDataType(ostream & os)
{
	map<string,shared_ptr<PrimitiveType> >::iterator it;

	for (it = _basicTypes.begin(); it != _basicTypes.end(); it++)
	{
		os << "<UML:DataType xmi.id = 'bt_" << it->second->getName() << "'>" << endl;
		os << "<UML:ModelElement.name>";
		os << it->second->getName();
		os << "</UML:ModelElement.name>" << endl;
		os << "<UML:ModelElement.visibility xmi.value = 'public'/>" << endl;
		os << "<UML:GeneralizableElement.isRoot xmi.value = 'false'/>" << endl;
		os << "<UML:GeneralizableElement.isLeaf xmi.value = 'false'/>" << endl;
		os << "<UML:GeneralizableElement.isAbstract xmi.value = 'false'/>" << endl;
		os << "</UML:DataType>" << endl;

	}
}

void Model::_writeStereoType(ostream & os)
{
	os << "<UML:Stereotype xmi.id = 'a1437341328-5'>" << endl;
	os << "<UML:ModelElement.name>Entity</UML:ModelElement.name>" << endl;
	os << "<UML:Stereotype.icon></UML:Stereotype.icon>" << endl;
	os << "<UML:Stereotype.baseClass>Class</UML:Stereotype.baseClass>" << endl;
	os << "</UML:Stereotype>" << endl;
	os << "<UML:Stereotype xmi.id = 'a1437341328-9'>" << endl;
	os << "<UML:ModelElement.name>VirtualHuman</UML:ModelElement.name>" << endl;
	os << "<UML:Stereotype.icon></UML:Stereotype.icon>" << endl;
	os << "<UML:Stereotype.baseClass>Class</UML:Stereotype.baseClass>" << endl;
	os << "</UML:Stereotype>" << endl;
	os << "<UML:Stereotype xmi.id = 'a1437341328-145'>" << endl;
	os << "<UML:ModelElement.name>Team</UML:ModelElement.name>" << endl;
	os << "<UML:Stereotype.icon></UML:Stereotype.icon>" << endl;
	os << "<UML:Stereotype.baseClass>Package</UML:Stereotype.baseClass>" << endl;
	os << "</UML:Stereotype>" << endl;
	os << "<UML:Stereotype xmi.id = 'a1437341328-150'>" << endl;
	os << "<UML:ModelElement.name>RoleClass</UML:ModelElement.name>" << endl;
	os << "<UML:Stereotype.icon></UML:Stereotype.icon>" << endl;
	os << "<UML:Stereotype.baseClass>Class</UML:Stereotype.baseClass>" << endl;
	os << "</UML:Stereotype>" << endl;
	os << "<UML:Stereotype xmi.id = 'a1437341328-148'>" << endl;
	os << "<UML:ModelElement.name>Procedure</UML:ModelElement.name>" << endl;
	os << "<UML:Stereotype.icon></UML:Stereotype.icon>" << endl;
	os << "<UML:Stereotype.baseClass>ActivityGraph</UML:Stereotype.baseClass>" << endl;
	os << "</UML:Stereotype>" << endl;

}
bool Model::writeToXMI2(ostream & os)
{
   _cpt = 0;
   _writeHeader2(os);
   _writeModel2(os);

   os << endl;

   return true;
}

void Model::_writeHeader2(ostream & os)
{
   os << "<?xml version=\"1.0\" encoding=\"UTF-8\"?> " << endl;
}

void Model::_writeModel2(ostream & os)
{
   shared_ptr<Package> rootPackage = getPackage();
   os << "<uml:Model xmlns:uml=\"http://schema.omg.org/spec/UML/2.1.1\" "; 
   os << "xmlns:xmi=\"http://schema.omg.org/spec/XMI/2.1\"  ";
   os << "xmi:version=\"2.1\" ";
   os << "xmi:id=\"" << _newID(rootPackage->getName()) << "\" " ;
   os << "name=\"" << rootPackage->getName() << "\"> " << endl;

   // Comment 
   os << "   <ownedComment xmi:type=\"uml:Comment\" ";
   os << "xmi:id=\"" << rootPackage->getName()<<"_comment_id" << "\">" << endl;
   os << "      <body>" << rootPackage->getDescription() <<"</body>"<< endl;
   os << "   </ownedComment>" << endl;

   // ------- packages ... --------
   vector< shared_ptr<Package> > packages = rootPackage->getPackages();
   for (unsigned int i =0; i <  packages.size(); i++)
      _writePackage2(packages[i], os);


   // Primitive types
   _writeDataType2(os);

   os << "</uml:Model>" << endl;
}

string Model::_newID(string val)
{
   map<string, string>::iterator it = _ids.find(val);
   if (it !=_ids.end()) return it->second;
   else 
   {
      std::ostringstream oss;
      oss << "Element_" << _cpt++;
      string id = oss.str();

      _ids[val] = id;
      return id;
   }
}

string Model::_getID(string val)
{
   map<string, string>::iterator it = _ids.find(val);
   if (it !=_ids.end()) return it->second;
   else 
      return _newID(val);

}

void Model::_writeDataType2(ostream & os)
{
   map<string,shared_ptr<PrimitiveType> >::iterator it;

   for (it = _basicTypes.begin(); it != _basicTypes.end(); it++)
   {
      string name = it->second->getName() ;
      if (name != "real" && 
          name != "string" && 
          name != "integer" && 
          name != "char" && 
          name != "boolean" )
      {
         os << "<packagedElement xmi:type=\"uml:PrimitiveType\" ";
         os << "xmi:id=\"" << _newID(it->second->getName()) << "\" ";
         os << "name=\"" << it->second->getName() << "\"/>" << endl;
      }
   }

}

void Model::_writePackage2(shared_ptr<Package> pkg, ostream & os)
{
   os << "   <packagedElement xmi:type=\"uml:Package\" ";
   os << "xmi:id=\"" << _newID(pkg->getFullName()) << "\" ";
   os << "name=\"" << pkg->getName() << "\">" << endl;

   // Comment 
   os << "   <ownedComment xmi:type=\"uml:Comment\" ";
   os << "xmi:id=\"" << pkg->getName()<<"_comment_id" << "\">" << endl;
   os << "      <body>" << pkg->getDescription() <<"</body>"<< endl;
   os << "   </ownedComment>" << endl;

   // ------- packages ... --------
   vector< shared_ptr<Package> > packages = pkg->getPackages();
   for (unsigned int i =0; i <  packages.size(); i++)
      _writePackage2(packages[i], os);


   // ------- classes ... --------
   vector< shared_ptr<Class> > classes = pkg->getClasses();
   for (unsigned int iC =0; iC <  classes.size(); iC++)
   {
      _assoToWrite.clear();
      _eventsToWrite.clear();
      _writeClasse2(classes[iC], os);
      // Associations
      _writeAssociation2(os);
      _writeEvent2(os);
   }

   // ------- Signals ... --------
   vector< shared_ptr<Signal> > signals = pkg->getSignals();
   for (unsigned int iS =0; iS <  signals.size(); iS++)
   {
      _writeSignal2(signals[iS], os);
   }

   os << "   </packagedElement>" << endl;
}

void Model::_writeSignal2(shared_ptr<Signal> signal, ostream & os)
{
   os << "<packagedElement xmi:type=\"uml:Signal\" "; 
   os << "xmi:id=\"" << _newID(signal->getFullName()) << "\" ";
   os << "name=\"" << signal->getName() << "\"/>" << endl;
}

void Model::_writeClasse2(shared_ptr<Class> classe, ostream & os)
{
   os << "<packagedElement xmi:type=\"uml:Class\" "; 
   os << "xmi:id=\"" << _newID(classe->getFullName()) << "\" ";
   os << "name=\"" << classe->getName() << "\">" << endl;

   // Comment 
   os << "   <ownedComment xmi:type=\"uml:Comment\" ";
   os << "xmi:id=\"" << classe->getName()<<"_comment_id" << "\">" << endl;
   os << "      <body>" << classe->getDescription() <<"</body>"<< endl;
   os << "   </ownedComment>" << endl;

   // Attributes
   map< string, shared_ptr<Property> > properties = classe->getAttributes();
   map< string, shared_ptr<Property> >::iterator itA;
   for (itA=properties.begin(); itA != properties.end(); itA++)
      _writeProperty2(classe, itA->second, os);

   // Operation
   map< string, shared_ptr<Operation> > operations = classe->getOperations();
   map< string, shared_ptr<Operation> >::iterator itO;
   for (itO=operations.begin(); itO != operations.end(); itO++)
      _writeOperation2(classe, itO->second, os);

   // Generalisation
   vector<shared_ptr<Classifier> > parents = classe->getParents();
   for (unsigned int iP=0; iP < parents.size() ; iP++)
   {
      os << "<generalization xmi:type=\"uml:Generalization\" ";
      os << "xmi:id=\"" << _newID("gen"+classe->getFullName()) << "\" "; 
      os << "general=\"" << _getID(parents[iP]->getFullName()) << "\"/>" << endl;
   }

   // Behaviors : StateMachines
   map<string, shared_ptr<Behavior> > behaviors = classe->getOwnedBehavior(); 
   map<string, shared_ptr<Behavior> >::iterator itB;
   for (itB=behaviors.begin(); itB != behaviors.end(); itB++)
   {
      shared_ptr<StateMachine> sm = shared_dynamic_cast<StateMachine>(itB->second);
      if (sm)
         _writeBehaviorStateMachine2(classe, sm, os);
   }

   // Behaviors : Activities
   for (unsigned int iAct = 0; iAct < _activityToWrite.size(); iAct++)
   {
      _writeActivity2(os, _activityToWrite[iAct].activity, _activityToWrite[iAct].operationName);      
   }
   _activityToWrite.clear();

   os << "</packagedElement>" << endl; 
}

void Model::_writeBehaviorStateMachine2(shared_ptr<Class> classe, shared_ptr<StateMachine> machine, ostream & os)
{
   os << "<ownedBehavior xmi:type=\"uml:StateMachine\" ";
   os << "xmi:id=\"" << _newID(classe->getFullName()+"_"+machine->getName()) << "\" ";
   os << "name=\"" << machine->getName() << "\">" << endl;

   // Regions
   vector< shared_ptr<Region> > regions = machine->getRegion();
   for (unsigned int iR= 0; iR < regions.size() ; iR++)
   {
      _writeStateMachineRegion2(classe,machine,regions[iR],os);
   }

   os << "</ownedBehavior>" << endl;
}

void Model::_writeStateMachineRegion2(shared_ptr<Class> classe, shared_ptr<StateMachine> machine, shared_ptr<Region> region, ostream& os)
{
   os << "<region xmi:id=\""<< _newID(classe->getFullName()+"_"+machine->getName()+"region") << "\" ";
   os << "name=\"Region\">" << endl;


   // SubVertice
   vector< shared_ptr<Vertex> > vertices = region->getVertices();
   for (unsigned int iV = 0; iV < vertices.size() ; iV++)
   {
      shared_ptr<PseudoState> ps = shared_dynamic_cast<PseudoState>(vertices[iV]);
      if (ps)
      {
         os << "<subvertex xmi:type=\"uml:Pseudostate\" ";
         os << "xmi:id=\"" << _newID(classe->getFullName()+"_"+machine->getName()+"_"+ps->getName()) << "\" ";
         os << "name=\"" << ps->getName() << "\" ";
         PseudoStateKind kind = ps->getKind();
         //if (kind == INITIAL) os << "kind=\"initial\"/>" << endl;
         if (kind == INITIAL) os << "/>" << endl;
         else if (kind == DEEPHISTORY) os << "kind=\"deepHistory\"/>" << endl;
         else if (kind == SHALLOWHISTORY) os << "kind=\"shallowHistory\"/>" << endl;
         else if (kind == JOIN) os << "kind=\"join\"/>" << endl;
         else if (kind == FORK) os << "kind=\"fork\"/>" << endl;
         else if (kind == JUNCTION) os << "kind=\"junction\"/>" << endl;
         else if (kind == CHOICE) os << "kind=\"choice\"/>" << endl;
         else if (kind == ENTRYPOINT) os << "kind=\"entryPoint\"/>" << endl;
         else if (kind == EXITPOINT) os << "kind=\"exitPoint\"/>" << endl;
         else if (kind == TERMINATE) os << "kind=\"terminate\"/>" << endl;
         else os << "/>" << endl;
      }
      else
      {
         shared_ptr<State> state = shared_dynamic_cast<State> (vertices[iV]);
         os << "<subvertex xmi:type=\"uml:State\" ";
         os << "xmi:id=\"" << _newID(classe->getFullName()+"_"+machine->getName()+"_"+state->getName()) << "\" ";
         os << "name=\"" << state->getName() << "\">" << endl;

         shared_ptr<Operation> doBehavior = state->getDoBehavior();
         shared_ptr<Operation> entryBehavior = state->getEntryBehavior();
         shared_ptr<Operation> exitBehavior = state->getExitBehavior();
         if (doBehavior) 
            _writeActivityFrom2("doActivity", classe, machine, state, doBehavior, os);
         if (entryBehavior) 
            _writeActivityFrom2("entry", classe, machine, state, entryBehavior, os);
         if (exitBehavior) 
            _writeActivityFrom2("exit", classe, machine, state, exitBehavior, os);

         os << "</subvertex>" << endl;
      }
   }

   // Transitions
   vector< shared_ptr<Transition> > transitions = region->getTransition();
   for (unsigned int iT = 0; iT < transitions.size() ; iT++)
   {
      if (transitions[iT]->getTarget() && transitions[iT]->getTarget())
      { 

      // Transition source and target
      os << "<transition xmi:type=\"uml:Transition\" ";
      std::ostringstream transID;
      transID << machine->getFullName()+"_"+transitions[iT]->getName() << iT;
      os << "xmi:id=\"" << _newID(transID.str()) << "\" ";

      os << "target=\"" << _getID(classe->getFullName()+"_"+machine->getName()+"_"+transitions[iT]->getTarget()->getName()) << "\" ";
      os << "source=\"" << _getID(classe->getFullName()+"_"+machine->getName()+"_"+transitions[iT]->getSource()->getName()) << "\">" << endl;

      // Effect 
      shared_ptr<Action> effect = transitions[iT]->getEffect();
      if (effect) 
      {
         os << "<effect xmi:type=\"uml:OpaqueBehavior\" ";
         os << "xmi:id=\"" << _newID(machine->getFullName()+"_"+transitions[iT]->getName()+"_effect") << "\" "; 
         string idSpecification;
         if ( effect->getKind() == "CallOperation" )
         {
            shared_ptr<CallOperationAction> caction = shared_dynamic_cast<CallOperationAction>(effect);
            shared_ptr<Operation> oper = caction->getOperation();
            idSpecification = _getID(oper->getFullName());
         }
         else { cerr << "Transition effect TO DO" << endl;}
         os << "specification=\"" << idSpecification << "\"/>" << endl;
      }

      // Trigger
      vector<shared_ptr<Trigger> > triggers = transitions[iT]->getTrigger();
      if (triggers.size() > 0)
      {
         shared_ptr<Trigger> trigger = triggers[0];
         shared_ptr<Event> event = trigger->getEvent();
         os << "<trigger xmi:type=\"uml:Trigger\" " ;
         os << "xmi:id=\"" << _newID(classe->getFullName()+"_"+machine->getName()+"_"+transitions[iT]->getName()+"_trigger") << "\" ";
         os << "event=\"" << _newID(machine->getOwner()->getFullName()+"_"+machine->getName()+"_"+event->getName()+"_event")<< "\"/>" << endl;

         // Check if event not already added
         bool toAdd = true;
         for (unsigned int iEvt = 0; iEvt < _eventsToWrite.size(); iEvt++)
         {
            if (_eventsToWrite[iEvt].event == event) toAdd=false;
         }
         if (toAdd)
         {
            Evt evt;
            evt.event = event;
            evt.machine = machine;
            _eventsToWrite.push_back(evt);
         }
      }

      // Guard
      // TO DO

      os << "</transition>" << endl;
      }
   }

   os << "</region>" << endl;
}

void Model::_writeActivityFrom2(string type, shared_ptr<Class> classe, shared_ptr<StateMachine> machine, shared_ptr<State> state, shared_ptr<Operation> operation, ostream& os)
{
   string activityName = classe->getName() + "_" + machine->getName() +"_" + state->getName() + "_" + type;
   string partitionID = _newID(classe->getName() + "_" + machine->getName() +"_" + state->getName() + "_" + type + "_partition");
   string initNodeID = _newID(classe->getName() + "_" + machine->getName() +"_" + state->getName() + "_" + type + "_initNode");
   string operNodeID = _newID(classe->getName() + "_" + machine->getName() +"_" + state->getName() + "_" + type + "_" + operation->getName());
   string edgeID = _newID(classe->getName() + "_" + machine->getName() +"_" + state->getName() + "_" + type + "_" + operation->getName()+ "_edge");
   os << "<" << type << " xmi:type=\"uml:Activity\" ";
   os << "xmi:id=\"" << _newID(activityName) << "\" ";
   os << "name=\"" << activityName << "\">" << endl;

   os << "<node xmi:type=\"uml:InitialNode\" ";
   os << "xmi:id=\"" + initNodeID + "\" ";
   os << "name=\"InitialNode\" ";
   os << "outgoing=\"" + edgeID +"\" ";
   os << "inPartition=\"" + partitionID +"\"/>" << endl;

   os << "<node xmi:type=\"uml:CallOperationAction\" ";
   os << "xmi:id=\"" << operNodeID << "\" ";
   os << "name=\"" << operation->getName() << "_CallOperation" << "\" ";
   os << "incoming=\"" << edgeID << "\" ";
   os << "inPartition=\"" << partitionID << "\" ";
   os << "operation=\"" << _getID(operation->getFullName()) << "\"/>" << endl;

   os << "<edge xmi:type=\"uml:ControlFlow\" ";
   os << "xmi:id=\"" << edgeID << "\" ";
   os << "name=\"ControlFlow\" ";
   os << "source=\"" << initNodeID << "\" ";
   os << "target=\"" << operNodeID << "\">" << endl;
   os << "<weight xmi:type=\"uml:LiteralInteger\" ";
   os << "xmi:id=\"" << _newID(edgeID+"_weight") << "\" value=\"1\"/>" << endl;
   os << "</edge>" << endl;

   os << "<group xmi:type=\"uml:ActivityPartition\" ";
   os << "xmi:id=\"" << partitionID << "\" ";
   os << "name=\"this\" ";
   os << "node=\"" << initNodeID << " " << operNodeID << "\" ";
   os << "represents=\"" << _getID(classe->getFullName()) << "\"/>" << endl;

   os << "</" << type << ">" << endl;

}


void Model::_writeProperty2(shared_ptr<Class> classe, shared_ptr<Property> property, ostream & os)
{
   string fName = classe->getFullName() + "." + property->getName();

   if (property->getFullName() != fName) return;

   os << "<ownedAttribute xmi:type=\"uml:Property\" ";
   os << "xmi:id=\"" << _newID(property->getFullName()) << "\" ";
   os << "name=\"" << property->getName() << "\"" ;

   shared_ptr<Classifier> type = property->getType();
   string typeName = type->getName();
   if (typeName == "real" || typeName == "string" || typeName == "integer" || typeName == "char" || typeName == "boolean" )
   {
      os << ">" << endl;
      os << "<type xmi:type=\"uml:PrimitiveType\" href=\"http://schema.omg.org/spec/UML/2.1.1/uml.xml#" ;
      if (typeName == "real") os << "Double" ;
      if (typeName == "string") os << "String" ;
      if (typeName == "integer") os << "Integer" ;
      if (typeName == "char") os << "Char" ;
      if (typeName == "boolean") os << "Boolean" ;
      os << "\"/>" << endl; 
   }
   else 
   {
      shared_ptr<Class> classType = shared_dynamic_cast<Class>(type);
      if (!classType)
      {
        os << " type=\"" << _getID(typeName) << "\">" << endl;
      }
      else 
      {
        Asso asso;
        asso.id = _cpt++;
        asso.propid = _getID(property->getFullName());
        asso.ownClassid = _getID(classe->getFullName());
        _assoToWrite.push_back(asso);
        os << " type=\"" << _getID(classType->getFullName()) << "\" " ;
        os << "association=\"association_" << asso.id << "\">" << endl;
      }
   }
   
   os << "</ownedAttribute>" << endl;
}

void Model::_writeEvent2(ostream & os)
{
   for (unsigned int i = 0; i < _eventsToWrite.size(); i++)
   {
      shared_ptr<Event> event = _eventsToWrite[i].event;
      shared_ptr<StateMachine> machine = _eventsToWrite[i].machine; 
      shared_ptr<Classifier> cl = machine->getOwner();
      if (event->getType() == "SignalEvent")
      {
         shared_ptr<SignalEvent> sigEvent = shared_dynamic_cast<SignalEvent>(event);
         shared_ptr<InstanceSpecification> signal = sigEvent->getSignal();
         if (!signal)
            cerr << " Erreur signal not set : " << cl->getFullName() << " / " << sigEvent->getName() << endl;
         os << "<packagedElement xmi:type=\"uml:SignalEvent\" " ;
         os << "xmi:id=\"" << _getID(cl->getFullName()+"_"+machine->getName()+"_"+sigEvent->getName()+"_event") << "\" ";
         os << "name=\"" << sigEvent->getName()<< "\" ";
         if (signal) os << "signal=\""<< _getID(sigEvent->getSignal()->getFullName()) << "\"/>" << endl;
         else  os << "signal=\""<< "undef" << "\"/>" << endl;
      }
      else if (event->getType() == "CallEvent")
      {
         shared_ptr<CallEvent> callEvent = shared_dynamic_cast<CallEvent>(event);
         os << "<packagedElement xmi:type=\"uml:CallEvent\" " ;
         os << "xmi:id=\"" << _getID(cl->getFullName()+"_"+machine->getName()+"_"+callEvent->getName()+"_event") << "\" ";
         os << "name=\"" << callEvent->getName() << "\" ";
         os << "operation=\""<< _getID(callEvent->getOperation()->getFullName()) << "\"/>" << endl;
      }
      else cerr << " Events : TO DO " << endl;
   }

}

void Model::_writeAssociation2(ostream & os)
{
   for (unsigned int i = 0; i < _assoToWrite.size(); i++)
   {
      Asso asso = _assoToWrite[i];
      os << "<packagedElement xmi:type=\"uml:Association\" " ;
      os << "xmi:id=\"association_" << asso.id << "\" " ;
      os << "memberEnd=\"" << asso.propid << " " << asso.ownClassid << "_" << asso.id << "\">" << endl;
      os << "   <ownedEnd xmi:type=\"uml:Property\" ";
      os << "xmi:id=\"" << asso.ownClassid << "_" << asso.id << "\" " ;
      os << "type=\"" << asso.ownClassid << "\" ";
      os << "association=\"association_" << asso.id << "\"/>" << endl;
      os << "</packagedElement>" << endl;
   }
}

void Model::_writeOperation2(shared_ptr<Class> classe, shared_ptr<Operation> operation, ostream & os)
{
   string fName = classe->getFullName() + "." + operation->getName();

   if (fName != operation->getFullName() ) return;

   os << "<ownedOperation xmi:type=\"uml:Operation\" ";
   os << "xmi:id=\"" << _getID(operation->getFullName()) << "\" ";
   os << "name=\"" << operation->getName() << "\"";

   // Method
   shared_ptr<Behavior> behavior = operation->getMethod();
   if (behavior)
   {
      shared_ptr<Activity> actMethod = shared_dynamic_cast<Activity> (behavior);
      if (actMethod) 
      {
         ActivityMethod method;
         method.activity = actMethod;
         method.operationName = operation->getFullName();
         os << " method=\"" << _newID(operation->getFullName() + "_method") << "\"";
         os << ">" << endl;

         _activityToWrite.push_back(method);
      }
      else os << ">" <<endl;
   }
   else os << ">" <<endl;

   // Type de retour 
   shared_ptr<Classifier> returnType = operation->getType();
   if (returnType)
   {
      os << "<ownedParameter direction=\"return\"" << endl;
      string typeName = returnType->getName();
      if (typeName == "real" || typeName == "string" || typeName == "integer" || typeName == "char" || typeName == "boolean" )
      {
         os << ">" << endl;
         os << "<type xmi:type=\"uml:PrimitiveType\" href=\"http://schema.omg.org/spec/UML/2.1.1/uml.xml#" ;
         if (typeName == "real") os << "Double" ;
         if (typeName == "string") os << "String" ;
         if (typeName == "integer") os << "Integer" ;
         if (typeName == "char") os << "Char" ;
         if (typeName == "boolean") os << "Boolean" ;
         os << "\"/>" << endl; 
      }
      else 
      {
         shared_ptr<Class> classType = shared_dynamic_cast<Class>(returnType);
         if (!classType)
           os << " type=\"" << _getID(typeName) << "\">" << endl;
         else 
           os << " type=\"" << _getID(classType->getFullName()) << "\">" << endl;
      }
   }
   // Parameters
   vector< shared_ptr<Parameter> > parameters = operation->getParameter();
   for (unsigned int iP = 0; iP < parameters.size(); iP++)
   {
      shared_ptr<Parameter> parameter = parameters[iP];
      shared_ptr<Classifier> type = parameter->getType();
      if (type)
      {
         os << "<ownedParameter xmi:id=\""<< _newID(parameter->getName())<<"\" ";
         os << "name=\""<< parameter->getName()<<"\" ";
         os << "direction=\""<< parameter->getDirection()<<"\" ";
         string typeName = type->getName();
         if (typeName == "real" || typeName == "string" || typeName == "integer" || typeName == "char" || typeName == "boolean" )
         {
            os << ">" << endl;
            os << "<type xmi:type=\"uml:PrimitiveType\" href=\"http://schema.omg.org/spec/UML/2.1.1/uml.xml#" ;
            if (typeName == "real") os << "Double" ;
            if (typeName == "string") os << "String" ;
            if (typeName == "integer") os << "Integer" ;
            if (typeName == "char") os << "Char" ;
            if (typeName == "boolean") os << "Boolean" ;
            os << "\"/>" << endl; 
         }
         else 
         {
            shared_ptr<Class> classType = shared_dynamic_cast<Class>(type);
            if (!classType)
              os << " type=\"" << _getID(typeName) << "\">" << endl;
            else 
              os << " type=\"" << _getID(classType->getFullName()) << "\">" << endl;
         }
         os << "</ownedParameter>" << endl;
      }
   }
   
   os << "</ownedOperation>" << endl;
}

void Model::_writeActivity2(ostream& os, shared_ptr<Activity> activity, string operationName)
{

   map<shared_ptr<ActivityEdge>, string> edges;
   int cptEdge = 0;

   string baseID;
   if (operationName != "") 
      baseID = operationName+"_method";
   else baseID = activity->getFullName();

   os << "<ownedBehavior " << " xmi:type=\"uml:Activity\" ";
   os << "xmi:id=\"" <<  _newID(baseID) << "\" ";
   os << "name=\"" << activity->getName() << "\">" << endl;


   // Nodes 
   vector<shared_ptr<ActivityNode> > nodes = activity->getNode();
   for (unsigned int iN = 0; iN < nodes.size(); iN++)
   {
      bool isOpaque = false;
      shared_ptr<ActivityNode> node = nodes[iN];
      string nodeType = node->getKind(); // initial, action, object
      if (nodeType == "initial")
      {
         os << "<node xmi:type=\"uml:InitialNode\" ";
         os << "xmi:id=\"" + _newID(baseID + "_" +node->getName()) + "\" ";
         os << "name=\"InitialNode\" ";
      } 
      else if (nodeType == "object")
      {
         shared_ptr<ObjectNode> on = shared_dynamic_cast<ObjectNode>(node);
         os << "<node xmi:type=\"uml:CentralBufferNode\" ";
         os << "xmi:id=\"" + _newID(baseID +"_"+ node->getName()) + "\" ";
         os << "name=\"" + node->getName() << "\" ";
         if (on->getResourceType())
           os << "type=\"" + _getID(on->getResourceType()->getFullName()) << "\" ";
         else cerr << "ERROR : in " << baseID << " " << node->getName() << " has no resource type " << endl;
         os << "isControlType=\"true\" ";
      }
      else if (nodeType == "action")
      {
         shared_ptr<ActionNode> an = shared_dynamic_cast<ActionNode> (node);
         shared_ptr<Action> action = an->getAction();
         string kind = action->getKind();
         if (kind == "CallOperation")
         {
            shared_ptr<CallOperationAction> coa = shared_dynamic_cast<CallOperationAction> (action);
            os << "<node xmi:type=\"uml:CallOperationAction\" ";
            os << "xmi:id=\"" + _newID(baseID +"_"+ node->getName()) + "\" ";
cerr << baseID + "_" + node->getName() << endl;
            os << "name=\"" + node->getName() << "\" ";
            os << "operation=\"" + _getID(coa->getOperation()->getFullName()) << "\" ";
         }
         else if (kind=="SendSignal")
         {
            shared_ptr<SendSignalAction> ssa = shared_dynamic_cast<SendSignalAction> (action);
            os << "<node xmi:type=\"uml:SendSignalAction\" ";
            os << "xmi:id=\"" + _newID(baseID +"_"+ node->getName()) + "\" ";
cerr << baseID + "_" + node->getName() << endl;
            os << "name=\"" + node->getName() << "\" ";
            os << "signal=\"" + _getID(ssa->getSignal()->getFullName()) << "\" ";
         }
         else
         {
            isOpaque = true;
            os << "<node xmi:type=\"uml:OpaqueAction\" ";
            os << "xmi:id=\"" + _newID(baseID +"_"+ node->getName()) + "\" ";
cerr << baseID + "_" + node->getName() << endl;
            os << "name=\"" + node->getName() << "\" ";
         }
      }
 
      vector<shared_ptr<ActivityEdge> > incomings = node->getIncoming();
      if (incomings.size() != 0) os << "incoming=\"" ;
      for (unsigned int iI =0; iI < incomings.size(); iI++)
      {
         if (iI!=0) os << " " ;
         string idEdge;
         map < shared_ptr<ActivityEdge> , string>::iterator it = edges.find(incomings[iI]);
         if (it == edges.end())
         {
            std::ostringstream iEdge;
            iEdge << cptEdge++;
            idEdge = _newID(baseID+ "_edge_"+ iEdge.str()); 
            edges[incomings[iI]] = idEdge;
         }
         else idEdge = it->second;
         os << idEdge;
      }
      if (incomings.size() != 0) os << "\" " ;
  
      
      vector<shared_ptr<ActivityEdge> > outgoings = node->getOutgoing();
      if (outgoings.size() != 0) os << "outgoing=\"" ;
      for (unsigned int iO =0; iO < outgoings.size(); iO++)
      {
         if (iO!=0) os << " " ;
         string idEdge;
         map < shared_ptr<ActivityEdge> , string>::iterator it = edges.find(outgoings[iO]);
         if (it == edges.end())
         {
            std::ostringstream iEdge;
            iEdge << cptEdge++;
            idEdge = _newID(baseID+ "_edge_"+ iEdge.str()); 
            edges[outgoings[iO]] = idEdge;
         }
         else idEdge = it->second;
         os << idEdge;
      }
      if (outgoings.size() != 0) os << "\" " ;

      vector<shared_ptr<ActivityPartition> > partitions = node->getPartition();
      if (partitions.size() >0)
        os << "inPartition=\"" << _newID(baseID+"_partition_" + node->getPartition_at(0)->getName()) << "\"" ;

      os << ">" << endl;

      if (isOpaque)
      {
         os << "<body>" ;
         shared_ptr<ActionNode> an = shared_dynamic_cast<ActionNode> (node);
         shared_ptr<Action> action = an->getAction();
         string kind = action->getKind();
         if (kind == "PlayAnimation")
         {
            shared_ptr<PlayAnimationAction> anim = shared_dynamic_cast<PlayAnimationAction>(action);
            os << "PlayAnimation " << anim->getAnimationName() ;
         }
         os << "</body>" << endl;
      }

      os << "</node>" << endl;
   }

   // Edges 
   map < shared_ptr<ActivityEdge> , string >::iterator it;
   for (it = edges.begin(); it!=edges.end(); it++)
   {
      os << "<edge xmi:type=\"uml:ControlFlow\" ";
      os << "xmi:id=\"" << it->second << "\" ";
      os << "name=\"ControlFlow\" ";
      os << "source=\"" << _getID(baseID +"_"+ it->first->getSource()->getName()) << "\" ";
      os << "target=\"" << _getID(baseID +"_"+ it->first->getTarget()->getName()) << "\">" << endl;
      os << "<weight xmi:type=\"uml:LiteralInteger\" ";
      os << "xmi:id=\"" << _newID(it->second+"_weight") << "\" value=\"1\"/>" << endl;
      os << "</edge>" << endl;
   }

   // Partitions
   vector< shared_ptr<ActivityPartition> > partitions = activity->getPartition();
   for (unsigned iP = 0; iP < partitions.size(); iP++)
   {
      string partitionID = _getID(baseID+"_partition_" + partitions[iP]->getName());
      os << "<group xmi:type=\"uml:ActivityPartition\" ";
      os << "xmi:id=\"" << partitionID << "\" ";
      os << "name=\"" << partitions[iP]->getName() << "\" ";
      os << "node=\"" ;
      vector< shared_ptr<ActivityNode> > pnodes = partitions[iP]->getNode();
      for (unsigned ipnode = 0; ipnode < pnodes.size(); ipnode++)
      {
         if (ipnode >0) os << " " ;
         os << _getID(baseID + "_" + pnodes[ipnode]->getName()) ;
      }
      os << "\"/>";
      // TO DO os << "represents=\"" << _getID(activity->getOwner()->getFullName()) << "\"/>" << endl;
   }

   os << endl;
   os << "</ownedBehavior>" << endl;

}



#if 0 //PAS POUR TOUT DE SUITE
void Model::merge(shared_ptr<Model> model)
{
	if (!model->getPackage())return;
	_mergeClassesFromPackage(model->getPackage(),getPackage());

}
void Model::_mergeClassesFromPackage(shared_ptr<Package> p, shared_ptr<Package> ownP)
{
	map<string, shared_ptr<Package> > ownPmap;
	BOOST_FOREACH(shared_ptr<Package> p2,ownP->getPackages())
	ownPmap[p2->getName()]=p2;
	map<string, shared_ptr<Class> > ownCmap;
	BOOST_FOREACH(shared_ptr<Class> c2,ownP->getClasses())
	ownPmap[c2->getName()]=c2;

	BOOST_FOREACH(shared_ptr<Package> p2,p->getPackages())
	{
		map<string, shared_ptr<Package> >::iterator it;
		if ((it=ownPmap.find(p2->getName()))!=ownPmap.end())
			_mergeClassesFromPackage(p2,it->second);
		else _copyPackage(p2,ownP);
	}
	BOOST_FOREACH(shared_ptr<Class> c2,p->getClasses())
	{
		map<string, shared_ptr<Class> >::iterator it;
		if ((it=ownCmap.find(c2->getName()))!=ownCmap.end())
			_mergeClass(c2,it->second);
		else _copyClass(c2,ownP);
	}
}
void Package::_copyClass(shared_ptr<Class> c,shared_ptr<Package> ownP)
{

	shared_ptr<Class> newC=shared_ptr<Class>(new Class(c->getName()));
	ownP->addClass(newP);
}
void Package::_copyPackage(shared_ptr<Package> p,shared_ptr<Package> ownP) //COPy or MOVE?
{
	//	shared_ptr<Package> newP=shared_ptr<Package>(new Package(p->getName()));
	//	BOOST_FOREACH(shared_ptr<Package> p2,p->getPackages())
	//		_copyPackage(p2,newP);
	//	BOOST_FOREACH(shared_ptr<Class> c2,c->getClasses())
	//		_copyClass(c2,newP);
	ownP->addPackage(newP);
	//TODO:manque signal, et autres..;	
}

void Package::_mergeClass(shared_ptr<Class> toCopy,shared_ptr<Class> ownedClass)
{
}
#endif

}


