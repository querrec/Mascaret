/*
 * AReViMouse.cpp
 *
 *  Created on: 3 mars 2011
 *      Author: soler
 */

#include "Interaction/OgreMouse.h"
using namespace IEHA;
using namespace boost;
namespace OgreMascaret
{
OgreMouse::OgreMouse()
: PaddlePeripheric("mouse")
{
	addButton(make_shared<Button>("button1"));
	addButton(make_shared<Button>("button2"));
	addButton(make_shared<Button>("button3"));
	addAxis(make_shared<Axis>("x"));
	addAxis(make_shared<Axis>("y"));
	addAxis(make_shared<Axis>("z"));
}
OgreMouse::~OgreMouse()
{

}

bool OgreMouse::mouseMoved( const OIS::MouseEvent &arg )
{

	int x=arg.state.X.abs;
	int y=arg.state.Y.abs;
	int z=arg.state.Z.abs;
	getAxis("x")->setValue(x);
	getAxis("y")->setValue(y);
	getAxis("z")->setValue(z);

	if(_cameraMan)
	  _cameraMan->injectMouseMove(arg);

	return false;
}
bool OgreMouse::mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
{
	string name="button";
	if(id ==OIS::MB_Left)
		name+="1";
	else if(id==OIS::MB_Middle)
		name+="2";
	else
		name+="3";
	getButton(name)->setPressed(true);

  if(_cameraMan)
    _cameraMan->injectMouseDown(arg, id);

  return false;
}
bool OgreMouse::mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
{
	string name="button";
	if(id ==OIS::MB_Left)
		name+="1";
	else if(id==OIS::MB_Middle)
		name+="2";
	else
		name+="3";
	getButton(name)->setPressed(false);

	if(_cameraMan)
	  _cameraMan->injectMouseUp(arg, id);

	return false;
}

void OgreMouse::enableCameraMan(bool enable, CameraMan* cameraMan)
{
  if(enable && cameraMan)
    _cameraMan = cameraMan;
}
}
