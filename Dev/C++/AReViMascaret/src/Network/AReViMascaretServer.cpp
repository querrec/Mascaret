
#include "AReVi/Lib3D/simpleInteractor.h"
#include "AReVi/Lib3D/sound3D.h"
#include "AReVi/Lib3D/object3D.h"
#include "AReVi/Lib3D/boundingBox3D.h"
#include "AReVi/Lib3D/material3D.h"
#include "AReVi/Lib3D/transform3D.h"
#include "AReVi/Lib3D/urlTexture.h"

#include "AReVi/Shapes/shape3D.h"
#include "AReVi/Shapes/sphere3D.h"

#include "AReVi/Utils/memoryBlock.h"
#include "AReVi/Utils/memoryStream.h"

#include "AReVi/Lib3D/osdSprite.h"
#include "AReVi/Lib3D/osdText.h"

#include "AReVi/arSystem.h"

#include "Network/AReViMascaretServer.h"

namespace AReViMascaret
{
AR_CLASS_NOVOID_DEF(AReViMascaretServer,ArObject)

AReViMascaretServer::AReViMascaretServer(ArCW & arCW, ArRef<Scene3D> scene, bool initListeners, int port)
: ArObject(arCW),
  _depService(LocalDepService::NEW(port)),
  _server(LocalViewer3D::NEW(port - 1,_depService)),
  _initListeners(initListeners),
  _scene(scene),
  _location(Base3D::NEW()),
  _farDistance(-1.0),
  _nearDistance(-1.0),
  _linearSpeed(-1.0),
  _scale(1.0),
  _headLightColor(0.8,0.8,0.8)
{
	if (_server->fail())
	{
		cerr << _server->getErrorMessage() << endl << "  => Server not initialized" << endl;
		setErrorMessage(_server->getErrorMessage()+"\n"+"Server not initialized");
		return;
	}
	if (_depService->fail())
	{
		cerr << _depService->getErrorMessage() << endl << "  => Server not initialized" << endl;
		setErrorMessage(_depService->getErrorMessage()+"\n"+"Server not initialized");
		return;
	}


	// Set default session name
	_server->setSessionName(strBasename(ArSystem::getCommandLine()[0]));

	// On new client
	_server->setViewerSetup(thisRef(),(LocalViewer3D::ViewerSetupMethod)&AReViMascaretServer::onNewViewer3D);

	// On client deconnect
	_server->addServiceDeconnectCB(thisRef(),&AReViMascaretServer::onDeconnectViewer3D);

	// On Client complete connection
	_server->registerEvent(NSEventEndConnection::CLASS()->getName(),
			thisRef(),
			(NetworkService::EventMethod)&AReViMascaretServer::_onClientEndConnectionEvent);
}

AReViMascaretServer::~AReViMascaretServer(void)
{
	cerr << "MyServer::~MyServer => destroy server" << endl;
	if (_server.valid())
		_server.destroy(); // UGLY !

	if (_depService.valid())
		_depService.destroy(); // UGLY !
}

ArRef<Viewer3D>
AReViMascaretServer::onNewViewer3D(unsigned int serviceID)
{
	cerr << getAlias() << "::New connection => NAME=" << _server->getServiceName(serviceID) << "     ID="  << serviceID << "   Host=" << _server->getServiceHostName(serviceID) << endl;
	if (_server->getNbServices()>4)
	{
		cerr << "LIMIT NB CLIENTS TO 3 => REFUSE THIS" << endl;
		return Viewer3D::nullRef();
	}

	ArRef<Viewer3D> viewer=Viewer3D::NEW();
	viewer->selectScene(_scene);
	ArRef<SimpleInteractor> interactor=SimpleInteractor::NEW();
	interactor->setRenderer(viewer);

	// Set default perspective view:
	ArRef<BoundingBox3D> bbox=BoundingBox3D::NEW();
	_scene->computeBoundingBox(bbox,_location);
	if(bbox->isValid())
	{
		viewer->setFarDistance(10.0*bbox->getMaxSize());
		viewer->setNearDistance(0.001*viewer->getFarDistance());
		double cx,cy,cz;
		bbox->getCenter(cx,cy,cz);
		viewer->setPosition(cx,cy,cz);
	}
	viewer->yaw(M_PI);
	viewer->translate(-10.0,0.0,0.0);


	//viewer->setMapped(true);
	viewer->setHeadLightColor(_headLightColor.x,_headLightColor.y,_headLightColor.z);
	viewer->setLocation(_location);

	if (_farDistance>=0.0)
		viewer->setFarDistance(_farDistance);

	if (_nearDistance>=0.0)
		viewer->setNearDistance(_nearDistance);

	if (_linearSpeed>=0.0)
		interactor->setLinearSpeed(_linearSpeed);

	ArRef<Object3D> obj=Object3D::NEW();
	ArRef<Shape3D> sh=Shape3D::NEW();
	ArRef<Sphere3D> sph=Sphere3D::NEW();
	ArRef<Material3D> mat=Material3D::NEW();
	mat->setDiffuseColor(ArSystem::realRand(),ArSystem::realRand(),ArSystem::realRand());
	mat->setAmbientIntensity(ArSystem::realRand());
	mat->setTransparency(0.0);

	sph->setRadius(3.0*_scale);
	sph->writeMaterial(mat);
	sph->setPrecision(5);
	sh->addRootPart(sph);
	obj->setShape(sh);
	_scene->addObject(obj);

	obj->setLocation(viewer);
	_clientsInfos[serviceID]._avatar=obj;
	obj->attachTo(viewer);
	viewer->attachTo(obj);

	// Ignore all avatars and all others avatars ignore me!
	StlMap<int, ClientInfos >::iterator it=_clientsInfos.begin();
	StlMap<int, ClientInfos >::iterator it_end=_clientsInfos.end();
	while(it!=it_end)
	{
		interactor->addIgnoredObject(it->second._avatar);

		if (it->second._interactor.valid())
			it->second._interactor->addIgnoredObject(obj);

		it++;
	}
	_clientsInfos[serviceID]._interactor=interactor;

	// Add OSD

	// Add remote listener
	if (_initListeners)
	{
		ArRef<Listener3D> l=Listener3D::NEW();
		l->setLocation(viewer);
		l->attachTo(viewer);
		l->selectScene(_scene);
		_server->setListener(serviceID,l);
	}

	return viewer;
}

void
AReViMascaretServer::onDeconnectViewer3D(const NetworkService::ServiceDeconnectEvent &evt)
{
	int serviceID(evt.serviceID);
	cerr << getAlias() << "::Deconnect => ID="  << serviceID << endl;

	StlMap<int, ClientInfos >::iterator it=_clientsInfos.begin();
	StlMap<int, ClientInfos >::iterator it_end=_clientsInfos.end();
	while(it!=it_end)
	{
		if (it->second._interactor.valid())
			it->second._interactor->removeIgnoredObject(_clientsInfos[evt.serviceID]._avatar);
		it++;
	}

	_scene->removeObject(_clientsInfos[evt.serviceID]._avatar);

	_clientsInfos[evt.serviceID]._interactor=SimpleInteractor::nullRef();
	_clientsInfos.erase(_clientsInfos.find(evt.serviceID));
}

ArRef<Scene3D>
AReViMascaretServer::accessScene(void)
{
	return _scene;
}

ArRef<LocalViewer3D>
AReViMascaretServer::accessServer(void)
{
	return _server;
}

void
AReViMascaretServer::setInitialLocation(ArRef<Base3D> base)
{
	_location->accessBase()=base->getBase();
}

void
AReViMascaretServer::setPerspective(double farDistance, double nearDistance)
{
	_farDistance=farDistance;
	_nearDistance=nearDistance;
}

void
AReViMascaretServer::setLinearSpeed(double speed)
{
	_linearSpeed=speed;
}

void
AReViMascaretServer::setHeadLightColor(Util3D::Dbl3 color)
{
	_headLightColor=color;
}
void
AReViMascaretServer::setAvatarSize(double scale)
{
	if (scale>=0.0)
		_scale=scale;
}
/*
ArRef<Scene3D>
AReViMascaretServer::copyGlobalScene(void)
{
  ArRef<Scene3D> scene=Scene3D::NEW();
  SerializationDependencies deps;
  ArRef<MemoryBlock> buffer=MemoryBlock::NEW();
  ArRef<MemoryOStream> memOStream=MemoryOStream::NEW(buffer,0);
  Scene3D::accessGlobalScene()->serialize(deps,memOStream);

  ArRef<MemoryIStream> memIStream=MemoryIStream::NEW(buffer,0);
  scene->unserialize(deps,memIStream);

  return scene;
}
 */
NSEvent::EventProcess
AReViMascaretServer::_onClientEndConnectionEvent(ArConstRef<NSEvent> event)
{
	int serviceID=event->getServiceId();
	cerr << getAlias() << "::Connection completed => NAME=" << _server->getServiceName(serviceID) << "     ID="  << serviceID << "   Host=" << _server->getServiceHostName(serviceID) << endl;

	return NSEvent::EP_OK;
}
}
