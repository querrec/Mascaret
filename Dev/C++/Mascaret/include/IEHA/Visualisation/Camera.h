/*
 * Light.h
 *
 *  Created on: 24 févr. 2011
 *      Author: soler
 */

#ifndef CAMERA_H_
#define CAMERA_H_
#include "Tools/veha_plateform.h"
#include "VEHA/Entity/Color.h"
#include "VEHA/Entity/PointSpecification.h"
#include "VEHA/Entity/Vector3.h"
#include "VEHA/Entity/RotationVector.h"
#include "IEHA/Interaction/Navigation.h"
#include "VEHA/Entity/Entity.h"
#include "VEHA/Entity/Zone.h"

namespace IEHA
{
using VEHA::PointSpecification;
using VEHA::Color;

using namespace VEHA;

struct UserPod {
	double physicalScreenHeight;
	double physicalScreenWidth;
};

class Scene;
class VEHA_API Camera
{
public:
	virtual double getAspectRatio() const=0;
	virtual void setAspectRatio(double aspectRatio)=0;

	virtual double getFarDistance() const=0;
	virtual void setFarDistance(double farDistance)=0;

	virtual double getFieldOfView() const=0;
	virtual void setFieldOfView(double fieldOfView)=0;

	virtual double getNearDistance() const=0;
	virtual void setNearDistance(double nearDistance)=0;

	virtual void setHeadLight(bool headLight)=0;

	virtual Color getBackgroundColor() const=0;
	virtual void setBackgroundColor(const Color& color)=0;

	virtual void setStereo(bool stereo)=0;
	virtual bool getStereo() const=0;

	virtual double getStereoEyeSeparation() const=0;
	virtual void setStereoEyeSeparation(double stereoEyeSeparation)=0;

	virtual double getStereoViewDistance() const=0;
	virtual void setStereoViewDistance(double stereoViewDistance)=0;

	virtual void gotoViewPoint(const string& name)=0;
	virtual void gotoNextViewPoint();
	virtual void gotoPreviousViewPoint();

    virtual string getCurrentViewpoint();

	/**
	 * Returns a copy of the location of the camera
	 */
	virtual shared_ptr<PointSpecification> getGlobalLocation() const=0;
	virtual void setGlobalLocation(shared_ptr<PointSpecification>& loc,Navigation::NavigationMode mode)=0;

	virtual void translate(const Vector3& delta,Navigation::NavigationMode mode)=0;
	virtual void rotate(const RotationVector& delta,Navigation::NavigationMode mode)=0;
	virtual void rotate(const Vector3& delta,Navigation::NavigationMode mode)=0;

	virtual const shared_ptr<Scene> getCurrentScene() const=0;
	virtual shared_ptr<Scene> accessCurrentScene()=0;
	virtual void setCurrentScene(shared_ptr<Scene> scene)=0;

	virtual void addSound(string url)=0;
	virtual void playSound()=0;

	virtual shared_ptr<Entity> getEntity(int x, int y,double& distOut) const=0;

	virtual void setHeadTracking(bool useHeadTracking) {_useHeadTracking = useHeadTracking;}
	virtual bool getHeadTracking() {return _useHeadTracking;}

	virtual void setUserPosition(double x, double y, double z) {};
	virtual bool isUserPod() {return _isUserPod;}

protected:
	string _currentViewPoint;
	bool _isUserPod;
	bool _useHeadTracking;
	struct UserPod _userPod;

};
}
#endif /* CAMERA_H_ */
