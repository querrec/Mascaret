/*
 * AReViMascaretApplication.cpp
 *
 *  Created on: 28 févr. 2011
 *      Author: soler
 */
#include "AReVi/arSystem.h"

#include "IEHA/Visualisation/Window3D.h"

#include "VRPlateform/AReViMascaretApplication.h"
#include "VRPlateform/AReViWindow3D.h"
#include "VEHA/Behavior/Common/behaviorScheduler.h"
#include "AReViVRComponentFactory.h"
#include "IEHA/Interaction/PaddlePeripheric.h"
#include "Network/AReViMascaretClient.h"
#include "Network/AReViMascaretServer.h"
#include "Interaction/AReViMouse.h"
#include "VEHA/Behavior/Common/pluginManager.h"

#include <boost/shared_ptr.hpp>

using boost::shared_ptr;

using namespace IEHA;
namespace AReViMascaret
{
AReViMascaretApplication* AReViMascaretApplication::_instance=NULL;
AReViMascaretApplication::AReViMascaretApplication()
{
	cerr << "AReViMascaret build datetime: " << __DATE__ << " " << __TIME__ << endl;
	if(_instance)
		throw std::runtime_error("Une autre instance de AReViMascaretApplication a déjà été créée");
	 _instance=this;
	setVRComponentFactory(shared_ptr<VEHA::VirtualRealityComponentFactory>(new AReViVRComponentFactory()));
}
AReViMascaretApplication::~AReViMascaretApplication()
{

}
void AReViMascaretApplication::parse(const string& name)
{
#if 0 // Moved to Windows3D
  shared_ptr<AReViMouse> mouse;
  shared_ptr<AReViKeyboard> keyboard;
  keyboard=shared_ptr<AReViKeyboard>(new AReViKeyboard());
  mouse=shared_ptr<AReViMouse>(new AReViMouse());
  addPeripheric(mouse);
  addPeripheric(keyboard);
#endif
	
	
	VRApplication::parse(name);
#if 0 // Moved to Windows3D
	if(accessWindow())
	{
	  (shared_dynamic_cast<AReViWindow3D>(accessWindow()))->setKeyboard(keyboard);
	  (shared_dynamic_cast<AReViWindow3D>(accessWindow()))->setMouse(mouse);
	
	  shared_ptr<AReViCursor> cursor(new AReViCursor());
	  cursor->setRenderer(shared_dynamic_cast<AReViWindow3D>(accessWindow())->getAReViViewer());
	  addCursor(cursor);
	}
#endif
}
bool AReViMascaretApplication::step()
{
	bool res = ArSystem::simulationStep();
	return res && VRApplication::step();
}
void AReViMascaretApplication::createDistributionServer(const string& sessionName, int port)
{
	StlVector< ArRef<Viewer3D> > vs;
	Viewer3D::CLASS()->getInstances(vs,true);

	ArRef<AReViMascaretServer> server=AReViMascaretServer::NEW(vs[0]->accessScene(),true, port);
	server->setTransient(false);
	server->accessServer()->setSessionName(sessionName);
	// All derived classes
	//    server->accessServer()->accessDepService()
	//                          ->registerDownCast("Object3D","Object3D",true);
	server->accessServer()->accessDepService()
                        		  ->registerDownCast("SoundPlayer3D","URLSound3D",true);
	server->accessServer()->accessDepService()
                        		  ->registerDownCast("MorphVrmlShape3D","VrmlShape3D",true);

	server->setAvatarSize(1.0);
}
void AReViMascaretApplication::createClient(const string& hostname, int port)
{
	ArRef<AReViMascaretClient> client = AReViMascaretClient::NEW(hostname, port);
	client->setTransient(false);
}
}
