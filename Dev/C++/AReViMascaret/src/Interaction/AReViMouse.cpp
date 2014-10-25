/*
 * AReViMouse.cpp
 *
 *  Created on: 3 mars 2011
 *      Author: soler
 */

#include "Interaction/AReViMouse.h"
using namespace IEHA;
using namespace AReVi;
using namespace boost;
namespace AReViMascaret
{
// --- AReViMouseListener
AR_CLASS_NOVOID_DEF(AReViMouseListener,ArObject)
AReViMouseListener::AReViMouseListener(ArCW& c,AReViMouse* mouse) : ArObject(c), _mouse(mouse)
{

}
AReViMouseListener::~AReViMouseListener()
{

}
void AReViMouseListener::onMouseButtonInteraction(const AReVi::Renderer3D::MouseButtonEvent& e)
{
	_mouse->onMouseButtonInteraction(e);
}
void AReViMouseListener::onMouseMotionInteraction(const AReVi::Renderer3D::MouseMotionEvent& e)
{
	_mouse->onMouseMotionInteraction(e);
}


// --- AReViMouse
AReViMouse::AReViMouse()
: PaddlePeripheric("mouse"),
  _mouseListener(AReViMouseListener::NEW(this)),
  _mouseCapture(false)
{
	shared_ptr<Button> button1(new Button("button1"));
	shared_ptr<Button> button2(new Button("button2"));
	shared_ptr<Button> button3(new Button("button3"));
	shared_ptr<Axis> axis1(new Axis("x"));
	shared_ptr<Axis> axis2(new Axis("y"));
	addButton(button1);
	addButton(button2);
	addButton(button3);
	addAxis(axis1);
	addAxis(axis2);
}
AReViMouse::~AReViMouse()
{

}
void AReViMouse::onMouseButtonInteraction(const AReVi::Renderer3D::MouseButtonEvent& e)
{
	string name="button";
	name+=intToStr(e.button);
	shared_ptr<Button> button=getButton(name);
	if(button)
	{
		button->setPressed(e.pressed);
	}
}
void AReViMouse::onMouseMotionInteraction(const AReVi::Renderer3D::MouseMotionEvent& e)
{
	unsigned int w,h;
	_mouseListener->getRenderer()->getRenderSize(w,h);
	int dx=w/2-e.xMouse;
	int dy=h/2-e.yMouse;
	if(_mouseCapture)
	{
	  getAxis("x")->setValue(dx);
	  getAxis("y")->setValue(dy);
	
	  _mouseListener->getRenderer()->moveSystemCursor(dx,dy);
	}
}
}
