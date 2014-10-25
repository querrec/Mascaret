/*
 * AReViWindow3D.h
 *
 *  Created on: 24 févr. 2011
 *      Author: soler
 */

#ifndef OGREWINDOW3D_H_
#define OGREWINDOW3D_H_
#include "ogreMascaret_plateform.h"
#include "IEHA/Visualisation/Window3D.h"
#include <Ogre.h>
#include <OIS.h>

#include "cameraMan.h"

namespace OgreMascaret
{
  class OgreMouse;
  class OgreKeyboard;

  using VEHA::Entity;

class OGRE_MASCARET_API OgreWindow3D : public IEHA::Window3D, public Ogre::WindowEventListener
{
public:
	OgreWindow3D();
	virtual ~OgreWindow3D();

  inline Ogre::RenderWindow* getWindow(void) {return _window;}

	virtual void getPosition(unsigned int& xOut, unsigned int& yOut) const;
	virtual void setPosition(unsigned int x, unsigned int y);

	virtual void getSize(unsigned int& xOut, unsigned int& yOut) const;
	virtual void setSize(unsigned int x, unsigned int y);

	virtual bool getDecoration() const;
	virtual void setDecoration(bool deco);

	virtual bool getMouseCapture() const;
	virtual void setMouseCapture(bool captureMouse);

	virtual void enableSimpleInteractor(bool simpleInteractor);

	virtual void windowResized(Ogre::RenderWindow* rw);

	//Unattach OIS before window shutdown (very important under Linux)
	virtual void windowClosed(Ogre::RenderWindow* rw);

	void showOperations(boost::shared_ptr<VEHA::Entity>, boost::shared_ptr<VEHA::Environment>) const {}
    
	void showSignals(boost::shared_ptr<VEHA::Entity>, boost::shared_ptr<VEHA::Environment>) const {}

	virtual void hide();
      	virtual void showText(string text, float ratio);
      	virtual void showTextWide(string text, float ratio);
      	virtual void showBalloon(string text,shared_ptr<Entity> e);

		virtual void showImage(string url) {}

	bool isClosed() const;
protected:
	virtual void _createInputSystem();
	virtual void _destroyInputSystem();
	friend class OgreCamera;
	Ogre::RenderWindow* _window;
  shared_ptr<OgreMouse> _masMouse;
  shared_ptr<OgreKeyboard> _masKeyboard;
  OIS::Mouse        *_mouse;
  OIS::Keyboard     *_keyboard;
  OIS::InputManager *_inputSystem;
  bool _isClosed;
  bool _captureMouse;

  CameraMan* _cameraMan;
};

inline bool OgreWindow3D::isClosed() const
{
	return _isClosed;
}
}
#endif /* OGREWINDOW3D_H_ */
