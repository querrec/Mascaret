/*
 * Light.h
 *
 *  Created on: 24 févr. 2011
 *      Author: soler
 */

#ifndef SCENE_H_
#define SCENE_H_
#include "Tools/veha_plateform.h"
#include "VEHA/Entity/Zone.h"
#include "VEHA/Entity/Color.h"

namespace IEHA
{

class SkyBox;
class Light;

class VEHA_API Scene
{
public:
	/**
	 * Layers where you can place an object (entity/light) in the scene.
	 */
	enum Layer
	{
		Back,
		Main,
		Front
	};

	Scene(const string& name);
	virtual ~Scene();

	inline string getName() const;
	inline void setName(const string& name);

	inline shared_ptr<VEHA::Zone> getZone() const;
	inline void setZone(shared_ptr<VEHA::Zone> zone);

	/**
	 * Skybox will always be in the back layer
	 */
	virtual void setSkybox(shared_ptr<SkyBox> skybox)=0;

	virtual void addLight(shared_ptr<Light> light, Layer l=Main)=0;
	virtual void removeLight(shared_ptr<Light> light, Layer l=Main)=0;
	virtual vector<shared_ptr<Light> > getLights() const=0;

	virtual void addEntity(shared_ptr<VEHA::Entity> entity, Layer l=Main)=0;
	virtual void removeEntity(shared_ptr<VEHA::Entity> entity, Layer l=Main)=0;

	virtual VEHA::Color getFogColor() const=0;
	virtual void setFogColor(const VEHA::Color& color)=0;

	virtual unsigned int getFogDegree() const=0;
	virtual double getFogDensity() const=0;
	virtual void setFog(unsigned int degree, double density)=0;


protected:
	string _name;
	shared_ptr<VEHA::Zone> _zone;

	VEHA::Color _fogColor;
	double _density;

};
string Scene::getName() const
{
	return _name;
}
void Scene::setName(const string& name)
{
	_name=name;
}

 shared_ptr<VEHA::Zone> Scene::getZone() const
{
	return _zone;
}
 void Scene::setZone(shared_ptr<VEHA::Zone> zone)
{
	_zone=zone;
}
}
#endif /* SCENE_H_ */
