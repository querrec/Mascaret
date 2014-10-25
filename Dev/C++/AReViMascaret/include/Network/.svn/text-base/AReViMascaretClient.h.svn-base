#ifndef MY_CLIENT_H
#define MY_CLIENT

#include "areviMascaret_plateform.h"
#include "AReVi/arSystem.h"
#include "AReVi/activity.h"
#include "AReViNS/remoteViewer3D.h"

namespace AReViMascaret
{

class AREVI_MASCARET_API AReViMascaretClient : public AReVi::ArObject
{
public:
	AR_CLASS(AReViMascaretClient)
	AR_CONSTRUCTOR_2(AReViMascaretClient, AReVi::StlString, hostname, int, port)
	virtual bool activate(AReVi::ArRef<AReVi::Activity>, double dt);

	virtual void onPing(const AReVi::RemoteService::PingEvent &event);
	virtual void onPing2(const AReVi::RemoteService::PingEvent &event);

	virtual bool showStats(void);

	virtual void onRemoteFileReception(const AReVi::RemoteDepService::RemoteFileReceptionEvent &event);
	virtual void onRemoteRefReception(const AReVi::RemoteDepService::RemoteRefReceptionEvent &event);
	virtual void onDeconnectViewer3D(const AReVi::NetworkService::ServiceDeconnectEvent &);

protected:
	AReVi::ArRef<AReVi::RemoteViewer3D> _client;
	AReVi::ArRef<AReVi::RemoteDepService> _depService;
	bool _ready;
	unsigned int _dataSize;
	unsigned int _connectionTime;
	unsigned int _depPing;
	unsigned int _viewerPing;
};
}

#endif
