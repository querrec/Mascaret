/*
 * OgreWindow3D.cpp
 *
 *  Created on: 25 févr. 2011
 *      Author: soler
 */
#include "ogreMascaret_plateform.h"
#ifdef _WIN32
#include <winsock2.h>
#endif
#include "VRPlateform/OgreWindow3D.h"
#include "VRPlateform/OgreMascaretApplication.h"
#include "VRPlateform/OgreCamera.h"
#include "Interaction/OgreMouse.h"
#include "Interaction/OgreKeyboard.h"
#include "Interaction/OgreCursor.h"
#include "VEHA/Entity/Entity.h"

using namespace Ogre;
namespace OgreMascaret
{
OgreWindow3D::OgreWindow3D() :
		_mouse(NULL),
		_keyboard(NULL),
		_inputSystem(NULL),
		_isClosed(false),
		_captureMouse(false)
{
  static int i=0;
  if(!i)
  {
    _window=OgreMascaretApplication::getInstance()->getRoot()->getAutoCreatedWindow();
    i++;
  }
  else
  {
    char name[16];
    snprintf(name, 16, "Ogre Mascaret %i", i++);
    _window=OgreMascaretApplication::getInstance()->getRoot()->createRenderWindow(name, 800, 600, false);
  }
	_masKeyboard = shared_ptr<OgreKeyboard>(new OgreKeyboard());
	_masMouse = shared_ptr<OgreMouse>(new OgreMouse());
  //if(i>1)
  //{
  //  char name[16];
  //  snprintf(name, 16, "keyboard%i", i);
  //  _masKeyboard->setName(name);
  //  snprintf(name, 16, "mouse%i", i);
  //  _masMouse->setName(name);
  //}
	addPeripheric(_masKeyboard);
	addPeripheric(_masMouse);
	_createInputSystem();
	windowResized(_window);
}
OgreWindow3D::~OgreWindow3D()
{
  if (_cameraMan) delete _cameraMan;
}

void OgreWindow3D::getPosition(unsigned int& xOut, unsigned int& yOut) const
{
	// Get window size
	unsigned int width, height, depth;
	_window->getMetrics( width, height, depth, (int&)xOut, (int&)yOut );
}
void OgreWindow3D::setPosition(unsigned int x, unsigned int y)
{
	_window->reposition((int)x, (int)y);
}

void OgreWindow3D::getSize(unsigned int& xOut, unsigned int& yOut) const
{
	// Get window size
	unsigned int depth;
	int left, top;
	_window->getMetrics( xOut, yOut, depth, left, top );
}
void OgreWindow3D::setSize(unsigned int x, unsigned int y)
{
	_window->resize(x, y);

	vector<shared_ptr<IEHA::Cursor> > cursors = getCursors();
	for (vector<shared_ptr<IEHA::Cursor> >::iterator it = cursors.begin(); it != cursors.end(); it++)
	{
		shared_dynamic_cast<OgreMascaret::OgreCursor>(*it)->setWindowDimensions(x, y);
	}
}
bool OgreWindow3D::getDecoration() const
{
	cerr << "OgreWindow3D::getDecoration: not implemented" <<endl;
	return true;
}
void OgreWindow3D::setDecoration(bool deco)
{
	cerr << "OgreWindow3D::setDecoration: not implemented" <<endl;
}
bool OgreWindow3D::getMouseCapture() const
{
	return _captureMouse;
}
void OgreWindow3D::setMouseCapture(bool captureMouse)
{
	cerr << "OgreWindow3D::setMouseCapture: not implemented" <<endl;
	return;
	/*
	This code doesn't work. The mouse is not working after that
	if(captureMouse != _captureMouse)
	{
		_captureMouse=captureMouse;
		_destroyInputSystem();
		_createInputSystem();
	}*/
}

void OgreWindow3D::enableSimpleInteractor(bool simpleInteractor)
{
  if (simpleInteractor)
  {
    //_cameraMan = new CameraMan(shared_dynamic_cast<OgreCamera>(_camera)->accessCamera(), _keyboard);
    _cameraMan = new CameraMan(shared_dynamic_cast<OgreCamera>(_camera), _keyboard);
    _cameraMan->setInteracting(true);
    _cameraMan->setTopSpeed(2.0);

    _masMouse->enableCameraMan(true, _cameraMan);
  }
  else
    _masMouse->enableCameraMan(false);
}

void OgreWindow3D::windowResized(RenderWindow* rw)
{
	if(rw == _window)
	{
		// Get window size
		unsigned int width, height, depth;
		int left, top;
		_window->getMetrics( width, height, depth, left, top );

		// Set mouse region
		const OIS::MouseState &mouseState = _mouse->getMouseState();
		mouseState.width  = width;
		mouseState.height = height;

		vector<shared_ptr<IEHA::Cursor> > cursors = getCursors();
		for (vector<shared_ptr<IEHA::Cursor> >::iterator it = cursors.begin(); it != cursors.end(); it++)
		{
			shared_dynamic_cast<OgreMascaret::OgreCursor>(*it)->setWindowDimensions(width, height);
		}
	}
}

//Unattach OIS before window shutdown (very important under Linux)
void OgreWindow3D::windowClosed(RenderWindow* rw)
{
	if(rw == _window)
	{
		_destroyInputSystem();
		_isClosed=true;
	}
}
void OgreWindow3D::_createInputSystem()
{
	cout << "=====================  OgreWindow3D::_createInputSystem()" << endl;

	using namespace OIS;
	LogManager::getSingletonPtr()->logMessage("*** Initializing OIS ***");
	ParamList pl;
	size_t windowHnd = 0;
	std::ostringstream windowHndStr;

	_window->getCustomAttribute("WINDOW", &windowHnd);
	windowHndStr << windowHnd;
	pl.insert(std::make_pair(std::string("WINDOW"), windowHndStr.str()));
	if(!_captureMouse)
	{
#ifdef _WIN32
		pl.insert(std::make_pair(std::string("w32_mouse"), std::string("DISCL_FOREGROUND" )));
		pl.insert(std::make_pair(std::string("w32_mouse"), std::string("DISCL_NONEXCLUSIVE")));
#else
		pl.insert(std::make_pair(std::string("x11_mouse_grab"), std::string("false")));
		pl.insert(std::make_pair(std::string("x11_mouse_hide"), std::string("false")));
		pl.insert(std::make_pair(std::string("x11_keyboard_grab"), std::string("false")));
//		pl.insert(std::make_pair(std::string("XAutoRepeatOn"), std::string("true")));
#endif
	}
	else
	{
		cerr << "Capturing mouse" << endl;
#ifdef _WIN32
//		pl.insert(std::make_pair(std::string("w32_mouse"), std::string("DISCL_FOREGROUND" )));
//		pl.insert(std::make_pair(std::string("w32_mouse"), std::string("DISCL_NONEXCLUSIVE")));
#else
		pl.insert(std::make_pair(std::string("x11_mouse_grab"), std::string("true")));
		pl.insert(std::make_pair(std::string("x11_mouse_hide"), std::string("true")));
		pl.insert(std::make_pair(std::string("x11_keyboard_grab"), std::string("true")));
#endif
	}
	//Register as a Window listener
	WindowEventUtilities::addWindowEventListener(_window, this);

	//shared_ptr<OgreKeyboard> keyboard=shared_dynamic_cast<OgreKeyboard>(OgreMascaretApplication::getInstance()->getPeripheric("keyboard"));
	//shared_ptr<OgreMouse> mouse=shared_dynamic_cast<OgreMouse>(OgreMascaretApplication::getInstance()->getPeripheric("mouse"));

	// Create inputsystem
	_inputSystem = OIS::InputManager::createInputSystem( pl );

	// If possible create a buffered keyboard
	//if( _inputSystem->numKeyboards() > 0 ) {
	if (_inputSystem->getNumberOfDevices(OIS::OISKeyboard) > 0)
	{
		_keyboard = static_cast<OIS::Keyboard*>( _inputSystem->createInputObject( OIS::OISKeyboard, true ) );
		_keyboard->setEventCallback( _masKeyboard.get() );
	}

	// If possible create a buffered mouse
	//if( _inputSystem->numMice() > 0 ) {
	cerr << "===== Number of mouse : " << _inputSystem->getNumberOfDevices(OIS::OISMouse);
	if (_inputSystem->getNumberOfDevices(OIS::OISMouse) > 0)
	{
		_mouse = static_cast<OIS::Mouse*>( _inputSystem->createInputObject( OIS::OISMouse, true ) );
		_mouse->setEventCallback( _masMouse.get() );
		shared_ptr<OgreCursor> cursor(new OgreCursor(_mouse));

		cursor->setImage("cross.png"); //FIXME: this data file MUST be here !
		cursor->setOffset(-6.,-6.);
		cursor->setVisible(true);
		cursor->setWindowDimensions(_window->getWidth(), _window->getHeight());
		cerr << "Cursor added ! =========== " << endl;

		//cursor->setRenderer(shared_dynamic_cast<OgreWindow3D>(getWindow())->getOgreViewer());
		addCursor(cursor);
	}
}
void OgreWindow3D::_destroyInputSystem()
{
	if(_mouse)
	{
		_inputSystem->destroyInputObject(_mouse);
		_mouse=NULL;
	}
	if(_keyboard)
	{
		_inputSystem->destroyInputObject(_keyboard);
		_keyboard=NULL;
	}
	OIS::InputManager::destroyInputSystem(_inputSystem);
	_inputSystem=NULL;
	//OgreMascaretApplication::getInstance()->removeCursor(OgreMascaretApplication::getInstance()->getCursor("system"));
}

void OgreWindow3D::hide()
{}
void OgreWindow3D::showText(string text, float ratio)
{}
void OgreWindow3D::showTextWide(string text, float ratio)
{}
void OgreWindow3D::showBalloon(string text,shared_ptr<Entity> e)
{}

}
