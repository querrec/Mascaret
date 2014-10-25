/*
 * OgreScene.h
 *
 *  Created on: 28 févr. 2011
 *      Author: soler
 */

#ifndef OGRESCENE_H_
#define OGRESCENE_H_
#include "ogreMascaret_plateform.h"
#include "IEHA/Visualisation/Scene.h"
#include "VRPlateform/OgrePointSpecification.h"
#include "VRPlateform/OgreShapeSpecification.h"

namespace OgreMascaret
{

using std::string;
using IEHA::SkyBox;
using IEHA::Light;

class OGRE_MASCARET_API OgreScene : public IEHA::Scene
{
public:
	OgreScene(const string& name);
	virtual ~OgreScene();

	virtual void setSkybox(shared_ptr<SkyBox> skybox);
	virtual shared_ptr<SkyBox> getSkybox(void);

	virtual void addLight(shared_ptr<Light> light, Layer l=Main);
	virtual void removeLight(shared_ptr<Light> light, Layer l=Main);
	virtual vector<shared_ptr<Light> > getLights() const;

	virtual void addEntity(shared_ptr<Entity> entity, Layer l=Main);
	virtual void removeEntity(shared_ptr<Entity> entity, Layer l=Main);

	virtual VEHA::Color getFogColor() const;
	virtual void setFogColor(const VEHA::Color& color);

	virtual unsigned int getFogDegree() const;
	virtual double getFogDensity() const;
	virtual void setFog(unsigned int degree, double density);
	void updateFog();

protected:
	shared_ptr<SkyBox> _skybox;
	shared_ptr<OgreShapeSpecification> skyboxShape;
	VEHA::Color _fogColor;
	double _fogDensity;
	unsigned int _fogDegree;
};

}
#endif /* AREVISCENE_H_ */
