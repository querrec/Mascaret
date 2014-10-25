#include <stdio.h>
#include <iostream>

#include "Tools/VEHAApplication.h"

#include <Ogre.h>
//#include "OgreConfigFile.h"

//using namespace Ogre;
using namespace std;

class OgreMascaret: public VEHAApplication
{
public :
   OgreMascaret(string filename);
   ~OgreMascaret();
	virtual bool step();
//   void createCamera(string cameraName);
//   void createScene(void);
//   void createShape(VEHA::Entity::Entity * entity);
//   void setPosition(VEHA::Entity::Entity * entity);
    virtual void createDistributionServer(string sessionName, int port);
    virtual void createClient(string hostname, int port);
protected :
//   Root * _root;
//   SceneManager * _sceneMgr;
//   Camera * _camera;
//   Viewport * _vp;

//   map < string, SceneNode* > _objects;
//   MascaretFrameListener * _fl;

};

