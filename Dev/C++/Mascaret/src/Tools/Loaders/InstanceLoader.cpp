#include "Tools/filesystem.h"
#include "Tools/Loaders/InstanceLoader.h"
#include "Tools/Loaders/ModelLoader2.h"
#include <iostream>
#include <sstream>

#include "Tools/Loaders/XmlParser.h"
#include "MascaretApplication.h"

#include "VEHA/Kernel/Model.h"
#include "VEHA/Kernel/Package.h"
#include "VEHA/Kernel/Slot.h"
#include "VEHA/Kernel/InstanceSpecification.h"
#include "VEHA/Kernel/Operation.h"
#include "VEHA/Kernel/Parameter.h"
#include "VEHA/Kernel/ValueSpecification.h"
#include "VEHA/Kernel/LiteralReal.h"
#include "VEHA/Kernel/LiteralInteger.h"
#include "VEHA/Kernel/LiteralString.h"
#include "VEHA/Kernel/Property.h"
#include "VEHA/Behavior/Common/Behavior.h"
#include "VEHA/Behavior/Activity/Activity.h"
#include "VEHA/Behavior/Activity/ActivityNode.h"
#include "VEHA/Behavior/StateMachine/StateMachineBehaviorExecution.h"
#include "VEHA/Behavior/StateMachine/Region.h"
#include "VEHA/Entity/ShapeSpecification.h"
#include "VEHA/Entity/AnimationSpecification.h"
#include "VEHA/Entity/SoundSpecification.h"
#include "VEHA/Entity/PointSpecification.h"
#include "VEHA/Entity/Vector3.h"
#include "VEHA/Entity/RotationVector.h"
#include "VEHA/Entity/EntityClass.h"
#include "VEHA/Entity/Zone.h"
#include "VEHA/Entity/VirtualRealityComponentFactory.h"
#include "VEHA/Behavior/Common/behaviorScheduler.h"

#include "HAVE/VirtualHumanClass.h"
#include "HAVE/Body.h"

#include "BEHAVE/OrganisationalStructure.h"
#include "BEHAVE/OrganisationalEntity.h"
#include "BEHAVE/RoleAssignement.h"
#include "BEHAVE/RessourceAssignement.h"
#include "BEHAVE/Ressource.h"
#include "BEHAVE/Procedure.h"
#include "BEHAVE/KnowledgeBase.h"
#include "VEHA/Behavior/Common/pluginManager.h"
#include <boost/function.hpp>

#include "Tools/Loaders/ParserHelper.h"

using namespace VEHA;

DECLARE_NAMESPACE_FILESYSTEM;

using boost::shared_dynamic_cast;

typedef KnowledgeBase* (*BaseInitFunc)();

int InstanceLoader::_id=0;

InstanceLoader::InstanceLoader(shared_ptr<AgentPlateform> agentPlateform,shared_ptr <Environment> environment, string filename, bool toRun)
{
	_toRun = toRun;
	if (filename != "")
	{
		_environment = environment;
		_agentPlateform = agentPlateform;
		_model = _environment->getModel();
		XmlParser parser;
                #if BOOST_FILESYSTEM_VERSION == 2
		_basedir=fs::path(filename).branch_path().file_string();
                #else
		_basedir=fs::path(filename).branch_path().string();
                #endif

		//remplacer branch_path par parent_path dans nouvelle version boost

		parser.parseFile(filename);
		shared_ptr<XmlNode> root (parser.getRoot());

		_parseInstances(root);
		_addRelations();
	}
	_relations.clear();

	cerr << " ##################FIN INSTANCES " << endl;
}

void InstanceLoader::parseInstances(shared_ptr<AgentPlateform> agentPlateform, shared_ptr <Environment> environment, string filename, bool toRun)
{
	_toRun = toRun;
	_environment = environment;
	_agentPlateform = agentPlateform;
	_model = _environment->getModel();
	XmlParser parser;
	#if BOOST_FILESYSTEM_VERSION == 2
	_basedir=fs::path(filename).branch_path().file_string();
        #else
	_basedir=fs::path(filename).branch_path().string();
	#endif

	cerr << "[InstanceLoader Info] Parse filename: " << filename << endl;

	parser.parseFile(filename);
	shared_ptr<XmlNode> root (parser.getRoot());

	_parseInstances(root);
	_addRelations();
	_relations.clear();
	cerr << " ##################FIN INSTANCES " << endl;
	cerr << "[InstanceLoader Info] End of parsing: " << filename << endl;
}


void InstanceLoader::_parseInstances(shared_ptr<XmlNode> node)
{
	if (node->getName() == "Area")
	{
		shared_ptr<Entity> instance=_createEntity(node);
		_environment->setMainZone(shared_dynamic_cast<Zone>(instance));
	}
	else
	{
		shared_ptr<XmlNode> child = node->getFirstChild();
		while (child )
		{
			if (child->getName() == "Entity" || child->getName() == "Area")
			{
				shared_ptr<Entity> instance=_createEntity(child);
			}
			else if (child->getName() == "Human")
			{
				shared_ptr<VirtualHuman> human=_createHuman(child);
				if(human != NULL)
					_agentPlateform->addAgent(human);
			}
			else if (child->getName() == "Organisation")
			{
				shared_ptr<OrganisationalEntity> org=_createOrganisation(child);
				_agentPlateform->addOrganisationalEntity(org);
			}
			child = child->getNext();
		}
	}
	cerr << " ##################FIN INSTANCES " << endl;
}


shared_ptr<Entity>  InstanceLoader::_createEntity(shared_ptr<XmlNode> node, shared_ptr<Entity> parent)
{
	shared_ptr<Entity>  e;

	if(!parent)
	{
	  string parentName = node->getProperty("area");
	  parent = MascaretApplication::getInstance()->getEnvironment()->getEntity(parentName);
	}

	string className = node->getProperty("class");
	shared_ptr<Class> classe;
	if (className == "")
		className = "undef";

	classe = _findClass(className);
	if (!classe)
	{
		cerr << "La classe " << className << " n'a pas été trouvée : L'entité "<< node->getProperty("name") << " ne sera pas créée" << endl;
		return e;
	}
	shared_ptr<EntityClass> entityClasse=shared_dynamic_cast<EntityClass>(classe);
	if(!entityClasse)
	{
		cerr << "La classe " << className << " n'est pas une classe d'entité : L'entité "<< node->getProperty("name") << " ne sera pas créée" << endl;
		return e;
	}
	if(node->getName() == "Area")
		e=boost::make_shared<Zone>(node->getProperty("name"),entityClasse );
	else
		e=boost::make_shared<Entity>(node->getProperty("name"),entityClasse );

	if (_toRun)
	{
		Classifier::OwnedBehaviorMap behaviors = classe->getOwnedBehavior();
		map<string, shared_ptr<Behavior> >::iterator itB;
		for (itB = behaviors.begin(); itB != behaviors.end(); itB++)
		{
			shared_ptr<Behavior> behavior = itB->second;
			BehaviorScheduler::getInstance()->executeBehavior(behavior,e,Parameters());
		}
	}
	_environment->addInstance(e);

	if (parent)
	{
		e->setParent(parent);
		parent->addChild(e);
	}

	_readEntity(node, e);
	return e;
}


void InstanceLoader::_readEntity(shared_ptr<XmlNode> node, shared_ptr<Entity> entity)
{
	shared_ptr<XmlNode> child = node->getFirstChild();
	Vector3 scaleFactor(1,1,1);
	//entity->setParent(entity->getParent());
	//point = entity->getReferentialPoint();

	while (child )
	{
		if (child->getName() == "Entity" || child->getName() == "Area")
		{
			shared_ptr<Entity> instance=_createEntity(child, entity);
                        //cerr << "Reading Entity : " << child->getName() << endl;
		}
		else if (child->getName() == "Position" || child->getName() == "Location")
		{
                        //cerr << "Reading Position : " << child->getName() << endl;
			entity->setLocalPosition(PositionVectorFromNodeProperties(child));
                        //cerr << "Reading Position : " << child->getName() << endl;
		}
		else if (child->getName() == "Orientation")
		{
			if (child->hasProperty("angle"))
			{
                        //cerr << "Reading Rotation : " << child->getName() << endl;
				entity->setLocalRotation(RotationVectorFromNodeProperties(child));
                        //cerr << "Reading Rotation : " << child->getName() << endl;
			}
			else
			{
                        //cerr << "Reading Orientation : " << child->getName() << endl;
				entity->setLocalOrientation(OrientationVectorFromNodeProperties(child));
                        //cerr << "Reading Orientation : " << child->getName() << endl;
			}
		}
		else if(child->getName() == "Topological")
		{
			string name=child->getProperty("name");
			shared_ptr<XmlNode> posNode=child->getChildByName("Position");
			shared_ptr<XmlNode> oriNode=child->getChildByName("Orientation");
			Vector3 pos = PositionVectorFromNodeProperties(posNode);
			shared_ptr<PointSpecification> point=MascaretApplication::getInstance()->getVRComponentFactory()->createPoint(pos);
			point->setParent(entity->getReferentialPoint());
			point->setLocalPosition(pos);
			if (oriNode)
			{
				if (oriNode->hasProperty("angle"))
				{
					point->setLocalRotation(RotationVectorFromNodeProperties(oriNode));
				}
				else
				{
					point->setLocalOrientation(OrientationVectorFromNodeProperties(oriNode));
				}
			}
			entity->getProperty(name)->addValue(point);
		}
		else if(child->getName() == "Viewpoint")
		{
                        cerr << "Reading Viewpoint : " << child->getName() << endl;
			string name=child->getProperty("name");
			shared_ptr<XmlNode> posNode=child->getChildByName("Position");
			shared_ptr<XmlNode> oriNode=child->getChildByName("Orientation");
			Vector3 pos = PositionVectorFromNodeProperties(posNode);
			shared_ptr<PointSpecification> viewPoint=MascaretApplication::getInstance()->getVRComponentFactory()->createPoint(pos);
			viewPoint->setParent(entity->getReferentialPoint());
			if (oriNode->hasProperty("angle"))
			{
				viewPoint->setLocalRotation(RotationVectorFromNodeProperties(oriNode));
			}
			else
			{
				viewPoint->setLocalOrientation(OrientationVectorFromNodeProperties(oriNode));
			}
			cerr << entity->getName() << endl;
			entity->addViewPoint(name,viewPoint);
                        //cerr << "Reading Viewpoint : " << child->getName() << endl;
		}
		else if (child->getName() == "OrientationLC")
		{
                        ////cerr << "Reading OrientationLC : " << child->getName() << endl;
			if (child->hasProperty("angle"))
			{
				entity->setLocalRotation(RotationVectorFromNodeProperties(child));
			}
			else
			{
				entity->setLocalOrientation(OrientationVectorFromNodeProperties(child));
			}
			Vector3 ori = entity->getLocalOrientation();
			//x = ori.x; y = ori.y; z = ori.z;
			// TODO: ATTENTION problème, ici c'est un setGlobalOrientation sans doute
			shared_ptr<PointSpecification> parentPoint=entity->getParent()->getReferentialPoint();
			parentPoint->localToGlobalOrientation(ori);
			//Vector3 ori2(x,y,z);
			entity->setLocalOrientation(ori);
                        ////cerr << "Reading OrientationLC : " << child->getName() << endl;
		}

		else if (child->getName() == "Shape")
		{
                        //cerr << "Reading Shape : " << child->getName() << endl;
			string url = child->getProperty("url") ;
			if(url.empty())
				url = child->getProperty("value") ;
			bool movable = false;
			bool recursive = false;
			if (child->hasProperty("movable")) child->getPropertyBoolean("movable", movable);
			if (child->hasProperty("recursive")) child->getPropertyBoolean("recursive", recursive);

			#if BOOST_FILESYSTEM_VERSION == 2
			url = fs::complete(url,_basedir).file_string();
			#else
			url = fs::complete(url,_basedir).string();
			#endif

			if(url.empty())
			{
				cerr << "Fichier " << child->getProperty("url") << " non trouvé pour l'entity "<< entity->getName() << endl;
			}
			string shader="";
			if(child->hasProperty("shader"))
			{
				shader=child->getProperty("shader");
			}
			string name="shape";
			if(child->hasProperty("name"))
			{
				name=child->getProperty("name");
			}
			shared_ptr<ShapeSpecification> specif(MascaretApplication::getInstance()->getVRComponentFactory()->createShape(entity->getName()+"_"+name,url,movable,recursive,shader));
			entity->getProperty(name)->addValue(specif);
			entity->setActiveShape(name);
                        //cerr << "Reading Shape : " << child->getName() << endl;
		}
		else if (child->getName() == "Shapes")
		{
			bool movable = false;
			if (child->hasProperty("movable"))
			{
				child->getPropertyBoolean("movable", movable);
			}
			bool recursive = false;
			if (child->hasProperty("recursive"))
			{
				child->getPropertyBoolean("recursive", recursive);
			}

			vector<string> urls;
			vector<double> distances;

			vector<shared_ptr<XmlNode> > shapeNodes=child->getChildNodesByName("Shape");
			for(size_t i=0;i<shapeNodes.size();i++)
			{
				shared_ptr<XmlNode> shapeNode=shapeNodes[i];
				string url = shapeNode->getProperty("url") ;

				#if BOOST_FILESYSTEM_VERSION == 2
				url = fs::complete(url,_basedir).file_string();
				#else
				url = fs::complete(url,_basedir).string();
				#endif

				if(url.empty())
				{
					cerr << "Fichier " << shapeNode->getProperty("url") << " non trouvé pour l'entity "<< entity->getName() << endl;
				}
				urls.push_back(url);
				double dist=0;
				if(shapeNode->hasProperty("dist"))
				{
					shapeNode->getPropertyReal("dist",dist);
				}
				distances.push_back(dist);
			}
			string name="shape";
			if(child->hasProperty("name"))
			{
				name=child->getProperty("name");
			}
			shared_ptr<ShapeSpecification> specif(MascaretApplication::getInstance()->getVRComponentFactory()->createShape(name,distances,urls,movable,recursive));
			entity->getProperty(name)->addValue(specif);
			//entity->set_DEP_current(specif);
			entity->setActiveShape(name);
		}
		else if (child->getName() == "Scale")
		{
			child->getPropertyReal("sx",scaleFactor.x);
			child->getPropertyReal("sy",scaleFactor.y);
			child->getPropertyReal("sz",scaleFactor.z);

		}
		else if (child->getName() == "Animation")
		{
			string name = child->getProperty("name");

			#if BOOST_FILESYSTEM_VERSION == 2
			string file = fs::complete(child->getProperty("url"),_basedir).file_string();
			#else
			string file = fs::complete(child->getProperty("url"),_basedir).string();
			#endif

			//shared_ptr<AnimationSpecification> animation(MascaretApplication::getInstance()->getVRComponentFactory()->createAnimation(name, file));
			//entity->getProperty(name)->addValue(animation);
		}
		else if (child->getName() == "Sound")
		{
			string name = child->getProperty("name");

			#if BOOST_FILESYSTEM_VERSION == 2
			string file = fs::complete(child->getProperty("url"),_basedir).file_string();
			#else
			string file = fs::complete(child->getProperty("url"),_basedir).string();
			#endif

			double gain = 1.0;
			double minDist = 1.0;
			double maxDist = 100.0;
			child->getPropertyReal("gain",gain);
			child->getPropertyReal("minDistance",minDist);
			child->getPropertyReal("maxDistance",maxDist);

			shared_ptr<SoundSpecification> sound(MascaretApplication::getInstance()->getVRComponentFactory()->createSound(name, file, gain,minDist,maxDist));
			entity->getProperty(name)->addValue(sound);
			sound->attachTo(entity->getReferentialPoint());
		}
		else if (child->getName() == "Inline")
		{
			XmlParser parser;

			#if BOOST_FILESYSTEM_VERSION == 2
			if (parser.parseFile(fs::complete(child->getProperty("url"),_basedir).file_string() ))
			#else
			if (parser.parseFile(fs::complete(child->getProperty("url"),_basedir).string() ))
			#endif
			{
				shared_ptr<XmlNode> root = parser.getRoot();
				_readEntity(root, entity);
			}
		}
		else if ( (child->getName()=="Attribute" ) || (child->getName()=="Relation") )
		{
			Relation r;
			r.e=entity;
			r.property=child->getProperty("name");
			r.value=child->getProperty("value");
			_relations.push_back(r);
		}
		else if (child->getName() == "InternalOrientationAxe")
		{
			//TODO: Pourquoi non utilisé ?
			Axe _front = AxeFromNodeProperty(child, "frontAxe");
			Axe _left = AxeFromNodeProperty(child, "leftAxe");
			Axe _above = AxeFromNodeProperty(child, "aboveAxe");
		}
		else if (child->getName() == "InternalOrientationVector")
		{
			//TODO:
		}
		else if (child->getName() == "State")
		{
			_initializeStateMachine(child, entity);
			cerr << "[InstanceLoader Info] STM " << child->getName() + " " + child->getProperty("machine") + " " + child->getProperty("state") << endl;
		}
		child = child->getNext();
	}
    if (entity->getActiveShape())
    {
        entity->getActiveShape()->setScale(scaleFactor);
    }

}

bool InstanceLoader::_initializeStateMachine(shared_ptr<XmlNode> node, shared_ptr<Entity> entity)
{
	string machine = node->getProperty("machine");
	string state = node->getProperty("state");
	if(machine.empty() || state.empty())
	{
		cerr << "[InstanceLoader Error] Entity: " << entity->getName() << " found empty string for machine or state." << endl;
		return false;
	}
	else
	{
		bool found_machine = false;
		vector< shared_ptr<StateMachineBehaviorExecution> > stmbes = entity->getStateMachineExecutions();
		for(size_t i = 0; i < stmbes.size(); i++)
		{
			if(stmbes[i]->getStateMachine()->getName() == machine)
			{
				found_machine = true;

				// Test if state exist on state machine
				bool found_state = false;
				shared_ptr<Region> region = stmbes[i]->getStateMachine()->getRegion()[0];
				vector< shared_ptr<Vertex> > vertices = region->getVertices();
				for(size_t i = 0; i< vertices.size(); i++)
				{
					cerr << "[IL]" << entity->getName() << " " << machine << " " << vertices[i]->getName() << endl;
					if (vertices[i]->getName() == state)
					{
						found_state = true;
					}
				}

				if(!found_state)
				{
					cerr << "[InstanceLoader Error] Entity: " << entity->getName() << " State "
							<< state << "not found on StateMachine " << machine << "." << endl;
				}

				stmbes[i]->setCurrentState(state);
			}
		}
		if(!found_machine)
		{
			cerr << "[InstanceLoader Error] Entity: " << entity->getName() << " StateMachine " << machine << " not found." << endl;
			return false;
		}
	}
	return true;
}

void InstanceLoader::_readGeometryEntity(shared_ptr<XmlNode> node, shared_ptr<Entity> entity)
{
	shared_ptr<XmlNode> child = node->getFirstChild();
	Vector3 scaleFactor(1,1,1);
	entity->setParent(entity->getParent());
	shared_ptr<PointSpecification> point;
	point = entity->getReferentialPoint();

	while (child )
	{
		if (child->getName() == "Position" || child->getName() == "Location")
		{
                        if (child->hasProperty("entity"))
                        {
                           string entityName = child->getProperty("entity");
                           //shared_ptr<InstanceSpecification> instance = MascaretApplication::getInstance()->getEnvironment()->getInstance(entityName);
                           shared_ptr<InstanceSpecification> instance = _environment->getInstance(entityName);
                           if (instance) 
                           {
                              shared_ptr<Entity> entPos = shared_dynamic_cast<Entity>(instance);
                              if (entPos) 
                              {
                                 entPos->addChild(entity);
                                 if (child->hasProperty("point"))
                                 {
                                    string pointName = child->getProperty("point");
                                    shared_ptr<PointSpecification> pt = entPos->getPoint(pointName);
                                    if (pt)
                                    {
                                       Vector3 pos = pt->getGlobalPosition();
                                       Vector3 ori = pt->getGlobalOrientation();
                                       point->setGlobalPosition(pos);
                                       point->setGlobalOrientation(ori);
                                    }
                                    else cerr << "Erreur : " << entPos->getName() << " has no point : " << pointName << " ! " << endl;
                                 }
                                 else
                                 {
                                    Vector3 pos = entPos->getGlobalPosition();
                                    Vector3 ori = entPos->getGlobalOrientation();
                                    point->setGlobalPosition(pos);
                                    point->setGlobalOrientation(ori);
                                 }
                              }
                              else cerr << " Error : " << entityName << " is not an entity ! " << endl;
                           }
                           else cerr << " Error : " << entityName << " not found ! " << endl;
                               
                        }
                        else
                        {
                        	point->setLocalPosition(PositionVectorFromNodeProperties(child));
                        }
		}
		else if (child->getName() == "Orientation")
		{
			if (child->hasProperty("angle"))
			{
				point->setLocalRotation(RotationVectorFromNodeProperties(child));
			}
			else
			{
				point->setLocalOrientation(OrientationVectorFromNodeProperties(child));
			}
		}

		else if (child->getName() == "Shape")
		{
			string url = child->getProperty("url") ;
			bool movable = false;
			bool recursive = false;
			if (child->hasProperty("movable")) child->getPropertyBoolean("movable", movable);
			if (child->hasProperty("recursive")) child->getPropertyBoolean("recursive", recursive);

			#if BOOST_FILESYSTEM_VERSION == 2
			url = fs::complete(url,_basedir).file_string();
			#else
			url = fs::complete(url,_basedir).string();
			#endif

			string shader="";
			if(child->hasProperty("shader"))
			{
				shader=child->getProperty("shader");
			}
			string name="shape";
			if(child->hasProperty("name"))
			{
				name=child->getProperty("name");
			}
			shared_ptr<ShapeSpecification> specif(MascaretApplication::getInstance()->getVRComponentFactory()->createShape(name,url,movable,recursive,shader));
			entity->getProperty(name)->addValue(specif);
			//entity->set_DEP_current(specif);
			entity->setActiveShape(name);
		}
		else if (child->getName() == "Shapes")
		{
			bool movable = false;
			if (child->hasProperty("movable"))
			{
				child->getPropertyBoolean("movable", movable);
			}
			bool recursive = false;
			if (child->hasProperty("recursive")) child->getPropertyBoolean("recursive", recursive);

			vector<string> urls;
			vector<double> distances;

			vector<shared_ptr<XmlNode> > shapeNodes=child->getChildNodesByName("Shape");
			for(size_t i=0;i<shapeNodes.size();i++)
			{
				shared_ptr<XmlNode> shapeNode=shapeNodes[i];
				string url = shapeNode->getProperty("url") ;

				#if BOOST_FILESYSTEM_VERSION == 2
				url = fs::complete(url,_basedir).file_string();
				#else
				url = fs::complete(url,_basedir).string();
				#endif

				if(url.empty())
				{
					cerr << "Fichier " << shapeNode->getProperty("url") << " non trouvé pour l'entity "<< entity->getName() << endl;
				}
				urls.push_back(url);
				double dist=0;
				if(shapeNode->hasProperty("dist"))
				{
					shapeNode->getPropertyReal("dist",dist);
				}
				distances.push_back(dist);
			}
			string name="shape";
			if(child->hasProperty("name"))
			{
				name=child->getProperty("name");
			}
			shared_ptr<ShapeSpecification> specif(MascaretApplication::getInstance()->getVRComponentFactory()->createShape(name,distances,urls,movable,recursive));
			entity->getProperty(name)->addValue(specif);
			//entity->set_DEP_current(specif);
			entity->setActiveShape(name);
		}
		else if (child->getName() == "Scale")
		{
			child->getPropertyReal("sx",scaleFactor.x);
			child->getPropertyReal("sy",scaleFactor.y);
			child->getPropertyReal("sz",scaleFactor.z);

		}
		else if (child->getName() == "Animation")
		{
			string name = child->getProperty("name");
			bool running=false;
                        if (child->hasProperty("running"))
                           child->getPropertyBoolean("running",running);

			string file = "";
			if(!child->getProperty("url").empty())
			{
				#if BOOST_FILESYSTEM_VERSION == 2
				file = fs::complete(child->getProperty("url"),_basedir).file_string();
				#else
				file = fs::complete(child->getProperty("url"),_basedir).string();
				#endif
			}

			shared_ptr<AnimationSpecification> animation(MascaretApplication::getInstance()->getVRComponentFactory()->createAnimation(name, file));
			entity->getProperty(name)->addValue(animation);
                        if (running) entity->playAnimation(name);
		}
		else if (child->getName() == "Sound")
		{
			string name = child->getProperty("name");

			#if BOOST_FILESYSTEM_VERSION == 2
			string file = fs::complete(child->getProperty("url"),_basedir).file_string();
			#else
			string file = fs::complete(child->getProperty("url"),_basedir).string();
			#endif

			double gain = 1.0;
			double minDist = 1.0;
			double maxDist = 100.0;
			child->getPropertyReal("gain",gain);
			child->getPropertyReal("minDistance",minDist);
			child->getPropertyReal("maxDistance",maxDist);

			shared_ptr<SoundSpecification> sound(MascaretApplication::getInstance()->getVRComponentFactory()->createSound(name, file, gain,minDist,maxDist));
			entity->getProperty(name)->addValue(sound);
			sound->attachTo(entity->getReferentialPoint());
		}
		else if (child->getName() == "Inline")
		{
			XmlParser parser;

			#if BOOST_FILESYSTEM_VERSION == 2
			if (parser.parseFile(fs::complete(child->getProperty("url"),_basedir).file_string() ))
			#else
			if (parser.parseFile(fs::complete(child->getProperty("url"),_basedir).string() ))
			#endif
			{
				shared_ptr<XmlNode> root = parser.getRoot();
				_readGeometryEntity(root, entity);
			}
		}
		else if (child->getName() == "InternalOrientationAxe")
		{
			//TODO: Pourquoi non utilisé ?
			Axe _front = AxeFromNodeProperty(child, "frontAxe");
			Axe _left = AxeFromNodeProperty(child, "leftAxe");
			Axe _above = AxeFromNodeProperty(child, "aboveAxe");
			/*point->setFront(_front);
			point->setLeft(_left);
			point->setAbove(_above);*/
		}
		else if (child->getName() == "InternalOrientationVector")
		{
			//TODO:
		}
		else if (child->getName() == "State")
		{
			// TODO: Maybe it's not normal to found a State here ?
			_initializeStateMachine(child, entity);
		}
		child = child->getNext();
	}
	/*
    if (entity->getActiveShape())
    {
        entity->getActiveShape()->setScale(scaleFactor);
    }	 */
}

shared_ptr<OrganisationalEntity>  InstanceLoader::_createOrganisation(shared_ptr<XmlNode> node)
{
	shared_ptr<OrganisationalEntity> organisation;
	string name = node->getProperty("name");


	shared_ptr<XmlNode> struc = node->getChildByName("Structure");
	if (struc)
	{
		string structName = struc->getChildByName("text")->getString();
		vector<shared_ptr<OrganisationalStructure> > structs = _agentPlateform->getOrganisationalStructure();
		shared_ptr<OrganisationalStructure> structOrg;
		bool found = false;
		int orgSize = structs.size();
		int i = 0;
		while (!found && i < orgSize)
		{
			if (structs[i]->getName() == structName) found = true;
			else i++;
		}
		if (!found)
			cerr << "Erreur : Pas de structure pour l'organisation : " << name << endl;
		else
		{
			structOrg = structs[i];
			organisation = boost::make_shared<OrganisationalEntity>(name);
			organisation->setStructure(structOrg);

			structOrg->addEntity(organisation);
			vector <shared_ptr<Role> > roles = structOrg->getRoles();

			shared_ptr<XmlNode> affects = node->getFirstChild();
			while (affects)
			{
				if (affects->getName() == "Affect")
				{
					if (affects->getProperty("type") == "Role")
					{
						shared_ptr<XmlNode> roleNode = affects->getChildByName("Role");
						shared_ptr<XmlNode> agentNode = affects->getChildByName("Agent");
						shared_ptr<XmlNode> aidNode = agentNode->getChildByName("AID");
						string agentName;
						string plateformeName ="localhost";
						int plateformePort =_agentPlateform->getPort();
						if (aidNode)
						{
							agentName = aidNode->getChildByName("Name")->getChildByName("text")->getString();
							plateformeName = aidNode->getChildByName("Plateform")->getChildByName("text")->getString();
							string plateformePortS = aidNode->getChildByName("Port")->getChildByName("text")->getString();
							plateformePort = atoi(plateformePortS.c_str());
						}
						else
							agentName = agentNode->getChildByName("text")->getString();

						string roleName = roleNode->getChildByName("text")->getString();
						if (plateformeName == "localhost") plateformeName = _agentPlateform->getName();

						shared_ptr<AID> aid = boost::make_shared<AID>(agentName,plateformeName,plateformePort);
						shared_ptr<Role> role;
						found = false;
						i = 0;
						while (!found && (unsigned)i < roles.size())
						{
							if (roles[i]->getName() == roleName) found = true;
							else i++;
						}
						if (!found)
							cerr << "Erreur, Role : " << roleName << " n'appartient pas a l'organisation : " << structName << endl;
						else
						{
							role = roles[i];
							shared_ptr<RoleAssignement> assignement = boost::make_shared<RoleAssignement>(organisation);
							assignement->setAgent(aid);
							assignement->setRole(role);
							organisation->addRoleAssignement(assignement);
						}

					}
					else if (affects->getProperty("type") == "Ressource")
					{
						shared_ptr<XmlNode> ressourceNode = affects->getChildByName("Ressource");
						shared_ptr<XmlNode> entityNode = affects->getChildByName("Entity");
						string ressourceName = ressourceNode->getChildByName("text")->getString();
						string entityName = entityNode->getChildByName("text")->getString();
						cerr << "[InstanceLoader Info] Affectation Ressource : " << ressourceName << " = " << entityName << endl;
						// Recherche de l'entite .....
						shared_ptr<InstanceSpecification> entite = _environment->getInstance(entityName);
						if (!entite) cerr << "[InstanceLoader Warning] Entity " << entityName << " non trouve ..." << endl;

						// Recherche de la ressource
						shared_ptr<Ressource> res;
						bool found = false;
						vector < shared_ptr<Ressource> > ressources = structOrg->getRessources();
						for (size_t iRes = 0; iRes < ressources.size(); iRes++)
						{
						//	cerr << "Resource search: comparing " << ressources[iRes]->getName() << " to " << ressourceName << endl;
							if (ressources[iRes]->getName() == ressourceName)
							{
								res = ressources[iRes];
								found = true;
							}
						}

						if(found)
						{
							cerr << "[InstanceLoader Info] Ressource trouve ..." << endl;

							// Creation de l'assignement
							shared_ptr<RessourceAssignement> ra = boost::make_shared<RessourceAssignement>();
							ra->setEntity(shared_dynamic_cast<Entity>(entite));
							ra->setRessource(res);

							organisation->addRessourceAssignement(ra);
						}
						else
						{
							cerr << "[InstanceLoader Warning] Ressource non trouve ..." << endl;
						}
					}
				}
				affects = affects->getNext();
			}
		}
	}
	else
		cerr << "Erreur : Pas de structure pour l'organisation : " << name << endl;

	return organisation;
}

shared_ptr<VirtualHuman>  InstanceLoader::_createHuman(shared_ptr<XmlNode> node)
{

	shared_ptr<VirtualHuman>  h;

	string className = node->getProperty("class");
	shared_ptr<Class> classe;
	if (className == "")
		className = "undef";

	classe = _findClass(className);
	if (!classe)
	{
		cerr << "La classe " << className << " n'a pas été trouvée : L'agent "<< node->getProperty("name") << " ne sera pas créé" << endl;
		return h;
	}
	shared_ptr<VirtualHumanClass> vhClasse=shared_dynamic_cast<VirtualHumanClass>(classe);
	if(!vhClasse)
	{
		cerr << "La classe " << className << " n'est pas une classe d'humain virtuel : L'agent "<< node->getProperty("name") << " ne sera pas créé" << endl;
		return h;
	}
	h=boost::make_shared<VirtualHuman>(_agentPlateform, node->getProperty("name"),vhClasse);

	
	h->setControlledByHuman(false); //default
	
	if ((node->hasProperty("controlled")) && (node->getProperty("controlled") == "yes"))
	{
		h->setControlledByHuman(true);
	}


	if (_toRun)
	{
		Classifier::OwnedBehaviorMap behaviors = classe->getOwnedBehavior();
		map<string, shared_ptr<Behavior> >::iterator itB;
		for (itB = behaviors.begin(); itB != behaviors.end(); itB++)
		{
			shared_ptr<Behavior> behavior = itB->second;
			BehaviorScheduler::getInstance()->executeBehavior(behavior,h,Parameters());
		}
	}


	_environment->addInstance(h);
	_readGeometryEntity(node, h->getBody());

	shared_ptr<XmlNode> child = node->getFirstChild();
	while (child )
	{
		if (child->getName() == "Behavior")
		{
			bool added=false;
			bool start=true;

			if(child->hasProperty("start"))
                        {
			    child->getPropertyBoolean("start",start);
                        }
			if(child->hasProperty("interval"))
			{
				double interval;
				if(child->getPropertyReal("interval",interval))
				{
					added=true;
					h->addBehavior(child->getProperty("name"),interval,start);
				}
			}
			if(!added)
				h->addBehavior(child->getProperty("name"),-1,start);
		}
		else if (child->getName() == "KnowledgeBase")
		{
			string name = child->getProperty("name");
			void * symbol = 0;
			symbol = PluginManager::getInstance()->getSymbol(name+"_init");
			if (symbol)
			{
				BaseInitFunc baseInit=reinterpret_cast<BaseInitFunc>((intptr_t)symbol);
				shared_ptr<KnowledgeBase> base(baseInit());
				h->setKnowledgeBase(base);
			}
			else
			{
				shared_ptr<KnowledgeBase> kb = h->getKnowledgeBase();
                                kb->setName(name);
				shared_ptr<XmlNode> envNode = child->getChildByName("Environment");
				if (envNode)
				{
					string url = envNode->getProperty("url");
					shared_ptr<Environment> env = parseEnvironment(url);
					kb->setEnvironment(env);
				}
				else
				{
					cerr << " Agent has global knowledge on environment" << endl;
					kb->setEnvironment(MascaretApplication::getInstance()->getEnvironment());
				}
                        }
		}
		else if(child->getName() == "Attribute")
		{
			InstanceSpecification::SlotMap::const_iterator it=h->getSlots().find(child->getProperty("name"));
			if(it != h->getSlots().end())
			{
				shared_ptr<ValueSpecification> val=it->second->getDefiningProperty()->createValueFromString(child->getProperty("value"));
				it->second->addValue(val);
			}
			else
			{
				cerr << "The attribute " << child->getProperty("name") << " specified on the entity " << h->getName() << " doesn't exist"<< endl;
			}
		}
		else if(child->getName() == "ExecuteOperation")
		{
			shared_ptr<Class> classifier = h->getClassifier();
#if GCC_VERSION > 40100
			shared_ptr<Operation> operation=classifier->getOperations().at(child->getProperty("name"));
#else
			shared_ptr<Operation> operation=MAP_AT(classifier->getOperations(),child->getProperty("name"));
#endif
			//Parameters param;
			InstanceSpecification::OperationCallParameters param;
			vector<shared_ptr<Parameter> > parameters = operation->getParameter();
			for (size_t i=0;i<parameters.size();i++)
			{
				bool found=false;
				string strVal="";
				vector<shared_ptr<XmlNode> > paramNodes=child->getChildNodesByName("Parameter");
				for(size_t j=0;j<paramNodes.size();j++)
				{
					shared_ptr<XmlNode> paramNode=paramNodes[j];
					if(parameters[i]->getName() == paramNode->getProperty("name"))
					{
						strVal=paramNode->getProperty("value");
						found=true;
						break;
					}
				}
				if(!found)
				{
					cerr << "Can't find the value of the parameter " << parameters[i]->getName() << " when launching operation "
							<< operation->getName() << " on entity "<< h->getName() << endl;
					continue;
				}
				for (size_t iC = 0; iC < strVal.size(); iC++)
				{
					if (strVal[iC] == '+') strVal[iC] = ' ';
				}
				try
				{
					param.push_back(parameters[i]->getType()->createValueFromString(strVal));
				}
				catch(std::out_of_range& e)
				{
					cerr << "Can't find the entity " << strVal << " when launching operation "
							<< operation->getName() << " on entity "<< h->getName() << endl;
				}
			}

			//cerr << " Launch oper : " << operation->getName() << " on entity : " << h->getName() << endl;
			h->executeOperation(child->getProperty("name"),param);
			//BehaviorScheduler::getInstance()->executeBehavior(operation->getMethod(),h,param);
		}
		else if (child->getName() == "Entity")
		{
			shared_ptr<Entity> instance=_createEntity(child, h->getBody());
		}
		child = child->getNext();
	}
	return h;
}


shared_ptr<Environment> InstanceLoader::parseEnvironment(string url)
{
       shared_ptr<Environment> env;

        shared_ptr<XmlParser> parser = boost::make_shared<XmlParser>();
        parser->parseFile(url);
        shared_ptr<XmlNode> root = parser->getRoot();

        shared_ptr<Model> model;
        shared_ptr<XmlNode> modelNode = root->getChildByName("Model");
        if(modelNode)
        {
                string urlModel=modelNode->getProperty("url");

                model = parseModel(urlModel);
	}
	else
		model = _model;

        env = boost::make_shared<Environment>(model);
        env->setUrl(url);
        model->addEnvironment(env);

	string baseDir = MascaretApplication::getInstance()->getBaseDir();

        #if BOOST_FILESYSTEM_VERSION == 2
        InstanceLoader instanceLoader(_agentPlateform, env, fs::complete(url,baseDir).file_string(),false);
        #else
        InstanceLoader instanceLoader(_agentPlateform, env, fs::complete(url,baseDir).string(),false);
        #endif

	return env;
}

shared_ptr<Model> InstanceLoader::parseModel(string url)
{
	string baseDir = MascaretApplication::getInstance()->getBaseDir();
        #if BOOST_FILESYSTEM_VERSION == 2
        shared_ptr<ModelLoader2> modelLoader = boost::make_shared<ModelLoader2>(fs::complete(url,baseDir).file_string());
        #else
        shared_ptr<ModelLoader2> modelLoader = boost::make_shared<ModelLoader2>(fs::complete(url,baseDir).string());
        #endif
        shared_ptr<Model> model = modelLoader->getModel();
        model->setModelLoader(modelLoader);

        return model;
}



shared_ptr<Class> InstanceLoader::_findClass(const string& className)
{
	//shared_ptr<Model> model = MascaretApplication::getInstance()->getModel();

	shared_ptr<Class> classe ;

	classe = _findClassPackage(_model->getPackage(),className);
	return classe;
}


shared_ptr<Class> InstanceLoader::_findClassPackage(shared_ptr<Package> package,const string& className)
{
	const vector<shared_ptr<Class> >& classes = package->getClasses();
	shared_ptr<Class> classe;

	for (size_t i = 0; i < classes.size(); i ++)
	{
		if (classes[i]->getName() == className)
		{
			classe = classes[i];
			break;
		}
	}

	if (!classe )
	{
		const vector<shared_ptr<Package> >& packages = package->getPackages();
		for (size_t i = 0; i < packages.size(); i ++)
		{
			classe = _findClassPackage(packages[i], className);
			if (classe)  break;
		}
	}
	return classe;
}


void InstanceLoader::_addRelations()
{
	cerr << "[InstanceLoader Info] Add Relations, " << _relations.size()
			<< " relations to add." << endl;
	for (size_t i=_relations.size();i--;)
	{
		Relation & r=_relations[i];
		//cerr << r.e->getName() << " / " << r.value << endl;
		if(r.e->getProperty(r.property) != NULL)
		  r.e->getProperty(r.property)->addValueFromString(r.value);
	}

}
