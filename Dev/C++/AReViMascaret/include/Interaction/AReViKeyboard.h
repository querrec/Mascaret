/*
 * AReViKeyboard.h
 *
 *  Created on: 3 mars 2011
 *      Author: soler
 */

#ifndef AREVIKEYBOARD_H_
#define AREVIKEYBOARD_H_

#include "AReVi/Lib3D/renderer3D.h"

#include "IEHA/Interaction/PaddlePeripheric.h"

namespace AReViMascaret
{
class AReViKeyboard;
class AReViKeyboardListener : public AReVi::ArObject
{
public:
	AR_CLASS(AReViKeyboardListener)
	AR_CONSTRUCTOR_1(AReViKeyboardListener,AReViKeyboard*, keyboard)

	inline AReVi::ArRef<AReVi::Renderer3D> getRenderer();
	inline void setRenderer(AReVi::ArRef<AReVi::Renderer3D> renderer);

protected:
	void onKeyboardInteraction(const AReVi::Renderer3D::KeyboardEvent& e);
	AReVi::ArRef<AReVi::Renderer3D> _renderer;
	AReViKeyboard* _keyboard;
};
class AReViKeyboard : public IEHA::PaddlePeripheric
{
public:
	AReViKeyboard();
	virtual ~AReViKeyboard();

	inline AReVi::ArRef<AReVi::Renderer3D> getRenderer();
	inline void setRenderer(AReVi::ArRef<AReVi::Renderer3D> renderer);
protected:
	friend class AReViKeyboardListener;
	void onKeyboardInteraction(const AReVi::Renderer3D::KeyboardEvent& e);
	AReVi::ArRef<AReViKeyboardListener> _keyboardListener;
};
AReVi::ArRef<AReVi::Renderer3D> AReViKeyboardListener::getRenderer()
{
	return _renderer;
}
void AReViKeyboardListener::setRenderer(AReVi::ArRef<AReVi::Renderer3D> renderer)
{
	if(_renderer == renderer)
		return;
	if(_renderer)
	{
		_renderer->removeKeyboardCB(thisRef(),&AReViKeyboardListener::onKeyboardInteraction);
	}
	_renderer=renderer;
	if(_renderer)
	{
		_renderer->addKeyboardCB(thisRef(),&AReViKeyboardListener::onKeyboardInteraction);
	}
}
AReVi::ArRef<AReVi::Renderer3D> AReViKeyboard::getRenderer()
{
	return _keyboardListener->getRenderer();
}
void AReViKeyboard::setRenderer(AReVi::ArRef<AReVi::Renderer3D> renderer)
{
	_keyboardListener->setRenderer(renderer);
}
}

#endif /* AREVIKEYBOARD_H_ */
