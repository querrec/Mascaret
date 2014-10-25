/*
 * AReViKeyboard.cpp
 *
 *  Created on: 3 mars 2011
 *      Author: soler
 */

#include "Interaction/AReViKeyboard.h"
using namespace IEHA;
using namespace AReVi;
namespace AReViMascaret
{
// --- AReViKeyboardListener
AR_CLASS_NOVOID_DEF(AReViKeyboardListener,ArObject)
AReViKeyboardListener::AReViKeyboardListener(ArCW& c,AReViKeyboard* keyboard) : ArObject(c), _keyboard(keyboard)
{

}
AReViKeyboardListener::~AReViKeyboardListener()
{

}
void AReViKeyboardListener::onKeyboardInteraction(const AReVi::Renderer3D::KeyboardEvent& e)
{
	_keyboard->onKeyboardInteraction(e);
}

// --- AReViKeyboard
AReViKeyboard::AReViKeyboard()
: PaddlePeripheric("keyboard"),
  _keyboardListener(AReViKeyboardListener::NEW(this))
{
	for(size_t i=32;i<127;i++)
	{
		string s="";
		s+=(char)i;
		shared_ptr<Button> button(new Button(s));
		addButton(button);
	}
	shared_ptr<Button> button(new Button("Up"));
	addButton(button);
	button=shared_ptr<Button>(new Button("Down"));
	addButton(button);
	button=shared_ptr<Button>(new Button("Left"));
	addButton(button);
	button=shared_ptr<Button>(new Button("Right"));
	addButton(button);
	button=shared_ptr<Button>(new Button("F1"));
	addButton(button);
	button=shared_ptr<Button>(new Button("F2"));
	addButton(button);
	button=shared_ptr<Button>(new Button("F3"));
	addButton(button);
	button=shared_ptr<Button>(new Button("F4"));
	addButton(button);
	button=shared_ptr<Button>(new Button("F5"));
	addButton(button);
	button=shared_ptr<Button>(new Button("F6"));
	addButton(button);
	button=shared_ptr<Button>(new Button("F7"));
	addButton(button);
	button=shared_ptr<Button>(new Button("F8"));
	addButton(button);
	button=shared_ptr<Button>(new Button("F9"));
	addButton(button);
	button=shared_ptr<Button>(new Button("F10"));
	addButton(button);
	button=shared_ptr<Button>(new Button("F11"));
	addButton(button);
	button=shared_ptr<Button>(new Button("F12"));
	addButton(button);

}
AReViKeyboard::~AReViKeyboard()
{

}
void AReViKeyboard::onKeyboardInteraction(const AReVi::Renderer3D::KeyboardEvent& e)
{
	shared_ptr<Button> b=getButton(e.key);
	if(b)
	{
		b->setPressed(e.pressed);
	}
	else
	{
		cerr << "button "<<e.key << " not found"<<endl;
	}
}

}
