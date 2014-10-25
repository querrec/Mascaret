/*
 * AReViKeyboard.h
 *
 *  Created on: 3 mars 2011
 *      Author: soler
 */

#ifndef OGREKEYBOARD_H_
#define OGREKEYBOARD_H_
#include "ogreMascaret_plateform.h"
#include <Ogre.h>
#include <OIS.h>

#include "IEHA/Interaction/PaddlePeripheric.h"

namespace OgreMascaret
{
class OGRE_MASCARET_API OgreKeyboard : public IEHA::PaddlePeripheric, public OIS::KeyListener
{
public:
	OgreKeyboard();
	virtual ~OgreKeyboard();

	virtual bool keyPressed( const OIS::KeyEvent &arg );
	virtual bool keyReleased( const OIS::KeyEvent &arg );
protected:
};
}

#endif /* AREVIKEYBOARD_H_ */
