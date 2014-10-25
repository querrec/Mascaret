/*
 * OgreScene.cpp
 *
 *  Created on: 28 févr. 2011
 *      Author: soler
 */

#include "VRPlateform/OgreScene.h"
#include "VRPlateform/OgreWindow3D.h"
#include "IEHA/Visualisation/SkyBox.h"
#include "VRPlateform/OgreLight.h"
#include "VRPlateform/OgreSoundSpecification.h"
#include "VRPlateform/OgreMascaretApplication.h"
//#include <Ogre.h>
using namespace IEHA;
using namespace VEHA;

namespace OgreMascaret
{

OgreScene::OgreScene(const string& name) :
    Scene(name)
{

}

OgreScene::~OgreScene()
{

}

void OgreScene::setSkybox(shared_ptr<SkyBox> skybox)
{
  //skyboxShape=shared_ptr<OgreShapeSpecification>(new OgreShapeSpecification("skybox",skybox->getUrl()));
  //skyboxShape->_obj->setPosition(skybox->getPosition().x,skybox->getPosition().y,skybox->getPosition().z);
  //skyboxShape->_obj->setOrientation(skybox->getOrientation().x,skybox->getOrientation().y,skybox->getOrientation().z);
  //_scene->addObject(skyboxShape->_obj);

  OgreMascaretApplication::getInstance()->getSceneManager()->setSkyBox(true, skybox->getUrl(),
                                                                       1000);
//  OgreMascaretApplication::getInstance()->getSceneManager()->getSkyBoxNode()->rotate(
//      Ogre::Vector3(1, 0, 0), Ogre::Radian(1.57));

}

shared_ptr<SkyBox> OgreScene::getSkybox(void)
{
  return _skybox;
}

void OgreScene::addLight(shared_ptr<Light> light, Layer)
{
  //_scene->addLight(shared_dynamic_cast<OgreLight>(light)->_light);
}

void OgreScene::removeLight(shared_ptr<Light> light, Layer)
{
  //_scene->removeLight(shared_dynamic_cast<OgreLight>(light)->_light);
}

vector<shared_ptr<Light> > OgreScene::getLights() const
{
  vector<shared_ptr<Light> > lights;
  return lights;
}

void OgreScene::addEntity(shared_ptr<Entity> entity, Layer)
{
  shared_ptr<OgreShapeSpecification> shape = shared_dynamic_cast<OgreShapeSpecification>(
      entity->getActiveShape());
  if (shape)
  {
    //_scene->addObject(shape->_obj);
  }
#if 0
  vector<shared_ptr<SoundSpecification> > sounds = entity->getSounds();
  for(size_t i = 0; i < sounds.size(); i++)
  {
    cout << "Sound" << entity->getFullName() << ":" << shared_dynamic_cast<OgreSoundSpecification>(sounds[i])->getName() << endl;
    //_scene->addSound(shared_dynamic_cast<OgreSoundSpecification>(sounds[i])->getSound());
  }
#endif
}

void OgreScene::removeEntity(shared_ptr<Entity> entity, Layer)
{
  shared_ptr<OgreShapeSpecification> shape = shared_dynamic_cast<OgreShapeSpecification>(
      entity->getActiveShape());
  if (shape)
  {
    //_scene->removeObject(shape->_obj);
  }
}

VEHA::Color OgreScene::getFogColor() const
{
  return _fogColor;
}

void OgreScene::setFogColor(const VEHA::Color& color)
{
  _fogColor = color;
  updateFog();
}

unsigned int OgreScene::getFogDegree() const
{
  return _fogDegree;
}

double OgreScene::getFogDensity() const
{
  return _fogDensity;
}

void OgreScene::setFog(unsigned int degree, double density)
{
  _fogDegree = (degree > 2 ? 2 : degree);
  _fogDensity = (_fogDegree && (density < 0.0) ? 0.0 : density);

  updateFog();
}

void OgreScene::updateFog()
{
  Ogre::SceneManager* scnMgr = OgreMascaretApplication::getInstance()->getSceneManager();

  Ogre::ColourValue c(_fogColor.r, _fogColor.g, _fogColor.b);
  cerr << "FogColor:" << c << endl;

  switch (_fogDegree)
  {
    case 0:
    {
      double start, end;
      if (_fogDensity > 0.0)
      {
        start = 5;
        end = start + (1000 - start) / _fogDensity;
//                    start=_perspective.getNearDistance();
//                    end=start+(_perspective.getFarDistance()-start)/scene->_fogDensity;
      }
      else
      {
        start = 1000;
        end = start + (start - 5) / _fogDensity;
//                    start=_perspective.getFarDistance();
//                    end=start+(start-_perspective.getNearDistance())/scene->_fogDensity;
      }
      scnMgr->setFog(Ogre::FOG_LINEAR, c, 0.0, start, end);
      break;
    }
    case 1:
    {
      //        scene->_fogDensity/_perspective.getFarDistance());
      scnMgr->setFog(Ogre::FOG_EXP, c, _fogDensity / 1000);
      break;
    }
    case 2:
    {
      //        scene->_fogDensity/_perspective.getFarDistance());
      scnMgr->setFog(Ogre::FOG_EXP2, c, _fogDensity / 1000);
      break;
    }
  }
}

}

