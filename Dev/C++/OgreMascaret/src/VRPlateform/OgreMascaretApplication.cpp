/*
 * OgreMascaretApplication.cpp
 *
 *  Created on: 28 févr. 2011
 *      Author: soler
 */
#ifdef _WIN32
#include <winsock2.h>
#endif
#include "VRPlateform/OgreMascaretApplication.h"
#include "VRPlateform/OgreWindow3D.h"
#include "VEHA/Behavior/Common/behaviorScheduler.h"
#include "OgreVRComponentFactory.h"
#include "Tools/Sound3D.h"

using namespace IEHA;
using namespace Ogre;
namespace OgreMascaret
{
OgreMascaretApplication* OgreMascaretApplication::_instance=NULL;
OgreMascaretApplication::OgreMascaretApplication()
{
	if(_instance)
		throw std::runtime_error("Une autre instance de OgreMascaretApplication a déjà été créée");
	_instance=this;

	// Desactive affichage log Ogre (mais toujours dans ogre.log)
	Ogre::LogManager* logMgr = OGRE_NEW Ogre::LogManager;
	logMgr->createLog("./OgreEnv/ogre.log", true, false, false);

#if defined(_DEBUG) && defined(_WIN32)
	_root = new Root("./OgreEnv/plugins_d.cfg","./OgreEnv/ogre_d.cfg", "./OgreEnv/ogre.log");
	 string mResourcesCfg = "./OgreEnv/resources_d.cfg";
#else
	_root = new Root("./OgreEnv/plugins.cfg","./OgreEnv/ogre.cfg", "./OgreEnv/ogre.log");
	string mResourcesCfg = "./OgreEnv/resources.cfg";
#endif

//	LogManager::getSingletonPtr()->getDefaultLog()->setDebugOutputEnabled(false);

	if (!_root->restoreConfig())
		_root->showConfigDialog();
	_root->initialise(true,"Ogre Mascaret");

	_sceneMgr = _root->createSceneManager(ST_GENERIC,"Scene Manager");

	_soundPlayer = new Sound::Sound3DPlayer();

	ResourceGroupManager * rgm = ResourceGroupManager::getSingletonPtr();
	rgm->addResourceLocation(".","FileSystem","General");
	rgm->addResourceLocation("/","FileSystem","General");
	rgm->addResourceLocation("./materials","FileSystem","General");
	Ogre::ConfigFile cf;
	cf.load(mResourcesCfg);

	Ogre::ConfigFile::SectionIterator seci = cf.getSectionIterator();
 
	Ogre::String secName, typeName, archName;
	while (seci.hasMoreElements())
	{
		secName = seci.peekNextKey();
		Ogre::ConfigFile::SettingsMultiMap *settings = seci.getNext();
		Ogre::ConfigFile::SettingsMultiMap::iterator i;
		for (i = settings->begin(); i != settings->end(); ++i)
		{
			typeName = i->first;
			archName = i->second;
			Ogre::ResourceGroupManager::getSingleton().addResourceLocation(
			    archName, typeName, secName);
		}
	}

	rgm->initialiseAllResourceGroups();

}
OgreMascaretApplication::~OgreMascaretApplication()
{
	delete _soundPlayer;
}
void OgreMascaretApplication::init()
{
	setVRComponentFactory(shared_ptr<VEHA::VirtualRealityComponentFactory>(new OgreVRComponentFactory()));
}
void OgreMascaretApplication::parse(const string& name)
{
	VRApplication::parse(name);
}
bool OgreMascaretApplication::step()
{
	return VRApplication::step();
}
void OgreMascaretApplication::run()
{
	_root->startRendering();
}
void OgreMascaretApplication::createDistributionServer(const string& sessionName, int port)
{
}
void OgreMascaretApplication::createClient(const string& hostname, int port)
{
}
}
