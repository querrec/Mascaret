/*
 * Camera.h
 *
 *  Created on: 24 févr. 2011
 *      Author: soler
 */

#ifndef AREVICAMERA_H_
#define AREVICAMERA_H_
#include "AReVi/Lib3D/viewer3D.h"
#include "AReVi/Lib3D/sound3D.h"
//#include "AReVi/Lib3D/urlSound3D.h"

#include "areviMascaret_plateform.h"
#include "IEHA/Visualisation/Camera.h"
#include "IEHA/Visualisation/Window3D.h"


namespace AReVi
{
	class URLSound3D;
}

namespace VEHA
{
  class PointSpecification;
  class RotationVector;
  class Entity;
  class Color;
  class Vector3;
}

namespace AReViMascaret
{
using IEHA::Scene;
using IEHA::Camera;
using IEHA::Window3D;
class AREVI_MASCARET_API AReViCamera : public Camera
{
public:
	AReViCamera(shared_ptr<Window3D> window);
	virtual ~AReViCamera();

	virtual double getAspectRatio() const;
	virtual void setAspectRatio(double aspectRatio);

	virtual double getFarDistance() const;
	virtual void setFarDistance(double farDistance);

	virtual double getFieldOfView() const;
	virtual void setFieldOfView(double fieldOfView);

	virtual double getNearDistance() const;
	virtual void setNearDistance(double nearDistance);

	virtual VEHA::Color getBackgroundColor() const;
	virtual void setBackgroundColor(const VEHA::Color& color);

	virtual bool getHeadLight() const;
	virtual void setHeadLight(bool);

	virtual void setStereo(bool stereo);
	virtual bool getStereo() const;

	virtual double getStereoEyeSeparation() const;
	virtual void setStereoEyeSeparation(double stereoEyeSeparation);

	virtual double getStereoViewDistance() const;
	virtual void setStereoViewDistance(double stereoViewDistance);
	/**
	 * Returns a copy of the location of the camera
	 */
	virtual shared_ptr<VEHA::PointSpecification> getGlobalLocation() const;
	virtual void setGlobalLocation(shared_ptr<VEHA::PointSpecification>& loc,IEHA::Navigation::NavigationMode mode);

	virtual void translate(const VEHA::Vector3& delta,IEHA::Navigation::NavigationMode mode);
	virtual void rotate(const VEHA::RotationVector& delta,IEHA::Navigation::NavigationMode mode);
	virtual void rotate(const VEHA::Vector3& delta,IEHA::Navigation::NavigationMode mode);

	virtual void gotoViewPoint(const string& name);

	virtual const shared_ptr<Scene> getCurrentScene() const;
	virtual shared_ptr<Scene> accessCurrentScene();
	virtual void setCurrentScene(shared_ptr<Scene> scene);

	virtual shared_ptr<VEHA::Entity> getEntity(int x, int y, double& distOut) const;

	virtual shared_ptr<VEHA::PointSpecification> getWingTurret() const;

	virtual void addSound(string url);
	virtual void playSound();

protected:
	shared_ptr<VEHA::PointSpecification> _location;
	shared_ptr<VEHA::PointSpecification> _wingTurret;
	AReVi::ArRef<AReVi::Viewer3D> _areviViewer;
	AReVi::ArRef<AReVi::Listener3D> _microphone;
	AReVi::ArRef<AReVi::URLSound3D> _sound;
	
	shared_ptr<Scene> _scene;
};
}
#endif /* AREVICAMERA_H_ */
