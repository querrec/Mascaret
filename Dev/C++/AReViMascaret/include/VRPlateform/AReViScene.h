/*
 * AReViScene.h
 *
 *  Created on: 28 févr. 2011
 *      Author: soler
 */

#ifndef AREVISCENE_H_
#define AREVISCENE_H_
#include "AReVi/Lib3D/scene3D.h"
#include "areviMascaret_plateform.h"
#include "IEHA/Visualisation/Scene.h"
#include "VRPlateform/AReViPointSpecification.h"
#include "VRPlateform/AReViShapeSpecification.h"


namespace AReViMascaret
{
using std::vector;
using AReVi::ArRef;
using AReVi::Scene3D;
using std::string;
using IEHA::SkyBox;
using IEHA::Light;

class AREVI_MASCARET_API AReViScene : public IEHA::Scene
{
public:
	AReViScene(const string& name);
	virtual ~AReViScene();

	virtual void setSkybox(shared_ptr<SkyBox> skybox);

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

	ArRef<Scene3D> getScene();
	ArRef<Scene3D> getSceneBack();
	ArRef<Scene3D> getSceneFront();
protected:
	ArRef<Scene3D> _scene;
	ArRef<Scene3D> _sceneBack;
	ArRef<Scene3D> _sceneFront;
	vector<shared_ptr<Light> > _lights;
	shared_ptr<AReViShapeSpecification> skyboxShape;
};
inline ArRef<Scene3D> AReViScene::getScene()
{
	return _scene;
}
inline ArRef<Scene3D> AReViScene::getSceneBack()
{
	return _sceneBack;
}
inline ArRef<Scene3D> AReViScene::getSceneFront()
{
	return _sceneFront;
}
}
#endif /* AREVISCENE_H_ */
