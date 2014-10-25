/*
 * OgreCursor.h
 *
 *  Created on: 4 mars 2011
 *	  Author: soler
 */

#ifndef OGRECURSOR_H_
#define OGRECURSOR_H_

#include "ogreMascaret_plateform.h"

#include "IEHA/Interaction/Cursor.h"

#include <OGRE/OgreTexture.h>
#include <OGRE/OgreMaterial.h>

namespace OIS
{
class Mouse;
}

namespace OgreMascaret
{
using VEHA::Entity;

class OGRE_MASCARET_API OgreCursor : public IEHA::Cursor
{
public:
	OgreCursor(OIS::Mouse* mouse);
	virtual ~OgreCursor();

	//virtual shared_ptr<Entity> getEntity();
	virtual void getPosition(int& x,int& y) const;
	//virtual vector<shared_ptr<Entity> > getEntities();

	void setImage(const std::string& filename);

	void setWindowDimensions(unsigned int width, unsigned int height);

	void setVisible(bool visible);

	void updatePosition(int x, int y);
	void getWheelPos(int & wheel);

	void update();

	Ogre::Real clamp(Ogre::Real a, Ogre::Real min, Ogre::Real max);

	int getId();

	Ogre::Overlay* getOverlay();

	void getOffset(Ogre::Real & x, Ogre::Real & y);
	void setOffset(Ogre::Real x, Ogre::Real y);

	bool leftButton;
	bool rightButton;
	bool wheelButton;
	int wheelUp;
	int wheelDown;
	
	int wheelPos;
	int deltaWheel;
	int wheelPosStep;

protected:
	OIS::Mouse* _mouse;

private:
	int _id;
	static int nbCursor;
	Ogre::Overlay* _guiOverlay;
	Ogre::OverlayContainer* _cursorContainer;
	Ogre::TexturePtr _texture;
	Ogre::MaterialPtr _material;
	
	Ogre::Real _windowWidth;
	Ogre::Real _windowHeight;
	Ogre::Real _xOffset;
	Ogre::Real _yOffset;
};

inline int OgreCursor::getId()
{
	return _id;
}
inline Ogre::Overlay* OgreCursor::getOverlay()
{
	return _guiOverlay;
}
inline void OgreCursor::setOffset(Ogre::Real x, Ogre::Real y)
{
	_xOffset = x;
	_yOffset = y;
}
inline void OgreCursor::getOffset(Ogre::Real& x, Ogre::Real& y)
{
	x = _xOffset;
	y = _yOffset;
}

}
#endif /* AREVICURSOR_H_ */
