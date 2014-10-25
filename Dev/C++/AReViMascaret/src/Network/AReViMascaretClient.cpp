#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include "AReVi/arSystem.h"
#include "AReVi/activity.h"
#include "AReVi/Lib3D/sound3D.h"
#include "AReViNS/remoteViewer3D.h"
#include "hLib/hLib.h"

#include "Network/AReViMascaretClient.h"
#include "Network/nsEventEndConnection.h"


using namespace hLib;

using namespace AReVi;

namespace AReViMascaret
{
AR_CLASS_NOVOID_DEF(AReViMascaretClient,ArObject)

AReViMascaretClient::AReViMascaretClient(ArCW & arCW, StlString hostname, int port)
: ArObject(arCW),_client(),_ready(false), _dataSize(0),
  _connectionTime(0), _depPing(0), _viewerPing(0)
{
	unsigned int distantIPAddress;

	distantIPAddress=ArSystem::getIPAddress(hostname);

	_depService=RemoteDepService::NEW(distantIPAddress,port);

	_client=RemoteViewer3D::NEW(_depService);
	_client->connect(distantIPAddress, port-1);

	if (!_client->fail() && !_depService->fail())
	{
		_connectionTime=ArSystem::getTime();
		_client->addServiceDeconnectCB(thisRef(),&AReViMascaretClient::onDeconnectViewer3D);
		_client->addPingCB(thisRef(),&AReViMascaretClient::onPing2);
		_client->accessDepService()->addPingCB(thisRef(),&AReViMascaretClient::onPing);
		_client->accessDepService()->addRemoteFileReceptionCB(thisRef(),&AReViMascaretClient::onRemoteFileReception);
		_client->accessDepService()->addRemoteRefReceptionCB(thisRef(),&AReViMascaretClient::onRemoteRefReception);

		/*
       StlString firefoxParam ="http://";
       firefoxParam += serverIP;
       firefoxParam += ":8080";
       cerr << firefoxParam << endl;

       pid_t child=fork();
       switch(child)
       {
          case 0 :
             execlp ("firefox", "firefox", firefoxParam.c_str());
             perror("execlp");
       }
		 */

		ArRef<Activity> act=Activity::NEW(2.0);
		act->setBehavior(thisRef(),&AReViMascaretClient::activate);

		// Show viewer during loading...
		_client->accessViewer()->setMapped(true);

	}
	else
	{
		if (_depService->fail())
			cerr << "Can't connect to server depService: " << endl << "   => " << _depService->getErrorMessage() << endl;
		else
			cerr << "Can't connect to server viewer : " << endl << "   => " << _client->getErrorMessage() << endl;

		_depService.clear();
		_client.clear();
	}
}

AReViMascaretClient::~AReViMascaretClient(void)
{
	if (_client.valid())
	{
		cerr << "Stop client" << endl;
		_client->setUpdateInterval(-1);//stop
		_client.clear();
	}
	cerr << "Leave simulation" << endl;
}

bool AReViMascaretClient::activate(ArRef<Activity>, double dt)
{
	StlVector<int> readingFDOut,writingFDOut;

	if (_ready)
	{
		unsigned int last(_dataSize);
		_dataSize=0;

		if (_depService.valid())
		{
			_dataSize+=_depService->getDataReadCount();
			_dataSize+=_depService->getDataWriteCount();
			_depService->ping();
		}
		else
			return false;

		if (_client.valid())
		{
			_dataSize+=_client->getDataReadCount();
			_dataSize+=_client->getDataWriteCount();
			_client->ping();
		}
		else
			return false;

		if ( _client->accessViewer().valid())
		{
			StlString title;
			title+="  DataRate="+doubleToStr(((int)((_dataSize-last)/(102.4*dt)))*0.1)+"ko/s";
			title+="  DepPing="+uintToStr(_depPing)+"ms";
			title+="  ViewerPing="+uintToStr(_viewerPing)+"ms";
			title+=" | " + doubleToStr(Viewer3D::getFrameRate()) + "fps";
			_client->accessViewer()->setTitle(title);
		}
		else
			return false;
	}
	else
	{
		if (_client.valid())
		{
			if (_client->accessDepService().valid())
			{
				unsigned int waitFor=_client->accessDepService()->getWaitingRefs().size()+_client->accessDepService()->getWaitingURLs().size();
				unsigned int have=_client->accessDepService()->getRemoteRefs().size()+_client->accessDepService()->getRemoteURLs().size();

				cerr << "Progress=" << (have?doubleToStr((1.0-(waitFor/(double)have))*100.0):"0.00") << "%"
						<< "   Refs : " << intToStr(_client->accessDepService()->getRemoteRefs().size()) << " (Waiting for " <<  _client->accessDepService()->getWaitingRefs().size() << ")"
						<< "   Urls : " << intToStr(_client->accessDepService()->getRemoteURLs().size()) << " (Waiting for " <<  _client->accessDepService()->getWaitingURLs().size() << ")"
						<< endl;
			}
		}
		else
			return false;
	}
	return true;
}

void
AReViMascaretClient::onPing(const RemoteService::PingEvent &event)
{
	_depPing=event.dt;
}

void
AReViMascaretClient::onPing2(const RemoteService::PingEvent &event)
{
	_viewerPing=event.dt;
}

bool
AReViMascaretClient::showStats(void)
{
	_dataSize=0;
	if (_depService.valid())
	{
		_dataSize+=_depService->getDataReadCount();
		_dataSize+=_depService->getDataWriteCount();
	}
	else
		return false;

	if (_client.valid())
	{
		_dataSize+=_client->getDataReadCount();
		_dataSize+=_client->getDataWriteCount();
	}
	else
		return false;

	double dt=ArSystem::getTime()- _connectionTime;
	cerr << "READY : Connection time=" << (dt)/1000.0 << "s" << endl
			<< "        Data Size =" << ((int)(_dataSize/102.4))*0.1 << "ko"<< endl
			<< "        Data Rate =" << doubleToStr(((int)((_dataSize)/(102.4*(dt/1000))))*0.1)+"ko/s" << endl;

	_client->sendEvent(NSEventEndConnection::NEW());
	return true;
}

void
AReViMascaretClient::onRemoteFileReception(const RemoteDepService::RemoteFileReceptionEvent &event)
{
	if (!event.source->getWaitingRefs().size() && !event.source->getWaitingURLs().size())
	{
		if (showStats())
		{
			_ready=true;

			_client->accessViewer()->setMapped(true);

			event.source->removeRemoteRefReceptionCB(thisRef(),&AReViMascaretClient::onRemoteRefReception);
			event.source->removeRemoteFileReceptionCB(thisRef(),&AReViMascaretClient::onRemoteFileReception);
		}
	}
}

void
AReViMascaretClient::onRemoteRefReception(const RemoteDepService::RemoteRefReceptionEvent &event)
{
	if (!event.source->getWaitingRefs().size() && !event.source->getWaitingURLs().size())
	{
		if (showStats())
		{
			_ready=true;

			_client->accessViewer()->setMapped(true);
			event.source->removeRemoteRefReceptionCB(thisRef(),&AReViMascaretClient::onRemoteRefReception);
			event.source->removeRemoteFileReceptionCB(thisRef(),&AReViMascaretClient::onRemoteFileReception);
		}
	}
}


void
AReViMascaretClient::onDeconnectViewer3D(const NetworkService::ServiceDeconnectEvent &)
{
	cerr << "Deconnection notification" << endl;
	_client->removePingCB(thisRef(),&AReViMascaretClient::onPing2);
	_client->accessDepService()->removePingCB(thisRef(),&AReViMascaretClient::onPing);
	_client->close();
	_client->setUpdateInterval(-1);
	if (_depService.valid())
	{
		_depService->setUpdateInterval(-1);
		_depService.clear();
	}

	_client=RemoteViewer3D::nullRef();
}

}
