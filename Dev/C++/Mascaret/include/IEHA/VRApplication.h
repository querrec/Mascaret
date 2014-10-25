/*
 * VRApplication.h
 *
 *  Created on: 26 févr. 2011
 *      Author: ping
 */

#ifndef VRAPPLICATION_H_
#define VRAPPLICATION_H_
#include "Tools/veha_plateform.h"
//#include "IEHA/Visualisation/Camera.h"
#include "IEHA/Visualisation/Window3D.h"
#include "IEHA/Visualisation/Scene.h"
#include "MascaretApplication.h"
#include "Tools/Loaders/XmlParser.h"

namespace IEHA
{
class VEHA_API VRApplication : public MascaretApplication
{
public:
	VRApplication();
	virtual ~VRApplication();

	virtual void parse(const string& name);

	virtual bool step();
	inline const vector<shared_ptr<Window3D> >& getWindows() const;

	shared_ptr<Scene> getScene(const string& name);
	vector<shared_ptr<Scene> > getScenes();
	void addScene(shared_ptr<Scene> scene);
	void removeScene(const string& name);

  virtual void sendSignal(shared_ptr<VEHA::Entity> entity, string signalName, map<string, shared_ptr<ValueSpecification> > values);
	inline void addFunctionCallBack(SigCallFunc func){_vecFunction.push_back(func);}

	static VRApplication* getInstance();
protected:
	virtual void parse(shared_ptr<XmlNode> root);
	virtual shared_ptr<Scene> parseScene(shared_ptr<XmlNode> sceneNode);
	virtual shared_ptr<Window3D> parseRenderer(shared_ptr<XmlNode> rendererNode);
	virtual void parseInteractions(shared_ptr<XmlNode> interactionNode);
	virtual void parseNavigation(shared_ptr<XmlNode> navigationNode);
	virtual void parsePeripherics(shared_ptr<XmlNode> periphNode);

	virtual void parseDistributionServer(shared_ptr<XmlNode> disNode);
	virtual void parseClient(shared_ptr<XmlNode> clientNode);

	virtual void addEntitiesToScene();

	virtual void createDistributionServer(const string& sessionName, int port)=0;
	virtual void createClient(const string& hostname, int port)=0;

	virtual void _parseFog(shared_ptr<Scene>, shared_ptr<XmlNode> fogNode);
	virtual void _parseLight(shared_ptr<Scene>, shared_ptr<XmlNode> lightNode);

protected:
  vector<shared_ptr<Window3D> > _windows;
	vector<shared_ptr<Camera> > _cameras;
	vector<shared_ptr<Scene> > _scenes;
	vector<string> _startViewPoint;
	vector<string> _startScene;

  map<string, shared_ptr<Signal> > _sigs;
	vector<SigCallFunc> _vecFunction;
  void _getSignals(shared_ptr<VEHA::Entity> entity);
private:
	static VRApplication * _instance;
};
inline shared_ptr<Scene> VRApplication::getScene(const string& name)
{
	for(size_t i=0;i<_scenes.size();i++)
	{
		if(_scenes[i] && _scenes[i]->getName() == name)
			return _scenes[i];
	}
	return shared_ptr<Scene>();
}
inline vector<shared_ptr<Scene> > VRApplication::getScenes()
{
	return _scenes;
}
inline void VRApplication::addScene(shared_ptr<Scene> scene)
{
	_scenes.push_back(scene);
}
inline void VRApplication::removeScene(const string& name)
{
	size_t i=0;
	for(;i<_scenes.size();i++)
	{
		if(_scenes[i] && _scenes[i]->getName() == name)
			break;
	}
	if(i<_scenes.size())
	{
		_scenes[i]=_scenes[_scenes.size()-1];
		_scenes.pop_back();
	}
}
inline VRApplication* VRApplication::getInstance()
{
	return _instance;
}
inline const vector<shared_ptr<Window3D> >& VRApplication::getWindows() const
{
  return _windows;
}
}

#endif /* VRAPPLICATION_H_ */
