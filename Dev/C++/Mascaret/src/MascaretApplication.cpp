#include "Tools/filesystem.h"
#ifdef _WIN32
#include "winsock2.h" // for gethostname
#endif
#include <stdio.h>
//#include <netdb.h>
#include <iostream>
#include <fstream>

//#include <pthread.h>

#include "MascaretApplication.h"

#include "Tools/Loaders/InstanceLoader.h"
#include "Tools/Loaders/ModelLoader.h"
#include "Tools/Loaders/ModelLoader2.h"
#include "VEHA/Behavior/Common/pluginManager.h"
#include "BEHAVE/AgentClass.h"
#include "BEHAVE/Agent.h"
#include "BEHAVE/AgentPlateform.h"
#include "VEHA/Entity/VirtualRealityComponentFactory.h"
#include "VEHA/Kernel/PrimitiveType.h"
#include "IEHA/Visualisation/Camera.h"
#include "IEHA/Visualisation/Window3D.h"
//#include "MATS/Poseidon/LearningSession.h"

#include "VEHA/Behavior/Common/behaviorScheduler.h"
#include "HAVE/Body.h"
#include "VEHA/Entity/EntityClass.h"

#include "Tools/Scenario/ScenarioScheduler.h"
#include "Tools/Scenario/Scenario.h"

DECLARE_NAMESPACE_FILESYSTEM;

typedef void (*fct)(void);

MascaretApplication* MascaretApplication::_instance = NULL;

MascaretApplication::MascaretApplication()
{
  cerr << "Mascaret build datetime: " << __DATE__ << " " << __TIME__ << endl;
  if (_instance) throw runtime_error("Une autre instance de MascaretApplication a déjà été créée");
  _instance = this;
  char hostname[1024];
  gethostname(hostname, 1024);
  _hostname = string(hostname);
//	_learningSession = shared_ptr<Poseidon::LearningSession>(new Poseidon::LearningSession());
}

MascaretApplication::~MascaretApplication()
{
  PluginManager::getInstance()->fireMascaretDestroying();
  BehaviorScheduler::destroyInstance();
}

void MascaretApplication::parse(const string& applicationFileName)
{
  cerr << "[MascaretApplication Info] #### Start parsing application file: " << applicationFileName << endl;
  fs::path appPath(fs::complete(applicationFileName));
#if BOOST_FILESYSTEM_VERSION == 2
  _baseDir=appPath.branch_path().file_string();
  cerr << "  name: "<< appPath.branch_path().file_string() << endl;
#else
  _baseDir = appPath.branch_path().string();
  cerr << "  name: " << appPath.branch_path().string() << endl;
#endif

  cerr << "[MascaretApplication Info] Creating parser object...";
  shared_ptr<XmlParser> parser = make_shared<XmlParser>();
  cerr << " OK!" << endl;
  parser->parseFile(applicationFileName);
  cerr << "[MascaretApplication Info] #### Finish parsing application file: " << applicationFileName << " OK!" << endl;
  shared_ptr<XmlNode> root = parser->getRoot();

  parse(root);

  // Init Plugin
  PluginManager::getInstance()->fireMascaretModelLoaded();
}
void MascaretApplication::parse(shared_ptr<XmlNode> root)
{
  shared_ptr<XmlNode> appliNode = root->getChildByName("ApplicationParameters");
  shared_ptr<XmlNode> actNode = root->getChildByName("Actors");
  shared_ptr<XmlNode> orgNode = root->getChildByName("Organisations");

  if (appliNode)
  {
    parseApplication(appliNode);
  }
  else
  {
    // Parametres par defaut de l'applie ....
    string ressourceDir = "HTTPServerBaseDir";
    agentPlateform = boost::make_shared<AgentPlateform>(_hostname, 8080, ressourceDir);
    agent = boost::make_shared<Agent>(agentPlateform, "HTTPManager", shared_ptr<AgentClass>());
    agent->addBehavior("SimpleCommunicationBehavior");
    agentPlateform->addAgent(agent);
    ////createServer(new HTTPManager(ressourceDir,agentPlateform,agent),8080);
  }

  //parseEnvironment(model,root);

  shared_ptr<XmlNode> envChild = root->getFirstChild();
  while (envChild)
  {
    if (envChild->getName() == "Environment")
    {
      string envUrl = envChild->getProperty("url");
      shared_ptr<Environment> env = parseEnvironment(envUrl, actNode, orgNode);
    }
    else if(envChild->getName() == "Scenario")
    {
        string url = envChild->getProperty("url");
#if BOOST_FILESYSTEM_VERSION == 2
        string fullpath = fs::complete(url,_baseDir).file_string();
#else
        string fullpath = fs::complete(url, _baseDir).string();
#endif
        Scenario::ScenarioScheduler::getInstance()->parseScenarioFile(fullpath);
        string scenario = Scenario::ScenarioScheduler::getInstance()->getScenario()->writeScenario();
        cout << scenario << endl;
    }
    envChild = envChild->getNext();
  }
}
void MascaretApplication::init()
{
  //Nothing to do
}
void MascaretApplication::run()
{
  while (step())
  {
  }
}
bool MascaretApplication::step()
{
  BehaviorScheduler::getInstance()->step();
  Scenario::ScenarioScheduler::getInstance()->activate();
  PluginManager::getInstance()->fireMascaretStep();
  return true;
}
void MascaretApplication::generateDocumentation()
{
//TODO
}

void MascaretApplication::parseApplication(shared_ptr<XmlNode> appliNode)
{
  bool noServer = false;
  string ressourceDir = "../HTTPServerBaseDir";
  int port = 8080;
  int ports = 8080;
  char _hostname[1024];

  gethostname(_hostname, 1024);

  shared_ptr<XmlNode> serverNode = appliNode->getChildByName("HTTP");
  if (serverNode)
  {
    if (serverNode->hasProperty("NoServer")) serverNode->getPropertyBoolean("httpPort", noServer);
    if (serverNode->hasProperty("RessourceDir")) ressourceDir = serverNode->getProperty("RessourceDir");
    if (serverNode->hasProperty("httpPort")) serverNode->getPropertyInteger("httpPort", port);
    if (serverNode->hasProperty("httpsPort")) serverNode->getPropertyInteger("httpsPort", ports);
  }

  agentPlateform = boost::make_shared<AgentPlateform>(_hostname, port, ressourceDir, noServer);
  shared_ptr<XmlNode> userNode = appliNode->getChildByName("User");
  if (userNode)
  {
    agent = boost::make_shared<Agent>(agentPlateform, userNode->getProperty("name"), shared_ptr<AgentClass>());
    agentPlateform->addAgent(agent);
    agent->addBehavior("SimpleCommunicationBehavior");
  }
  else
  {
    agent = make_shared<Agent>(agentPlateform, "HTTPManager", shared_ptr<AgentClass>());
    agent->addBehavior("SimpleCommunicationBehavior");
    agentPlateform->addAgent(agent);
  }
  //createServer(new HTTPManager(ressourceDir,agentPlateform, agent),port);

  shared_ptr<XmlNode> plugsNode = appliNode->getChildByName("Plugins");
  if (plugsNode)
  {
    cerr << "Loading plugins..." << endl;
#if BOOST_FILESYSTEM_VERSION == 2
    PluginManager::getInstance()->setPluginDir(fs::complete(plugsNode->getProperty("PluginsDir"),_baseDir).file_string());
#else
    PluginManager::getInstance()->setPluginDir(
        fs::complete(plugsNode->getProperty("PluginsDir"), _baseDir).string());
#endif

    //PluginManager::getInstance()->setPluginDir(plugsNode->getProperty("PluginsDir"));
    shared_ptr<XmlNode> pluginNode = plugsNode->getFirstChild();
    while (pluginNode)
    {
      if (pluginNode->getName() == "Plugin")
      {
        string name = pluginNode->hasProperty("Name") ? pluginNode->getProperty("Name") : pluginNode->getProperty("name");
        PluginManager::getInstance()->openPlugin(name, pluginNode);
      }
      pluginNode = pluginNode->getNext();
    }
  }

}

shared_ptr<Model> MascaretApplication::parseModel(string url)
{
#if BOOST_FILESYSTEM_VERSION == 2
  shared_ptr<ModelLoader2> modelLoader =boost::make_shared<ModelLoader2>(fs::complete(url,_baseDir).file_string());
#else
  shared_ptr<ModelLoader2> modelLoader = make_shared<ModelLoader2>(fs::complete(url, _baseDir).string());
#endif
  shared_ptr<Model> model = modelLoader->getModel();
  model->setModelLoader(modelLoader);

  return model;
}

shared_ptr<Environment> MascaretApplication::parseEnvironment(string url,
                                                              shared_ptr<XmlNode> actNode,
                                                              shared_ptr<XmlNode> orgNode)
{
  shared_ptr<Environment> env;

  shared_ptr<XmlParser> parser = make_shared<XmlParser>();
  parser->parseFile(url);
  shared_ptr<XmlNode> root = parser->getRoot();

  shared_ptr<Model> model;
  shared_ptr<XmlNode> modelNode = root->getChildByName("Model");
  if (modelNode)
  {
    string urlModel = modelNode->getProperty("url");

    model = parseModel(urlModel);

    map<string, shared_ptr<Environment> >::iterator it = model->getEnvironments().find(url);
    if (model->getEnvironments().size() != 0 && it != model->getEnvironments().end())
    {
      return it->second;
    }
    else
    {
      env = make_shared<Environment>(model);
      env->setUrl(url);
      model->addEnvironment(env);

#if BOOST_FILESYSTEM_VERSION == 2
      InstanceLoader instanceLoader(agentPlateform, env, fs::complete(url,_baseDir).file_string());
#else
      InstanceLoader instanceLoader(agentPlateform, env, fs::complete(url, _baseDir).string());
#endif

      cout << "SIZE of env map " << model->getEnvironments().size() << " of model :"
          << model->getUrl() << endl;

      if (actNode)
#if BOOST_FILESYSTEM_VERSION == 2
        instanceLoader.parseInstances(agentPlateform,env,fs::complete(actNode->getProperty("url"),_baseDir).file_string());
#else
        instanceLoader.parseInstances(agentPlateform, env,
                                      fs::complete(actNode->getProperty("url"), _baseDir).string());
#endif

      if (orgNode)
#if BOOST_FILESYSTEM_VERSION == 2
        instanceLoader.parseInstances(agentPlateform,env,fs::complete(orgNode->getProperty("url"),_baseDir).file_string());
#else
        instanceLoader.parseInstances(agentPlateform, env,
                                      fs::complete(orgNode->getProperty("url"), _baseDir).string());
#endif
    }
  }
  else
    cerr << "There's no model in this environment.Please add a model in " << url << "." << endl;
  return env;
}

shared_ptr<PrimitiveType> MascaretApplication::getBasicType(string type)
{

  return getModel()->getBasicType(type);
  /*

   shared_ptr<PrimitiveType> primitiveType;

   map<string,shared_ptr<Model> >::iterator it;
   for(it = _models.begin();it!=_models.end();it++)
   {else
   cerr<<"There's no model in this environment.Please add a model in "<<url<<"."<<endl;
   primitiveType = it->second->getBasicType(type);
   }

   return primitiveType;*/

}

shared_ptr<Environment> MascaretApplication::getEnvironment()
{
  return _models.begin()->second->getEnvironments().begin()->second;
}

shared_ptr<Model> MascaretApplication::getModel()
{
  return _models.begin()->second;
}

shared_ptr<Model> MascaretApplication::getModel(string modelName)
{
  map<std::string, boost::shared_ptr<Model> >::iterator it = _models.find(modelName);
  if (it != _models.end())
    return it->second;
  else
    return shared_ptr<Model>();
}

bool MascaretApplication::hasModel(string modelName)
{
  map<std::string, boost::shared_ptr<Model> >::iterator it = _models.find(modelName);
  return it != _models.end();
}

