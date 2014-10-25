#include "VRPlateform/cameraMan.h"

#include "VEHA/Entity/Vector3.h"
#include "IEHA/Interaction/Navigation.h"
#include "VRPlateform/OgreCamera.h"

namespace OgreMascaret
{

//CameraMan::CameraMan(Ogre::Camera* cam, OIS::Keyboard* keyboard) :
CameraMan::CameraMan(shared_ptr<OgreCamera> cam, OIS::Keyboard* keyboard) :
  mCamera(0),
  mTarget(0),
  mAnchor(0),
  mInteracting(false),
  mMouseControlled(true),
  mOrbiting(false),
  mZooming(false),
  mTopSpeed(10.0),
  mVelocity(Ogre::Vector3::ZERO),
  mGoingForward(false),
  mGoingBack(false),
  mGoingLeft(false),
  mGoingRight(false),
  mGoingUp(false),
  mGoingDown(false),
  mFastMove(false),
  mFastFactor(10.0),
  _keyboard(keyboard)
{
  setCamera(cam);
}

void CameraMan::setTarget(Ogre::SceneNode* target)
{
  mTarget = target;
  if (mTarget)
  {
    mCamera->setAutoTracking(true, mTarget);
  }
  else
  {
    mCamera->setAutoTracking(false);
  }
}

Ogre::SceneNode* CameraMan::getAnchor()
{
  return mAnchor ? mAnchor->getParentSceneNode() : NULL;
}

void CameraMan::setAnchor(Ogre::SceneNode* anchor, Ogre::Vector3 offset,
                          bool hideAnchor)
{
  // Clear previous anchor
  if (mAnchor)
  {
    // Show all objects in anchor node
    Ogre::SceneNode::ObjectIterator it =
      mAnchor->getParentSceneNode()->getAttachedObjectIterator();
    while (it.hasMoreElements())
    {
      it.getNext()->setVisible(true);
    }

    Ogre::Vector3 position = mCamera->getRealPosition();
    Ogre::Quaternion orientation = mCamera->getRealOrientation();
    mAnchor->detachObject(mCamera);
    mAnchor->getParentSceneNode()->removeAndDestroyChild("CameraManAnchor");
    mAnchor = NULL;
    mCamera->setPosition(position);
    mCamera->setOrientation(orientation);
  }

  if (anchor)
  {
    // Hide all objects in anchor node
    Ogre::SceneNode::ObjectIterator it =
      anchor->getAttachedObjectIterator();
    if (hideAnchor)
    {
      while (it.hasMoreElements())
      {
        it.getNext()->setVisible(false);
      }
    }

    // Create anchor
    mAnchor = anchor->createChildSceneNode("CameraManAnchor");
    //    if(mCamera->getParentSceneNode())
    //      mCamera->getParentSceneNode()->detachObject(mCamera);
    mAnchor->attachObject(mCamera);
    mAnchor->setInheritOrientation(false);
    mCamera->setPosition(offset);
  }
}

void CameraMan::setYawPitchDist(Ogre::Radian yaw, Ogre::Radian pitch,
                                Ogre::Real dist)
{

  CameraStyle mStyle = getCameraStyle();
  if (mStyle == CS_ORBIT)
  {
    mCamera->setPosition(mTarget->_getDerivedPosition());
    mCamera->setOrientation(mTarget->_getDerivedOrientation());
    mCamera->yaw(yaw);
    mCamera->pitch(-pitch);
    mCamera->moveRelative(Ogre::Vector3(0, 0, dist));
  }
}

void CameraMan::setInteracting(bool s)
{
  mInteracting = s;
  if (mInteracting)
    manualStop();
}

CameraStyle CameraMan::getCameraStyle()
{
  if (!mInteracting)
  {
    if (!mTarget && !mAnchor)
    {
      return CS_FREELOOK;
    }
    else if (mTarget && !mAnchor)
    {
      return CS_ORBIT;
    }
    else if (!mTarget && mAnchor)
    {
      return CS_FIRSTPERSON;
    }
    else //avoid warning
    {
      return CS_LOCK;
    }
  }
  else
  {
    if (mMouseControlled)
    {
      return CS_MOUSE;
    }
    else
    {
      return CS_LOCK;
    }
  }
}

void CameraMan::manualStop()
{
  mGoingForward = false;
  mGoingBack = false;
  mGoingLeft = false;
  mGoingRight = false;
  mGoingUp = false;
  mGoingDown = false;
  mVelocity = Ogre::Vector3::ZERO;
}

bool CameraMan::frameRenderingQueued(const Ogre::FrameEvent& evt)
{
  CameraStyle mStyle = getCameraStyle();
  if (mStyle == CS_FREELOOK)
  {
    // build our acceleration vector based on keyboard input composite
    Ogre::Vector3 accel = Ogre::Vector3::ZERO;
    if (mGoingForward)
      accel += mCamera->getDirection();
    if (mGoingBack)
      accel -= mCamera->getDirection();
    if (mGoingRight)
      accel += mCamera->getRight();
    if (mGoingLeft)
      accel -= mCamera->getRight();
    if (mGoingUp)
      accel += mCamera->getUp();
    if (mGoingDown)
      accel -= mCamera->getUp();

    // if accelerating, try to reach top speed in a certain time
    Ogre::Real topSpeed = mFastMove ? mTopSpeed * mFastFactor : mTopSpeed;
    if (accel.squaredLength() != 0)
    {
      accel.normalise();
      mVelocity += accel * topSpeed * evt.timeSinceLastFrame * 10;
    }
    // if not accelerating, try to stop in a certain time
    else
    {
      Ogre::Vector3 dir = mVelocity;
      mVelocity -= mVelocity * evt.timeSinceLastFrame * 10;
      if (mVelocity.dotProduct(dir) < 0.)
        mVelocity = Ogre::Vector3::ZERO;

    }

    Ogre::Real tooSmall = std::numeric_limits<Ogre::Real>::epsilon();

    // keep camera velocity below top speed and above epsilon
    if (mVelocity.squaredLength() > topSpeed * topSpeed)
    {
      mVelocity.normalise();
      mVelocity *= topSpeed;
    }
    else if (mVelocity.squaredLength() < tooSmall * tooSmall)
      mVelocity = Ogre::Vector3::ZERO;

    if (mVelocity != Ogre::Vector3::ZERO)
      mCamera->move(mVelocity * evt.timeSinceLastFrame);
  }
  return true;
}

void CameraMan::injectKeyDown(const OIS::KeyEvent& evt)
{
  if (mInteracting)
    return;

  CameraStyle mStyle = getCameraStyle();
  if (mStyle == CS_FREELOOK)
  {
    if (evt.key == OIS::KC_Z || evt.key == OIS::KC_UP)
      mGoingForward = true;
    else if (evt.key == OIS::KC_S || evt.key == OIS::KC_DOWN)
      mGoingBack = true;
    else if (evt.key == OIS::KC_Q || evt.key == OIS::KC_LEFT)
      mGoingLeft = true;
    else if (evt.key == OIS::KC_D || evt.key == OIS::KC_RIGHT)
      mGoingRight = true;
    else if (evt.key == OIS::KC_A || evt.key == OIS::KC_PGUP)
      mGoingUp = true;
    else if (evt.key == OIS::KC_E || evt.key == OIS::KC_PGDOWN)
      mGoingDown = true;
    else if (evt.key == OIS::KC_LSHIFT)
      mFastMove = true;
  }
}

void CameraMan::injectKeyUp(const OIS::KeyEvent& evt)
{
  if (mInteracting)
    return;

  CameraStyle mStyle = getCameraStyle();
  if (mStyle == CS_FREELOOK)
  {
    if (evt.key == OIS::KC_Z || evt.key == OIS::KC_UP)
      mGoingForward = false;
    else if (evt.key == OIS::KC_S || evt.key == OIS::KC_DOWN)
      mGoingBack = false;
    else if (evt.key == OIS::KC_Q || evt.key == OIS::KC_LEFT)
      mGoingLeft = false;
    else if (evt.key == OIS::KC_D || evt.key == OIS::KC_RIGHT)
      mGoingRight = false;
    else if (evt.key == OIS::KC_A || evt.key == OIS::KC_PGUP)
      mGoingUp = false;
    else if (evt.key == OIS::KC_E || evt.key == OIS::KC_PGDOWN)
      mGoingDown = false;
    else if (evt.key == OIS::KC_LSHIFT)
      mFastMove = false;
  }
}

#if OGRE_PLATFORM == OGRE_PLATFORM_IPHONE
void CameraMan::injectMouseMove(const OIS::MultiTouchEvent& evt)
#else
void CameraMan::injectMouseMove(const OIS::MouseEvent& evt)
#endif
{
  CameraStyle mStyle = getCameraStyle();
  if (mStyle == CS_MOUSE)
  {
    if(evt.state.buttonDown(OIS::MB_Middle))
    {
      if(_keyboard && _keyboard->isModifierDown(OIS::Keyboard::Shift))
      {
        //mCamera->moveRelative(Ogre::Vector3(evt.state.X.rel * 0.15f, -evt.state.Y.rel * 0.15f, 0.0f));
        _camera->translate(VEHA::Vector3(evt.state.X.rel * 0.15f, -evt.state.Y.rel * 0.15f, 0.0f), IEHA::Navigation::FREE);
      }
      else if(_keyboard && _keyboard->isModifierDown(OIS::Keyboard::Ctrl))
      {
        //mCamera->moveRelative(Ogre::Vector3(0.0f, 0.0f, -evt.state.Y.rel * 0.15f));
        _camera->translate(VEHA::Vector3(0.0f, 0.0f, -evt.state.Y.rel * 0.15f), IEHA::Navigation::FREE);
      }
      else
      {
        //mCamera->yaw(Ogre::Degree(-evt.state.X.rel * 0.15f));
        //mCamera->pitch(Ogre::Degree(-evt.state.Y.rel * 0.15f));
        Vector3 ori;
        ori.y = Ogre::Degree(-evt.state.X.rel * 0.15f).valueRadians();
        ori.x = Ogre::Degree(-evt.state.Y.rel * 0.15f).valueRadians();
        _camera->rotate(ori, IEHA::Navigation::FREE);
      }
    }
  }
  if (mInteracting)
    return;

  else if (mStyle == CS_ORBIT)
  {
    Ogre::Real dist = (mCamera->getPosition()
                       - mTarget->_getDerivedPosition()).length();

    if (mOrbiting) // yaw around the target, and pitch locally
    {
      mCamera->setPosition(mTarget->_getDerivedPosition());

      mCamera->yaw(Ogre::Degree(-evt.state.X.rel * 0.25f));
      mCamera->pitch(Ogre::Degree(-evt.state.Y.rel * 0.25f));

      mCamera->moveRelative(Ogre::Vector3(0, 0, dist));

      // don't let the camera go over the top or around the bottom of the target
    }
    else if (mZooming)   // move the camera toward or away from the target
    {
      // the further the camera is, the faster it moves
      mCamera->moveRelative(
        Ogre::Vector3(0, 0, evt.state.Y.rel * 0.004f * dist));
    }
    else if (evt.state.Z.rel != 0)   // move the camera toward or away from the target
    {
      // the further the camera is, the faster it moves
      mCamera->moveRelative(
        Ogre::Vector3(0, 0, -evt.state.Z.rel * 0.0008f * dist));
    }
  }
  else if (mStyle == CS_FREELOOK || mStyle == CS_FIRSTPERSON)
  {
    mCamera->yaw(Ogre::Degree(-evt.state.X.rel * 0.15f));
    mCamera->pitch(Ogre::Degree(-evt.state.Y.rel * 0.15f));
  }
}

#if OGRE_PLATFORM == OGRE_PLATFORM_IPHONE
void CameraMan::injectMouseDown(const OIS::MultiTouchEvent& evt)
{
  if (mInteracting)
    return;

  CameraStyle mStyle = getCameraStyle();
  if (mStyle == CS_ORBIT)
  {
    mOrbiting = true;
  }
}
#else
void CameraMan::injectMouseDown(const OIS::MouseEvent& evt,
                                OIS::MouseButtonID id)
{
  if (mInteracting)
    return;

  CameraStyle mStyle = getCameraStyle();
  if (mStyle == CS_ORBIT)
  {
    if (id == OIS::MB_Left)
      mOrbiting = true;
    else if (id == OIS::MB_Right)
      mZooming = true;
  }
}
#endif

#if OGRE_PLATFORM == OGRE_PLATFORM_IPHONE
void CameraMan::injectMouseUp(const OIS::MultiTouchEvent& evt)
{
  if (mInteracting)
    return;

  CameraStyle mStyle = getCameraStyle();
  if (mStyle == CS_ORBIT)
  {
    mOrbiting = false;
  }
}
#else
void CameraMan::injectMouseUp(const OIS::MouseEvent& evt,
                              OIS::MouseButtonID id)
{
  if (mInteracting)
    return;

  CameraStyle mStyle = getCameraStyle();
  if (mStyle == CS_ORBIT)
  {
    if (id == OIS::MB_Left)
      mOrbiting = false;
    else if (id == OIS::MB_Right)
      mZooming = false;
  }
}
#endif

void CameraMan::setCamera(shared_ptr<OgreCamera> cam)
{
    _camera = cam;
    mCamera = cam->accessCamera();
}
}
