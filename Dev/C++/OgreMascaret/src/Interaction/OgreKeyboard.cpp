/*
 * AReViKeyboard.cpp
 *
 *  Created on: 3 mars 2011
 *      Author: soler
 */

#include "Interaction/OgreKeyboard.h"
using namespace IEHA;

namespace OgreMascaret
{
string keycodeToButton(OIS::KeyCode k);

OgreKeyboard::OgreKeyboard()
: PaddlePeripheric("keyboard")
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
OgreKeyboard::~OgreKeyboard()
{

}
bool OgreKeyboard::keyPressed(const OIS::KeyEvent &arg)
{
	string key;
	key+=keycodeToButton(arg.key);
	shared_ptr<Button> button=getButton(key);
	if(button)
	{
		button->setPressed(true);
	}
	return false;
}
bool OgreKeyboard::keyReleased(const OIS::KeyEvent &arg)
{
	string key;
	key+=keycodeToButton(arg.key);
	shared_ptr<Button> button=getButton(key);
	if(button)
	{
		button->setPressed(false);
	}
	return false;
}
string keycodeToButton(OIS::KeyCode k)
{
	// TODO: voir pourquoi on recoit du qwerty sous windows !!
	string text="";
	switch(k)
	{
	case OIS::KC_NUMPAD0:
	case OIS::KC_0:
		text="0";
		break;
	case OIS::KC_NUMPAD1:
	case OIS::KC_1:
		text="1";
		break;
	case OIS::KC_NUMPAD2:
	case OIS::KC_2:
		text="2";
		break;
	case OIS::KC_NUMPAD3:
	case OIS::KC_3:
		text="3";
		break;
	case OIS::KC_NUMPAD4:
	case OIS::KC_4:
		text="4";
		break;
	case OIS::KC_NUMPAD5:
	case OIS::KC_5:
		text="5";
		break;
	case OIS::KC_NUMPAD6:
	case OIS::KC_6:
		text="6";
		break;
	case OIS::KC_NUMPAD7:
	case OIS::KC_7:
		text="7";
		break;
	case OIS::KC_NUMPAD8:
	case OIS::KC_8:
		text="8";
		break;
	case OIS::KC_NUMPAD9:
	case OIS::KC_9:
		text="9";
		break;
	case OIS::KC_A:
		text="a";
		break;
	case OIS::KC_B:
		text="b";
		break;
	case OIS::KC_C:
		text="c";
		break;
	case OIS::KC_D:
		text="d";
		break;
	case OIS::KC_E:
		text="e";
		break;
	case OIS::KC_F:
		text="f";
		break;
	case OIS::KC_G:
		text="g";
		break;
	case OIS::KC_H:
		text="h";
		break;
	case OIS::KC_I:
		text="i";
		break;
	case OIS::KC_J:
		text="j";
		break;
	case OIS::KC_K:
		text="k";
		break;
	case OIS::KC_L:
		text="l";
		break;
	case OIS::KC_M:
		text="m";
		break;
	case OIS::KC_N:
		text="n";
		break;
	case OIS::KC_O:
		text="o";
		break;
	case OIS::KC_P:
		text="p";
		break;
	case OIS::KC_Q:
		text="q";
		break;
	case OIS::KC_R:
		text="r";
		break;
	case OIS::KC_S:
		text="s";
		break;
	case OIS::KC_T:
		text="t";
		break;
	case OIS::KC_U:
		text="u";
		break;
	case OIS::KC_V:
		text="v";
		break;
	case OIS::KC_W:
		text="w";
		break;
	case OIS::KC_X:
		text="x";
		break;
	case OIS::KC_Y:
		text="y";
		break;
	case OIS::KC_Z:
		text="z";
		break;
	case OIS::KC_LEFT:
		text="Left";
		break;
	case OIS::KC_RIGHT:
		text="Right";
		break;
	case OIS::KC_UP:
		text="Up";
		break;
	case OIS::KC_DOWN:
		text="Down";
		break;
	//TODO: others
	};
	return text;
}
}