/*
 * Camera.h
 *
 *  Created on: 24 févr. 2011
 *      Author: soler
 */

#ifndef AREVICAMERA_H_
#define AREVICAMERA_H_

#include "ogreMascaret_plateform.h"
#include "IEHA/Visualisation/Camera.h"
#include "VRPlateform/OgreWindow3D.h"
#include "VRPlateform/OgrePointSpecification.h"
#include <Ogre.h>
#include "Tools/Sound3D.h"

namespace OgreMascaret
{
using IEHA::Scene;
using IEHA::Camera;
using IEHA::Window3D;
using Ogre::FrameEvent;
class OGRE_MASCARET_API OgreCamera : public Camera, public Ogre::FrameListener
{
public:
	OgreCamera(boost::shared_ptr<Window3D> window);
	virtual ~OgreCamera();

	virtual double getAspectRatio() const;
	virtual void setAspectRatio(double aspectRatio);

	virtual double getFarDistance() const;
	virtual void setFarDistance(double farDistance);

	virtual double getFieldOfView() const;
	virtual void setFieldOfView(double fieldOfView);

	virtual double getNearDistance() const;
	virtual void setNearDistance(double nearDistance);

	virtual void setHeadLight(bool headLight) {}

	virtual VEHA::Color getBackgroundColor() const;
	virtual void setBackgroundColor(const VEHA::Color& color);

	virtual void setStereo(bool stereo);
	virtual bool getStereo() const;

	virtual double getStereoEyeSeparation() const;
	virtual void setStereoEyeSeparation(double stereoEyeSeparation);

	virtual double getStereoViewDistance() const;
	virtual void setStereoViewDistance(double stereoViewDistance);

	virtual void gotoViewPoint(const string& name);
	/**
	 * Returns a copy of the location of the camera
	 */
	virtual shared_ptr<PointSpecification> getGlobalLocation() const;
	virtual void setGlobalLocation(shared_ptr<PointSpecification>& loc,IEHA::Navigation::NavigationMode mode);

	virtual void translate(const Vector3& delta,IEHA::Navigation::NavigationMode mode);
	virtual void rotate(const RotationVector& delta,IEHA::Navigation::NavigationMode mode);
	virtual void rotate(const Vector3& delta,IEHA::Navigation::NavigationMode mode);

	virtual const shared_ptr<Scene> getCurrentScene() const;
	virtual shared_ptr<Scene> accessCurrentScene();
	virtual void setCurrentScene(shared_ptr<Scene> scene);

	// Override frameRenderingQueued event to process that (don't care about frameEnded)
	virtual bool frameRenderingQueued(const FrameEvent& evt);
	virtual bool frameStarted(const FrameEvent & evt);
	virtual bool frameEnded(const FrameEvent& evt);

	// Some function to get the Entity at screen on coord (x, y), return the distance to the object
	//FIXME: make it work for all type of ogre object ! It can failed dependant of how Ogre object are created in the simulation
	//WARNING : This method return the first entity (change the last param if you want to include the invisible entities or not).
	virtual shared_ptr<VEHA::Entity> getEntity(int x, int y, double& distOut, bool excludeHidden = true) const;
	//Same but you can pass a list of entities to pick (if blackList = false) or to ignore (if blackList = true)
	virtual shared_ptr<VEHA::Entity> getEntity(int x, int y, double& distOut,
			vector<Ogre::String > listEnt, bool blackList, bool excludeHidden = true) const;
	//TODO: Made this available in all IEHA::Camera, remove dependance on Ogre::String for object list

	virtual shared_ptr<VEHA::Entity> getEntity(int x, int y, double& distOut) const;

	virtual void addSound(string url){}
	virtual void playSound(){}

	virtual Vector3 getPoint(int x, int y) const;

	virtual void _getEntityAndPoint(int x, int y,
			std::vector<Ogre::String > listEnt,  bool blackList, bool excludeHidden,
			shared_ptr<VEHA::Entity> & entity, VEHA::Vector3 & pos, double & distOut
			) const;

	void getMeshInformation(Ogre::Entity *entity,
									size_t &vertex_count,
									Ogre::Vector3* &vertices,
									size_t &index_count,
									unsigned long* &indices,
									const Ogre::Vector3 &position,
									const Ogre::Quaternion &orient,
									const Ogre::Vector3 &scale) const;

	inline Ogre::Camera* accessCamera() { return _camera; }
	inline Ogre::Camera* accessCameraRight() { return _cameraRight; }

	inline virtual shared_ptr<PointSpecification> getCameraLocation() { return _location; }

	void _updateFrustum();
	void _createStereoCamera();
	void _deleteStereoCamera();

	void _manageMicrophone();

protected:
	double _stereoEyeSeparation;
	double _stereoViewDistance;

	shared_ptr<OgrePointSpecification> _userPosition;
	shared_ptr<OgrePointSpecification> _userLeftEye;
	shared_ptr<OgrePointSpecification> _userRightEye;

	shared_ptr<OgrePointSpecification> _location;
	shared_ptr<OgrePointSpecification> _wingTurret;

	shared_ptr<OgreWindow3D> _window;
	shared_ptr<OgreWindow3D> _windowRight;

	shared_ptr<Sound::Listener3D> _microphone;

	shared_ptr<Scene> _scene;

	Ogre::Camera * _camera;
	Ogre::Camera * _cameraRight;

	Ogre::RaySceneQuery* mRaySceneQuery;

	Ogre::RenderWindow * _stereoWindow;
};
}
#endif /* AREVICAMERA_H_ */
