/*
 * AReViCursor.cpp
 *
 *  Created on: 4 mars 2011
 *      Author: soler
 */

#include "Interaction/OgreCursor.h"

#include <iostream>

#include <OGRE/OgrePanelOverlayElement.h>
#include <OGRE/OgreTextAreaOverlayElement.h>
#include <OGRE/OgreMaterialManager.h>
#include <OGRE/OgreOverlay.h>
#include <OGRE/OgreOverlayContainer.h>
#include <OGRE/OgreOverlayManager.h>
#include <OGRE/OgreTextureManager.h>
#include <OGRE/OgreTechnique.h>
#include <OGRE/OgrePass.h>
#include <OGRE/OgreTextureUnitState.h>
#include <OIS/OISMouse.h>

namespace OgreMascaret
{

int OgreCursor::nbCursor = 0;

OgreCursor::OgreCursor(OIS::Mouse* mouse) :
    Cursor("system"),
    _guiOverlay(0),
    _cursorContainer(0),
    _mouse(mouse)
{
  _id = nbCursor;
  nbCursor++;

  std::ostringstream cursorName;
  cursorName << "MouseCursor_" << _id ;

  _material = Ogre::MaterialManager::getSingleton().create("MouseCursor/default", "General");
  _cursorContainer = static_cast<Ogre::OverlayContainer*>(Ogre::OverlayManager::getSingleton().createOverlayElement(
      "Panel", cursorName.str() ));
  _cursorContainer->setMaterialName(_material->getName());
  _cursorContainer->setPosition(0, 0);

  _guiOverlay = Ogre::OverlayManager::getSingleton().create(cursorName.str());
  _guiOverlay->setZOrder(649);
  _guiOverlay->add2D(_cursorContainer);
  _guiOverlay->show();

  wheelUp = 0;
  wheelDown = 0;
  wheelPos = _mouse->getMouseState().Z.abs;
  wheelPosStep = 120;
  deltaWheel = 0;
}

OgreCursor::~OgreCursor()
{

}

void OgreCursor::getPosition(int& x, int& y) const
{
  x = _mouse->getMouseState().X.abs + 7; //FIXME: ugly hack to compensate offset (-7, -7) from OIS Mouse
  y = _mouse->getMouseState().Y.abs + 7;
}

void OgreCursor::setImage(const std::string& filename)
{
  //std::cerr << __PRETTY_FUNCTION__ << filename << std::endl;
  _texture = Ogre::TextureManager::getSingleton().load(filename, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME); //"General"

  Ogre::TextureUnitState *pTexState;
  if (_material->getTechnique(0)->getPass(0)->getNumTextureUnitStates())
  {
    pTexState = _material->getTechnique(0)->getPass(0)->getTextureUnitState(0);
  }
  else
  {
    pTexState = _material->getTechnique(0)->getPass(0)->createTextureUnitState(_texture->getName());
  }
  pTexState->setTextureAddressingMode(Ogre::TextureUnitState::TAM_CLAMP);
  pTexState->setTextureName(filename);
  _material->getTechnique(0)->getPass(0)->setSceneBlending(Ogre::SBT_TRANSPARENT_ALPHA);
}

void OgreCursor::setWindowDimensions(unsigned int width, unsigned int height)
{
  _windowWidth = (width > 0) ? width : 1;
  _windowHeight = (height > 0) ? height : 1;

  _cursorContainer->setWidth(_texture->getWidth() / _windowWidth);
  _cursorContainer->setHeight(_texture->getHeight() / _windowHeight);
}

void OgreCursor::setVisible(bool visible)
{
  if (visible)
  {
    _guiOverlay->show();
    _cursorContainer->show();
  }
  else
  {
    _guiOverlay->hide();
    _cursorContainer->hide();
  }
}

void OgreCursor::getWheelPos(int & wheel)
{
  wheel = _mouse->getMouseState().Z.abs;
  //std::cerr << __PRETTY_FUNCTION__ << " " << _mouse->getMouseState().Z.abs << " " << _mouse->getMouseState().Z.rel << std::endl;
}

void OgreCursor::update()
{
  int x,y, w;
  getPosition(x, y);
  updatePosition(x-7, y-7); //FIXME: ugly hack to compensate offset (-7, -7) from OIS Mouse

  getWheelPos(w);

  deltaWheel += (wheelPos - w);
  int upDown = deltaWheel / wheelPosStep;
  //std::cerr << "upDown=" << upDown << std::endl;

  if (upDown > 0)
  {
    wheelDown += upDown;
    deltaWheel -= upDown * wheelPosStep;
  }
  else if (upDown < 0)
  {
    wheelUp -= upDown;
    deltaWheel -= upDown * wheelPosStep;
  }

  wheelPos = w;

  //update button status
  OIS::MouseState ms = _mouse->getMouseState();
  leftButton = ms.buttonDown(OIS::MB_Left);
  rightButton = ms.buttonDown(OIS::MB_Right);
  wheelButton = ms.buttonDown(OIS::MB_Middle);
}

void OgreCursor::updatePosition(int x, int y)
{
  Ogre::Real rx = (x+_xOffset) / _windowWidth;
  Ogre::Real ry = (y+_yOffset) / _windowHeight;
  //std::cout << "ois cursor: " << x << " " << y << " overlay cursor: " << rx << " "<< ry << std::endl;
  _cursorContainer->setPosition(clamp(rx, 0.0f, 1.0f), clamp(ry, 0.0f, 1.0f));
}

Ogre::Real OgreCursor::clamp(Ogre::Real a, Ogre::Real min, Ogre::Real max)
{
  if (a < min)
  {
    return min;
  }
  if (a > max)
  {
    return max;
  }
  return a;
}

}
