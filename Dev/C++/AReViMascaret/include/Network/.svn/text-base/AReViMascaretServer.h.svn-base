//----------------------------------------------------------------------------
// @(#) ENIB/LI2 (c) - VET : Virtual Environement Training
//----------------------------------------------------------------------------
// package     : TEST
// file        : myServer.h
// author(s)   : CAZEAUX Eric
// mail        : cazeaux@enib.fr
//----------------------------------------------------------------------------
// last update : 17/03/2004
//----------------------------------------------------------------------------
#ifndef NETWORKSCENE3D_TEST_MYSERVER
#define NETWORKSCENE3D_TEST_MYSERVER 1

#include "areviMascaret_plateform.h"
#include "AReVi/Lib3D/scene3D.h"
#include "AReViNS/localViewer3D.h"

#include "nsEventEndConnection.h"

using namespace AReVi;
namespace AReVi
{
class SimpleInteractor;
class Object3D;
class OSDSprite;
class OSDText;
}

namespace AReViMascaret
{
class AREVI_MASCARET_API AReViMascaretServer : public ArObject
{
public:
	AR_CLASS(AReViMascaretServer)
	AR_CONSTRUCTOR_3(AReViMascaretServer,ArRef<Scene3D>, scene, bool, initListener, int, port)

	void
	setInitialLocation(ArRef<Base3D> base);

	void
	setPerspective(double farDistance, double nearDistance);

	void
	setLinearSpeed(double speed);

	void
	setHeadLightColor(Util3D::Dbl3 color);

	void
	setAvatarSize(double scale);

	//static ArRef<Scene3D>
	//copyGlobalScene(void);

	virtual ArRef<Viewer3D>
	onNewViewer3D(unsigned int serviceID);

	virtual void
	onDeconnectViewer3D(const NetworkService::ServiceDeconnectEvent &evt);

	ArRef<Scene3D>
	accessScene(void);

	ArRef<LocalViewer3D>
	accessServer(void);

protected:

	virtual NSEvent::EventProcess
	_onClientEndConnectionEvent(ArConstRef<NSEvent> event);

protected:
	ArRef<LocalDepService> _depService;
	ArRef<LocalViewer3D> _server;

	bool _initListeners;

	ArRef<Scene3D> _scene;
	struct ClientInfos
	{
		ArRef<OSDSprite> _sprite;
		ArRef<OSDText> _text;
		ArRef<Object3D> _avatar;
		ArRef<SimpleInteractor> _interactor;
	};

	StlMap<int, ClientInfos> _clientsInfos;

	ArRef<Base3D> _location;
	double _farDistance;
	double _nearDistance;
	double _linearSpeed;

	double _scale;

	Util3D::Dbl3 _headLightColor;
};
}

#endif
// eof
