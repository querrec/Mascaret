/*
 * AReViCamera.cpp
 *
 *  Created on: 25 févr. 2011
 *      Author: soler
 */
#include "VRPlateform/AReViCamera.h"
#include "VRPlateform/AReViWindow3D.h"
#include "VRPlateform/AReViScene.h"

#include "VEHA/Entity/RotationVector.h"
#include "VEHA/Entity/PointSpecification.h"
#include "VEHA/Entity/Entity.h"
#include "VEHA/Entity/Color.h"

#include "AReVi/Lib3D/urlSound3D.h"
#include "AReVi/Lib3D/scene3D.h"

using namespace VEHA;
using namespace IEHA;
using namespace AReVi;
using AReVi::URLSound3D;

namespace AReViMascaret
{

AReViCamera::AReViCamera(shared_ptr<Window3D> window)
{
	_areviViewer=boost::shared_dynamic_cast<AReViWindow3D>(window)->getAReViViewer();
	_location=shared_ptr<PointSpecification>(new AReViPointSpecification());
	_wingTurret=shared_ptr<PointSpecification>(new AReViPointSpecification());
	_wingTurret->setParent(_location);
	_areviViewer->attachTo(shared_dynamic_cast<AReViPointSpecification>(_wingTurret)->getPoint());
	_microphone=Listener3D::NEW();
	_microphone->setLocation(_areviViewer);
	_microphone->attachTo(_areviViewer);
	Listener3D::setCurrent(_microphone);
cerr << " ################ LISTENER " << endl;
	//shared_dynamic_cast<AReViPointSpecification>(_location)->getPoint()->attachTo(_areviViewer);
}

AReViCamera::~AReViCamera()
{

}

void AReViCamera::playSound()
{
  	_sound->play();	
}

void AReViCamera::addSound(string url)
{
	ArRef<Listener3D> m=Listener3D::NEW();
	m->setLocation(_areviViewer);
	m->attachTo(_areviViewer);
	Listener3D::setCurrent(m);

	_sound = URLSound3D::NEW(url,true);
	cout<<"IF THE SOUND FAIL : "<<_sound->fail()<<" IT'S BECAUSE : "<<_sound->getErrorMessage()<<endl;
	_sound->setGain(1000);
	_sound->setRepeat(false);
	_sound->setLocation(_areviViewer);
	_sound->attachTo(_areviViewer);
  	//_sound->play();	
}

double AReViCamera::getAspectRatio() const
{
	return _areviViewer->getAspectRatio();
}
void AReViCamera::setAspectRatio(double aspectRatio)
{
	_areviViewer->setFixedAspectRatio(aspectRatio);
}

double AReViCamera::getFarDistance() const
{
	return _areviViewer->getFarDistance();
}
void AReViCamera::setFarDistance(double farDistance)
{
	_areviViewer->setFarDistance(farDistance);
}

double AReViCamera::getFieldOfView() const
{
	return _areviViewer->getFieldOfView();
}
void AReViCamera::setFieldOfView(double fieldOfView)
{
	_areviViewer->setFieldOfView(fieldOfView);
}

double AReViCamera::getNearDistance() const
{
	return _areviViewer->getNearDistance();
}
void AReViCamera::setNearDistance(double nearDistance)
{
	_areviViewer->setNearDistance(nearDistance);
}

Color AReViCamera::getBackgroundColor() const
{
	double r,g,b;
	_areviViewer->getBackgroundColor(r,g,b);
	Color c(r,g,b);
	return c;
}
void AReViCamera::setBackgroundColor(const Color& color)
{
	_areviViewer->setBackgroundColor(color.r,color.g,color.b);
}
bool AReViCamera::getHeadLight() const
{
	return _areviViewer->getHeadLight();
}
void AReViCamera::setHeadLight(bool val)
{
	_areviViewer->setHeadLight(val);
}

void AReViCamera::setStereo(bool stereo)
{
	if(stereo)
		_areviViewer->setStereoMode(Renderer3D::STEREO_SPLIT);
	else
		_areviViewer->setStereoMode(Renderer3D::STEREO_NONE);
}
bool AReViCamera::getStereo() const
{
	return _areviViewer->getStereoMode() != Renderer3D::STEREO_SPLIT;
}

double AReViCamera::getStereoEyeSeparation() const
{
	return _areviViewer->getStereoEyeSeparation();
}
void AReViCamera::setStereoEyeSeparation(double stereoEyeSeparation)
{
	_areviViewer->setStereoEyeSeparation(stereoEyeSeparation);
}

double AReViCamera::getStereoViewDistance() const
{
	return _areviViewer->getStereoViewDistance();
}
void AReViCamera::setStereoViewDistance(double stereoViewDistance)
{
	_areviViewer->setStereoViewDistance(stereoViewDistance);
}


shared_ptr<PointSpecification> AReViCamera::getGlobalLocation() const
{
	shared_ptr<AReViPointSpecification> point(new AReViPointSpecification());
	point->setGlobalPosition(_location->getGlobalPosition());
	Vector3 ori=_wingTurret->getGlobalOrientation();
	ori.x+=_location->getGlobalOrientation().x;
	ori.y+=_location->getGlobalOrientation().y;
	ori.z=_location->getGlobalOrientation().z;
	point->setGlobalOrientation(ori);
	return point;
}
void AReViCamera::setGlobalLocation(shared_ptr<PointSpecification>& loc,Navigation::NavigationMode mode)
{
	_location->setGlobalPosition(loc->getGlobalPosition());
	Vector3 ori=loc->getGlobalOrientation();
	if(mode == Navigation::HUMAN)
	{
		_location->setGlobalOrientation(Vector3(0,0,ori.z));
		_wingTurret->setLocalOrientation(Vector3(ori.x,ori.y,0));
	}
	else
	{
		_wingTurret->setLocalOrientation(Vector3(0,0,0));
		_location->setGlobalOrientation(ori);
	}
}

void AReViCamera::translate(const Vector3& delta,Navigation::NavigationMode mode)
{
	if(mode == Navigation::HUMAN)
	{
		//TODO: check orientation of wingTurret and change if needed
	}
	Vector3 ori=_location->getGlobalOrientation();
	_location->translate(delta);
}
void AReViCamera::rotate(const RotationVector& delta,Navigation::NavigationMode mode)
{
	//if(mode == Navigation::HUMAN)
	{
		_location->rotate(delta);
		Vector3 ori=_location->getLocalOrientation();
		_location->setLocalOrientation(Vector3(0,0,ori.z));
		_wingTurret->setLocalOrientation(Vector3(ori.x,ori.y,0));
	}
}
void AReViCamera::rotate(const Vector3& delta,Navigation::NavigationMode mode)
{
	if(mode == Navigation::HUMAN)
	{
		_location->rotate(Vector3(0,0,delta.z));
		_wingTurret->rotate(Vector3(delta.x,delta.y,0));
	}
	else
	{
		_location->rotate(Vector3(delta.x,delta.y,delta.z));
	}
}

void AReViCamera::gotoViewPoint(const string& name)
{
	cerr << "AReViCamera::gotoViewPoint("<<name<<")"<<endl;
	shared_ptr<Zone> zone=accessCurrentScene()->getZone();
	if(zone)
	{
		cerr << zone->getName() << endl;
		shared_ptr<PointSpecification> viewPoint=zone->getViewPoint(name);
		if(viewPoint)
		{
		        cerr << " OK ..." << endl;
			setGlobalLocation(viewPoint,Navigation::HUMAN);
			//shared_dynamic_cast<AReViPointSpecification>(_location)->getPoint()->setLocation(shared_dynamic_cast<AReViPointSpecification>(viewPoint)->getPoint());
			_currentViewPoint = name;
		}
		else cerr << " Pas de viewpoint : " << name << endl;
	}
	else cerr << " Pb de zone" << endl;
}

const shared_ptr<Scene> AReViCamera::getCurrentScene() const
{
	return _scene;
}
shared_ptr<Scene> AReViCamera::accessCurrentScene()
{
	return _scene;
}
void AReViCamera::setCurrentScene(shared_ptr<Scene> scene)
{
	_scene=scene;
	shared_ptr<AReViScene> arScene=shared_dynamic_cast<AReViScene>(scene);
	_areviViewer->selectScene(arScene->getScene());
	_areviViewer->setBackScene(arScene->getSceneBack());
	_areviViewer->setFrontScene(arScene->getSceneFront());
	_microphone->selectScene(arScene->getScene());
}
shared_ptr<Entity> AReViCamera::getEntity(int x, int y, double& distOut) const
{
	if(_areviViewer)
	{
		ArRef<Object3D> obj;
		ArRef<ShapePart3D> part;
		unsigned int triIndex;
		_areviViewer->accessScene()->firstPicking(_areviViewer,x,y,0,true,AReViEntityObject::CLASS(),obj,part,triIndex,distOut);
		if(obj != Object3D::nullRef())
		{
			return ar_down_cast<AReViEntityObject>(obj)->getEntity();
		}
	}
	return shared_ptr<Entity>();
}


shared_ptr<VEHA::PointSpecification> AReViCamera::getWingTurret() const
{
	return _wingTurret;
}

}
