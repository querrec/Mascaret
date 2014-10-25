/*
 * AReViMouse.h
 *
 *  Created on: 3 mars 2011
 *      Author: soler
 */

#ifndef AREVIMOUSE_H_
#define AREVIMOUSE_H_
#include "AReVi/Lib3D/renderer3D.h"

#include "IEHA/Interaction/PaddlePeripheric.h"


namespace AReViMascaret
{
class AReViMouse;
class AReViMouseListener : public AReVi::ArObject
{
public:
	AR_CLASS(AReViMouseListener)
	AR_CONSTRUCTOR_1(AReViMouseListener,AReViMouse*, mouse)

	AReVi::ArRef<AReVi::Renderer3D> getRenderer();
	void setRenderer(AReVi::ArRef<AReVi::Renderer3D> renderer);

protected:
	void onMouseButtonInteraction(const AReVi::Renderer3D::MouseButtonEvent& e);
	void onMouseMotionInteraction(const AReVi::Renderer3D::MouseMotionEvent& e);
	AReVi::ArRef<AReVi::Renderer3D> _renderer;
	AReViMouse* _mouse;
};

class AReViMouse : public IEHA::PaddlePeripheric
{
public:
	AReViMouse();
	virtual ~AReViMouse();

	AReVi::ArRef<AReVi::Renderer3D> getRenderer();
	void setRenderer(AReVi::ArRef<AReVi::Renderer3D> renderer);

	bool getMouseCapture() const;
	void setMouseCapture(bool mouseCapture);
protected:
	friend class AReViMouseListener;
	void onMouseButtonInteraction(const AReVi::Renderer3D::MouseButtonEvent& e);
	void onMouseMotionInteraction(const AReVi::Renderer3D::MouseMotionEvent& e);

	AReVi::ArRef<AReViMouseListener> _mouseListener;
	bool _mouseCapture;
};
inline AReVi::ArRef<AReVi::Renderer3D> AReViMouse::getRenderer()
{
	return _mouseListener->getRenderer();
}
inline void AReViMouse::setRenderer(AReVi::ArRef<AReVi::Renderer3D> renderer)
{
	_mouseListener->setRenderer(renderer);
}
inline bool AReViMouse::getMouseCapture() const
{
	return _mouseCapture;
}
inline void AReViMouse::setMouseCapture(bool mouseCapture)
{
	_mouseCapture=mouseCapture;
}

inline AReVi::ArRef<AReVi::Renderer3D> AReViMouseListener::getRenderer()
{
	return _renderer;
}
inline void AReViMouseListener::setRenderer(AReVi::ArRef<AReVi::Renderer3D> renderer)
{
	if(_renderer == renderer)
		return;
	if(_renderer)
	{
		_renderer->removeMouseButtonCB(thisRef(),&AReViMouseListener::onMouseButtonInteraction);
		_renderer->removeMouseMotionCB(thisRef(),&AReViMouseListener::onMouseMotionInteraction);
	}
	_renderer=renderer;
	if(_renderer)
	{
		_renderer->addMouseButtonCB(thisRef(),&AReViMouseListener::onMouseButtonInteraction);
		_renderer->addMouseMotionCB(thisRef(),&AReViMouseListener::onMouseMotionInteraction);
	}
}
}

#endif /* AREVIMOUSE_H_ */
