#include "ogreInterface3D.h"
#include "ogreMascaret.h"
using boost::shared_dynamic_cast;
#define INSTANCE (shared_dynamic_cast<OgreInterface3D>( _instance))


void eulToVecRot( double RotX, double RotY, double RotZ,RotationVector& quat)
{

      double ti = RotX*0.5;
      double tj = RotY*0.5;
      double th = RotZ*0.5;



      double ci = cos(ti);
      double cj = cos(tj);
      double ch = cos(th);
      double si = sin(ti);
      double sj = sin(tj);
      double sh = sin(th);
      double cc = ci*ch;
      double cs = ci*sh;
      double sc = si*ch;
      double ss = si*sh;
      quat.angle = cj*cc + sj*ss;
      quat.x = cj*sc - sj*cs;
      quat.y = cj*ss + sj*cc;
      quat.z = cj*cs - sj*sc;
/*
      double q0 = cj*cc + sj*ss;
      double q1 = cj*sc - sj*cs;
      double q2 = cj*ss + sj*cc;
      double q3 = cj*cs - sj*sc;


      quat.angle = 2 * acos(q0);

      if (fabs(quat.angle) < 0.000001)
      {
		quat.x=1;
		quat.y=0;
		quat.z=0;
      }
      else
      {
	      //double sphi = 1.0/sqrt(1.0 - (q0*q0));
	      double sphi = 1.0/sqrt(q1*q1+q2*q2+q3*q3);

	      quat.x=q1*sphi;
	      quat.y=q2*sphi;
	      quat.z=q3*sphi;
      }


*/
      return;
}


bool MascaretFrameListener::frameStarted(const FrameEvent & evt)
{
}
bool MascaretFrameListener::frameEnded(const FrameEvent & evt)
{
        if( (shared_dynamic_cast<OgreInterface3D>(OgreInterface3D::get()))->getApplication())
           (shared_dynamic_cast<OgreInterface3D>(OgreInterface3D::get()))->getApplication()->step();
        else cerr << "PAS d'APPLIE ..." << endl;


}
void OgreInterface3D::init()
{
	
	_instance=shared_ptr<Interface3D>(new OgreInterface3D());
	INSTANCE->initOgre();
}
void OgreInterface3D::initOgre()
{
   _root = new Root("./plugins.cfg","./ogre.cfg");
   if (!_root->restoreConfig())
      _root->showConfigDialog();
   _root->initialise(true,"Test");

   _sceneMgr = _root->createSceneManager(ST_GENERIC,"Scene Manager");

   ResourceGroupManager * rgm = ResourceGroupManager::getSingletonPtr();
   rgm->addResourceLocation(".","FileSystem","General");
   rgm->addResourceLocation("/","FileSystem","General");
   rgm->addResourceLocation("./materials","FileSystem","General");
   rgm->initialiseAllResourceGroups();

   //createCamera("mainCam");


   _camera = _sceneMgr->createCamera("MainCam");
   _camera->setNearClipDistance(5);
   _camera->setFarClipDistance(1000);
   _camera->setAspectRatio(Real(1.333333));
   _camera->setFixedYawAxis(true,Ogre::Vector3::UNIT_Z);
   _vp = _root->getAutoCreatedWindow()->addViewport(_camera);
   _vp->setBackgroundColour(ColourValue(0,0,0.5));


	_nodes[0]=_sceneMgr->getRootSceneNode()->createChildSceneNode();
		_nodes[0]->setFixedYawAxis(true,Ogre::Vector3::UNIT_Z);

   _fl = new MascaretFrameListener(_root->getAutoCreatedWindow(),_camera);
   _root->addFrameListener(_fl);
   _sceneMgr->setAmbientLight(ColourValue(0.7,0.7,0.7));
	_nodes[0]->attachObject(_sceneMgr->createLight("Lumiere"));
	_nodes[0]->attachObject(_camera);
   //parse(filename);

//   _root->startRendering();
}   


void OgreInterface3D::createSnapshot(string filename)
{

}



long OgreInterface3D::createShape(const string& fileName,long parentID, bool movable,const string& shader)
{
	//  ShapeSpecification *specif = entity->get_current();
	//  if (specif)
		long id=_nodes.rbegin()->first+1;
	     SceneNode *sc;

//	     cout << " ****** OgreInterface3D::createShape   " << fileName << endl;

	     Ogre::Entity * ent = _sceneMgr->createEntity(fileName,fileName);

	/*
	     if (entity->get_parent()!= NULL)
	     {
	        map< string, SceneNode*>::iterator it = _objects.find(entity->get_parent()->get_id());
	        if (it != _objects.end())
	        {
	           SceneNode * pSc = it->second;
	           sc = pSc->createChildSceneNode();
	        }
	        else cerr << "Object hierarchy problem...." << endl;
	     }
	     else sc = _sceneMgr->getRootSceneNode()->createChildSceneNode();
	*/
	     SceneNode * pSc = _nodes[parentID];
	     sc = pSc->createChildSceneNode();
	     sc->setInheritOrientation(true);
	     sc->setFixedYawAxis(true, Ogre::Vector3::UNIT_Z);
		_parent[id]=parentID;
		sc->attachObject(ent);

		_nodes[id]=sc;
		return id;
}
long OgreInterface3D::createLODShapes(const vector<double>& distances,const vector<string>& fileNames,long parentID, bool movable, const string& shader)
{
	createShape(fileNames[0],parentID,movable,shader);
}

long OgreInterface3D::createNode(long parentID)
{
//  cout << " ****** OgreInterface3D::createNode   " << parentID << endl;
	long id=_nodes.rbegin()->first+1;
     SceneNode *sc;
//	if (parentID)
//	{
	           SceneNode * pSc = _nodes[parentID];
        	    sc= pSc->createChildSceneNode();
		sc->setFixedYawAxis(true, Ogre::Vector3::UNIT_Z);
//	sc->setInheritOrientation(true);
//	}
//	else sc = _sceneMgr->getRootSceneNode()->createChildSceneNode();
	_parent[id]=parentID;
//	_nodes[id]=_nodes[parentID]->createChildSceneNode();
	_nodes[id]=sc;
	return id;
}
//Get/Set Local Position
VEHA::Entity::Vector3 OgreInterface3D::getPosition(long  id)
{
	Ogre::Vector3 v=
	_nodes[id]->getPosition();
	return VEHA::Entity::Vector3(v.x,v.y,v.z);
}
void OgreInterface3D::setScale(long id,const VEHA::Entity::Vector3& scale)
{
	_nodes[id]->setScale(scale.x,scale.y,scale.z);
}
VEHA::Entity::Vector3 OgreInterface3D::getScale(long id)
{
	
}
bool OgreInterface3D::setPosition(long id,const VEHA::Entity::Vector3& pos)
{
_nodes[0]->pitch(Degree(90));
	_nodes[id]->setPosition(pos.x,pos.y,pos.z);
_nodes[0]->pitch(Degree(-90));
//	_nodes[id]->translate(pos.x,pos.y,pos.z,SceneNode::TS_WORLD);
	return true;
}

 VEHA::Entity::Vector3 OgreInterface3D::getOrientation(long id)
{	
	VEHA::Entity::Vector3 b;
//	_nodes[id]->getOrientation();
	cerr<<"NOT implemented     "<<endl;
 	
	return b;
}
 bool OgreInterface3D::setOrientation(long id,const VEHA::Entity::Vector3& ori)
{
	
	RotationVector rot;
	eulToVecRot(ori.x,ori.y,ori.z,rot);
	_nodes[id]->setOrientation(rot.angle,rot.x,rot.y,rot.z);
	return true;
}
RotationVector OgreInterface3D::getRotation(long id)
{
	RotationVector q;
	Quaternion ogreQ= _nodes[id]->getOrientation();
	q.x=ogreQ.x;
	q.y=ogreQ.y;
	q.z=ogreQ.z;
	q.angle=ogreQ.w;
	return q;
}
bool OgreInterface3D::setRotation(long id,const RotationVector& rot)
{
	_nodes[id]->resetOrientation();
	Ogre::Vector3 axis;axis.x=rot.x;axis.y=rot.y;axis.z=rot.z;
	_nodes[id]->rotate(axis,Radian(rot.angle));

	return true;
}

void OgreInterface3D::deleteShape(long id)
{
	cerr<<"delete"<<endl;
	_sceneMgr->destroySceneNode(_nodes[id]);
	_nodes.erase(id);
	//DESTROY ENTITY TODO
}
void OgreInterface3D::deleteNode(long id)
{
	_sceneMgr->destroySceneNode(_nodes[id]);
	_nodes.erase(id);
}

void OgreInterface3D::setParent(long id,long parentId)
{
	_nodes[_parent[id]]->removeChild(_nodes[id]);
	_nodes[parentId]->addChild(_nodes[id]);
	_parent[id]=parentId;
}



void OgreInterface3D::createAnimation(const string & name, const string & url, shared_ptr<VEHA::Entity::Entity> e)
{

}
bool OgreInterface3D::playAnimation(long id, const string & name, int sens, double animationSpeed, bool cycle)
{
	return false;
}
bool OgreInterface3D::stopAnimation(long id)
{
	return false;
}

void OgreInterface3D::createSound(const string & name, const string & url, shared_ptr<VEHA::Entity::Entity> e,double gain,double minDistance,double maxDistance)
{

}
bool OgreInterface3D::playSound(long id, const string & name, double pitch, bool cycle)
{
	return false;
}

bool OgreInterface3D::translate(long id,const VEHA::Entity::Vector3& pos)
{
	return false;
}

void OgreInterface3D::localToGlobalOrientation(long id, double & x, double & y, double & z)
{

}
/*
void OgreInterface3D::detach()
{

}
void OgreInterface3D::attach()
{

}*/

void OgreInterface3D::playAnimation(long objectId,long animationNumber)
{

}

bool OgreInterface3D::isCurrentlyDisplayed(long id)
{
	return false;
}
void OgreInterface3D::setVisibility(long id,bool)
{

}
bool OgreInterface3D::getVisibility(long id)
{
	return false;
}



void OgreInterface3D::setDisplayBoundingBox(long id, bool val)
{

}
bool OgreInterface3D::getDisplayBoundingBox(long id)
{
	return false;
}

void OgreInterface3D::setDisplayObjectName(long id,bool)
{

}

bool OgreInterface3D::getDisplayObjectName(long id)
{
	return false;
}

void OgreInterface3D::setDisplayProperty(long id,bool)
{

}
bool OgreInterface3D::getDisplayProperty(long id)
{
	return false;
}

void OgreInterface3D::setTransparency(long id,float t)
{

}
float OgreInterface3D::getTransparency(long id)
{
	return 1;
}

void OgreInterface3D::restore(long id)
{
}
void OgreInterface3D::restoreAll()
{

}
void OgreInterface3D::setRed(long id)
{

}

void OgreInterface3D::setBlink(long id,bool)
{

}
bool OgreInterface3D::getBlink(long id)
{
	return false;
}

bool OgreInterface3D::effectExist(const string& effectName)
{
	return false;
}
bool OgreInterface3D::setEffect(const string& effectName, const vector<string>& parameters)
{
	return false;
}

vector<string> OgreInterface3D::getEffectList()
{
	return vector<string>();
}


void OgreInterface3D::setCameraPosition(double x,double y,double z)
{

}
void OgreInterface3D::setCameraOrientation(double x,double y,double z)
{

}
void OgreInterface3D::getCameraPosition(double& x,double& y,double& z)
{

}
void OgreInterface3D::getCameraOrientation(double& x,double& y,double& z)
{

}


void OgreInterface3D::showMouse(bool)
{

}
void OgreInterface3D::selectObjectOnClick(bool )
{

}
void OgreInterface3D::dragAndDropWithMouse(bool)
{

}


long OgreInterface3D::createMenu(string fileName)
{
	return -1;
}
void OgreInterface3D::showMenu(long id)
{

}
void OgreInterface3D::hideMenu(long id)
{

}
