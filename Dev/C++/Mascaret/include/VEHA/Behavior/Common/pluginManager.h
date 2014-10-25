#ifndef PLUGIN_MANAGER_H
#define PLUGIN_MANAGER_H
#include "Tools/veha_plateform.h"
class XmlNode;
namespace VEHA
{
class OpaqueBehavior;

class VEHA_API PluginManager
{
public:
	static PluginManager * getInstance();
	virtual ~PluginManager();
	//TODO ->add
	bool openPlugin(const std::string & name, shared_ptr<XmlNode> pluginNode=shared_ptr<XmlNode>());
	void * getSymbol(const std::string& symbolName);
	std::string getPluginError();

	const std::string& getPluginDir() const{return _pluginDir;}
	void setPluginDir(const std::string& dir){_pluginDir=dir;}

	/* Plugin Creation */
	void updatePlugin(const std::string& pluginName,const std::vector<std::pair<std::string,std::string> >&);
	void test();
	void fireMascaretWindowsCreated();
	void fireMascaretModelLoaded();
	void fireMascaretStep();
	void fireMascaretDestroying();
protected:
	void fire(const std::string& message);
	std::string _generateCppCodePlugin(const std::vector<std::pair<std::string,std::string> >& behaviors);
	void _generateCppFile(const std::string& pluginName,const std::string& data);
	void _generatePlugin(const std::string& pluginName);
	/* ------ */

protected:
	PluginManager();
	static PluginManager * _instance;
	std::vector< void *> _libs;
	std::string _pluginDir;
	bool _init;

	//	std::vector<std::string,std::string> _codeFromBehavior;
};
}
#endif
