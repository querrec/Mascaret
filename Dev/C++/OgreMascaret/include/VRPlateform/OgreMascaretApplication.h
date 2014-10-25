/*
 * AReViMascaretApplication.h
 *
 *  Created on: 27 févr. 2011
 *      Author: ping
 */

#ifndef OGREMASCARETAPPLICATION_H_
#define OGREMASCARETAPPLICATION_H_
#include "ogreMascaret_plateform.h"
#include "IEHA/VRApplication.h"

namespace Sound
{
	class Sound3DPlayer;
}
namespace Ogre
{
    class Root;
    class SceneManager;
}

namespace OgreMascaret
{
using IEHA::VRApplication;


class OGRE_MASCARET_API OgreMascaretApplication : public VRApplication
{
public:
	OgreMascaretApplication();
	virtual ~OgreMascaretApplication();

	virtual bool step();
	virtual void run();
	virtual void init();
	virtual void createDistributionServer(const string& sessionName, int port);
	virtual void createClient(const string& hostname, int port);
	virtual void parse(const string& name);
	static inline OgreMascaretApplication* getInstance();

	inline Ogre::Root * getRoot() {return _root;}
	inline Ogre::SceneManager * getSceneManager(){return _sceneMgr;}

	Sound::Sound3DPlayer * getSoundPlayer();
protected:
	Ogre::Root *  _root;
	Ogre::SceneManager * _sceneMgr;
	Sound::Sound3DPlayer * _soundPlayer;
private:
	static OgreMascaretApplication* _instance;
};

OgreMascaretApplication* OgreMascaretApplication::getInstance()
{
	return _instance;
}


inline Sound::Sound3DPlayer * OgreMascaretApplication::getSoundPlayer()
{
	return _soundPlayer;
}

}
#endif /* AREVIMASCARETAPPLICATION_H_ */
