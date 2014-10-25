/*
 * AReViMouse.h
 *
 *  Created on: 3 mars 2011
 *      Author: soler
 */

#ifndef OGREMOUSE_H_
#define OGREMOUSE_H_

#include "ogreMascaret_plateform.h"
#include <Ogre.h>
#include <OIS.h>
#include "IEHA/Interaction/PaddlePeripheric.h"
#include "VRPlateform/cameraMan.h"

namespace OgreMascaret
{
class OgreMouse : public IEHA::PaddlePeripheric,public OIS::MouseListener
{
public:
	OgreMouse();
	virtual ~OgreMouse();

	virtual bool mouseMoved( const OIS::MouseEvent &arg );
	virtual bool mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id );
	virtual bool mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id );

	virtual void enableCameraMan(bool enable, CameraMan* cameraMan=NULL);

protected:

	CameraMan* _cameraMan;
};

}

#endif /* AREVIMOUSE_H_ */
