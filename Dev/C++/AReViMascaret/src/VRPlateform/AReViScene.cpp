/*
 * AReViScene.cpp
 *
 *  Created on: 28 févr. 2011
 *      Author: soler
 */
#include "AReVi/Shapes/shape3D.h"
#include "VRPlateform/AReViScene.h"
#include "IEHA/Visualisation/SkyBox.h"
#include "VRPlateform/AReViLight.h"
#include "VRPlateform/AReViSoundSpecification.h"
using namespace IEHA;
using namespace VEHA;

namespace AReViMascaret
{
AReViScene::AReViScene(const string& name)
: Scene(name), _scene(Scene3D::NEW()), _sceneBack(Scene3D::NEW()), _sceneFront(Scene3D::NEW())
{

}

AReViScene::~AReViScene()
{

}

void AReViScene::setSkybox(shared_ptr<SkyBox> skybox)
{
        cerr << "ADD SKYBOX ..." << endl;
	skyboxShape=shared_ptr<AReViShapeSpecification>(new AReViShapeSpecification("skybox",skybox->getUrl()));
	skyboxShape->getObject3D()->setPosition(skybox->getPosition().x,skybox->getPosition().y,skybox->getPosition().z);
	skyboxShape->getObject3D()->setOrientation(skybox->getOrientation().x,skybox->getOrientation().y,skybox->getOrientation().z);
	_sceneBack->addObject(skyboxShape->getObject3D());
}

void AReViScene::addLight(shared_ptr<Light> light, Layer l)
{
        cerr << "Add Light ..." << endl;
	if(l == Front)
		_sceneFront->addLight(shared_dynamic_cast<AReViLight>(light)->getLight());
	else if(l == Back)
		_sceneBack->addLight(shared_dynamic_cast<AReViLight>(light)->getLight());
	else
		_scene->addLight(shared_dynamic_cast<AReViLight>(light)->getLight());
	_lights.push_back(light);
}
void AReViScene::removeLight(shared_ptr<Light> light, Layer l)
{
	if(l == Front)
		_sceneFront->removeLight(shared_dynamic_cast<AReViLight>(light)->getLight());
	else if(l == Back)
		_sceneBack->removeLight(shared_dynamic_cast<AReViLight>(light)->getLight());
	else
		_scene->removeLight(shared_dynamic_cast<AReViLight>(light)->getLight());
	// TODO: fast erase
}
vector<shared_ptr<Light> > AReViScene::getLights() const
{
	return _lights;
}

void AReViScene::addEntity(shared_ptr<Entity> entity, Layer l)
{
	shared_ptr<AReViShapeSpecification> shape=shared_dynamic_cast<AReViShapeSpecification>(entity->getActiveShape());
	if(shape)
	{
		if(l == Front)
			_sceneFront->addObject(shape->getObject3D());
		else if(l == Back)
			_sceneBack->addObject(shape->getObject3D());
		else
			_scene->addObject(shape->getObject3D());
	}
	vector<shared_ptr<SoundSpecification> > sounds=entity->getSounds();
	for(size_t i=0;i<sounds.size();i++)
	{
		_scene->addSound(shared_dynamic_cast<AReViSoundSpecification>(sounds[i])->getSound());
	}
}
void AReViScene::removeEntity(shared_ptr<Entity> entity, Layer l)
{
	shared_ptr<AReViShapeSpecification> shape=shared_dynamic_cast<AReViShapeSpecification>(entity->getActiveShape());
	if(shape)
	{
		if(l == Front)
			_sceneFront->removeObject(shape->getObject3D());
		else if(l == Back)
			_sceneBack->removeObject(shape->getObject3D());
		else
			_scene->removeObject(shape->getObject3D());
	}
}

VEHA::Color AReViScene::getFogColor() const
{
	double r,g,b;
	_scene->getFogColor(r,g,b);
	return Color(r,g,b);
}
void AReViScene::setFogColor(const VEHA::Color& color)
{
	_scene->setFogColor(color.r,color.g,color.b);
}

unsigned int AReViScene::getFogDegree() const
{
	unsigned int degre;
	double density;
	_scene->getFog(degre,density);
	return degre;
}
double AReViScene::getFogDensity() const
{
	unsigned int degre;
	double density;
	_scene->getFog(degre,density);
	return density;
}
void AReViScene::setFog(unsigned int degree, double density)
{
	_scene->setFog(degree,density);
}
}

