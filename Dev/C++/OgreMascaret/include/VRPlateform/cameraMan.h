#ifndef CAMERAMAN_H
#define CAMERAMAN_H

#include "ogreMascaret_plateform.h"
#include <Ogre.h>
#include <OIS.h>
#include <boost/shared_ptr.hpp>

namespace OgreMascaret
{
    class OgreCamera;

enum CameraStyle // enumerator values for different styles of camera movement
{
  /** Simple interactor with mouse middle bouton (+ctrl), 
   * active when isInteracting.*/
  CS_MOUSE,
  /// keyboard controlled fly mode
  CS_FREELOOK,
  /// examining an object
  CS_ORBIT,
  /// Attached to a character
  CS_FIRSTPERSON,
  /// Fixed Camera
  CS_LOCK
};

class OGRE_MASCARET_API CameraMan
{
public:
  //CameraMan(Ogre::Camera* cam, OIS::Keyboard* keyboard);
  CameraMan(shared_ptr<OgreCamera> cam, OIS::Keyboard* keyboard);
  ~CameraMan()
  {
  }

//  inline void setCamera(Ogre::Camera* cam);
  void setCamera(shared_ptr<OgreCamera> cam);
//  inline Ogre::Camera* getCamera(); // FIXME: transform in 'acessCamera' ?

  /// Return camera style based on target / anchor parameters
  CameraStyle getCameraStyle();

  /// Sets the anchor where camera will be fixed.
  Ogre::SceneNode* getAnchor();
  void setAnchor(Ogre::SceneNode* anchor, Ogre::Vector3 offset =
                   Ogre::Vector3(0.0, 0.0, 0.0), bool hideAnchor = false);

  /// Sets the target we will revolve around. Only applies for orbit style.
  void setTarget(Ogre::SceneNode* target);

  /// Sets the spatial offset from the target. Only applies if target is set.
  void setYawPitchDist(Ogre::Radian yaw, Ogre::Radian pitch, Ogre::Real dist);
  
  inline void setMouseControlled(bool mouseControlled);

  /// Sets the camera's top speed. Only applies for free-look style.
  inline void setTopSpeed(Ogre::Real topSpeed);
  inline Ogre::Real getTopSpeed() const;

  /// when true the cameraMan is disable (intecting with UI is possible).
  inline bool isInteracting();
  void setInteracting(bool enabled);

  bool frameRenderingQueued(const Ogre::FrameEvent& evt);

  /// Processes key presses for free-look style movement.
  void injectKeyDown(const OIS::KeyEvent& evt);

  /// Processes key releases for free-look style movement.
  void injectKeyUp(const OIS::KeyEvent& evt);

  /// Processes mouse movement differently for each style.
#if OGRE_PLATFORM == OGRE_PLATFORM_IPHONE
  void injectMouseMove(const OIS::MultiTouchEvent& evt);
#else
  void injectMouseMove(const OIS::MouseEvent& evt);
#endif

  /** Processes mouse presses. Only applies for orbit style.
   * Left button is for orbiting, and right button is for zooming.*/
#if OGRE_PLATFORM == OGRE_PLATFORM_IPHONE
  void injectMouseDown(const OIS::MultiTouchEvent& evt);
#else
  void injectMouseDown(const OIS::MouseEvent& evt, OIS::MouseButtonID id);
#endif

  /** Processes mouse releases. Only applies for orbit style.
   * Left button is for orbiting, and right button is for zooming.*/
#if OGRE_PLATFORM == OGRE_PLATFORM_IPHONE
  void injectMouseUp(const OIS::MultiTouchEvent& evt);
#else
  void injectMouseUp(const OIS::MouseEvent& evt, OIS::MouseButtonID id);
#endif

  /// Speed multiplactor when pressing LSHIFT.
  inline void setFastFactor(Ogre::Real fastFactor);
  inline Ogre::Real getFastFactor(void) const;

protected:

  void manualStop();

  Ogre::Camera* mCamera;
  shared_ptr<OgreCamera> _camera;

  Ogre::SceneNode* mTarget;
  Ogre::SceneNode* mAnchor;

  bool mInteracting;
  bool mMouseControlled;

  bool mOrbiting;
  bool mZooming;
  Ogre::Real mTopSpeed;
  Ogre::Vector3 mVelocity;
  bool mGoingForward;
  bool mGoingBack;
  bool mGoingLeft;
  bool mGoingRight;
  bool mGoingUp;
  bool mGoingDown;
  bool mFastMove;
  Ogre::Real mFastFactor;

  OIS::Keyboard* _keyboard;
};

#if 0
void CameraMan::setCamera(Ogre::Camera* cam)
{
  mCamera = cam;
}

Ogre::Camera* CameraMan::getCamera()
{
  return mCamera;
}
#endif

void CameraMan::setTopSpeed(Ogre::Real topSpeed)
{
  mTopSpeed = topSpeed;
}

Ogre::Real CameraMan::getTopSpeed() const
{
  return mTopSpeed;
}

void CameraMan::setFastFactor(Ogre::Real fastFactor)
{
  mFastFactor = fastFactor;
}

Ogre::Real CameraMan::getFastFactor(void) const
{
  return mFastFactor;
}

bool CameraMan::isInteracting()
{
  return mInteracting;
}

void CameraMan::setMouseControlled(bool mouseControlled)
{
  mMouseControlled = mouseControlled;
}

}
#endif // CAMERAMAN_H
