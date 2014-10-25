/*
 * AReViCursor.h
 *
 *  Created on: 4 mars 2011
 *      Author: soler
 */

#ifndef AREVICURSOR_H_
#define AREVICURSOR_H_
#include "AReVi/Lib3D/renderer3D.h"

#include "areviMascaret_plateform.h"
#include "IEHA/Interaction/Cursor.h"

namespace AReViMascaret
{
using VEHA::Entity;
class AREVI_MASCARET_API AReViCursor : public IEHA::Cursor
{
public:
	AReViCursor();
	virtual ~AReViCursor();

	//virtual shared_ptr<Entity> getEntity();
	virtual void getPosition(int& x,int& y) const;
	//virtual vector<shared_ptr<Entity> > getEntities();

	inline AReVi::ArRef<AReVi::Renderer3D> getRenderer();
	inline void setRenderer(AReVi::ArRef<AReVi::Renderer3D> renderer);
protected:
	AReVi::ArRef<AReVi::Renderer3D> _renderer;
};

AReVi::ArRef<AReVi::Renderer3D> AReViCursor::getRenderer()
{
	return _renderer;
}
void AReViCursor::setRenderer(AReVi::ArRef<AReVi::Renderer3D> renderer)
{
	_renderer=renderer;
}
}


#endif /* AREVICURSOR_H_ */
