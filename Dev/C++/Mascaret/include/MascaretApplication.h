#ifndef MASCARET_APPLICATION_H
#define MASCARET_APPLICATION_H


#include "Tools/veha_plateform.h"
#include "VEHA/Kernel/Model.h"

class XmlNode;

namespace BEHAVE
{
class Agent;
class AgentPlateform;
}
namespace VEHA
{
class VirtualRealityComponentFactory;
class PrimitiveType;
class Environment;
}

/*namespace Poseidon
{
class LearningSession;
}*/

using std::string;
using std::map;

using VEHA::VirtualRealityComponentFactory;
using VEHA::Environment;
using VEHA::Model;
using VEHA::PrimitiveType;

using BEHAVE::AgentPlateform;
using BEHAVE::Agent;

//using namespace Poseidon;

class VEHA_API MascaretApplication
{
public :
	MascaretApplication();
	virtual ~MascaretApplication();

	virtual void parse(const string& applicationFileName);


	virtual bool step();
	virtual void run();

	/**
	  if something special to do in specific applications
	*/

	virtual void init();
	virtual void generateDocumentation(void);


	inline boost::shared_ptr<AgentPlateform> getAgentPlateform() {return agentPlateform;}


	//inline boost::shared_ptr<Poseidon::LearningSession> getLearningSession() {return _learningSession;}
	boost::shared_ptr<Agent> agent;

	inline boost::shared_ptr<VirtualRealityComponentFactory> getVRComponentFactory() {return _vrComponentFactory;}
	inline void setVRComponentFactory(boost::shared_ptr<VirtualRealityComponentFactory> fact) {_vrComponentFactory=fact;}

	static inline MascaretApplication* getInstance() {return _instance;}
	inline std::string getBaseDir() {return _baseDir;}

	boost::shared_ptr<Model> parseModel(std::string url);
	boost::shared_ptr<Environment> parseEnvironment(std::string url,boost::shared_ptr<XmlNode> actNode,boost::shared_ptr<XmlNode> orgNode);
	boost::shared_ptr<PrimitiveType> getBasicType(std::string type);

	/*TO MODIFY*/
	boost::shared_ptr<Environment> getEnvironment();
	boost::shared_ptr<Model> getModel();
	boost::shared_ptr<Model> getModel(string modelName);
	bool hasModel(string modelName);
	inline map<std::string, boost::shared_ptr<Model> > getModels() {return _models;}
	inline void addModel( boost::shared_ptr<Model> curModel){_models[curModel->getName()]=curModel;}


protected:
	virtual void parse(boost::shared_ptr<XmlNode> root);
	virtual void parseApplication(boost::shared_ptr<XmlNode> applicationNode);


private:
	std::string _baseDir;
	std::string _hostname;
	map<std::string, boost::shared_ptr<Model> > _models;
	boost::shared_ptr<AgentPlateform> agentPlateform;
	boost::shared_ptr<VirtualRealityComponentFactory> _vrComponentFactory;
	//boost::shared_ptr<Poseidon::LearningSession> _learningSession;
	static MascaretApplication* _instance;
};
#endif
