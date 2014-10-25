#ifndef OGRE_INTERFACE_3D_H
#define OGRE_INTERFACE_3D_H


#include "VEHA/Entity/interface3D.h"
#include "Ogre.h"
#include "ExampleFrameListener.h"
//using namespace VEHA::Entity;
class OgreMascaret;
class MascaretFrameListener : public ExampleFrameListener
{
public :
	MascaretFrameListener(RenderWindow* win, Camera* cam)
	: ExampleFrameListener(win, cam){}
	bool frameStarted(const FrameEvent & evt);
	bool frameEnded(const FrameEvent & evt);
	//   SceneNode * _sc;
};


class OgreInterface3D: public Interface3D
{
public:
	static void init();
	void initOgre();

	inline void setApplication(OgreMascaret * appli) {_appli = appli;}
	inline OgreMascaret * getApplication(void) {return _appli;}
	virtual void createSnapshot(string filename);

	//Object
	//return id
	virtual long createShape(const string& fileName,long parentID=0, bool movable=true,const string& shader="");
	virtual long createLODShapes(const vector<double>& distances,const vector<string>& fileNames,long parentID, bool movable=true, const string& shader="");
	//return id
	virtual long createNode(long parentID=0);

	virtual void setScale(long id,const VEHA::Entity::Vector3& scale);
	virtual VEHA::Entity::Vector3 getScale(long id);

	virtual void createAnimation(const string & name, const string & url, shared_ptr<VEHA::Entity::Entity> e);
	virtual bool playAnimation(long id, const string & name, int sens = 1, double animationSpeed = 1.0, bool cycle = false);
	virtual bool stopAnimation(long id);

	virtual void createSound(const string & name, const string & url, shared_ptr<VEHA::Entity::Entity> e,double gain,double minDistance,double maxDistance);
	virtual bool playSound(long id, const string & name, double pitch = 1.0, bool cycle = false);

	//Get/Set Local Position
	/*
                    virtual bool getPosition(long  id, double &x,double&y,double& z);
                    virtual bool setPosition(long id,double x,double y,double z);
	 */
	virtual VEHA::Entity::Vector3 getPosition(long  id);
	virtual bool setPosition(long id,const VEHA::Entity::Vector3& pos);
	virtual bool translate(long id,const VEHA::Entity::Vector3& pos);
	/*
                    virtual bool getOrientation(long id,double& roll, double& pitch, double& yaw);
                    virtual bool setOrientation(long id,double roll, double pitch, double yaw);
	 */
	virtual VEHA::Entity::Vector3 getOrientation(long id);
	virtual bool setOrientation(long id,const VEHA::Entity::Vector3& ori);

	virtual void localToGlobalOrientation(long id, double & x, double & y, double & z);

	virtual RotationVector getRotation(long id);
	virtual bool setRotation(long id,const RotationVector& rot);

	virtual void deleteShape(long id);
	virtual void deleteNode(long id);

	virtual void setParent(long id,long parentId);
	//	virtual void detach();
	//	virtual void attach();

	virtual void playAnimation(long objectId,long animationNumber);

	virtual bool isCurrentlyDisplayed(long id);
	virtual void setVisibility(long id,bool);
	virtual bool getVisibility(long id);


	/* Effects*/
	virtual void setDisplayBoundingBox(long id, bool val);
	virtual bool getDisplayBoundingBox(long id);

	virtual void setDisplayObjectName(long id,bool);
	virtual bool getDisplayObjectName(long id);

	virtual void setDisplayProperty(long id,bool);
	virtual bool getDisplayProperty(long id);

	virtual void setTransparency(long id,float t);
	virtual float getTransparency(long id);

	virtual void restore(long id);
	virtual void restoreAll();
	virtual void setRed(long id);

	virtual void setBlink(long id,bool);
	virtual bool getBlink(long id);

	/* General effect */
	virtual bool effectExist(const string& effectName);
	virtual bool setEffect(const string& effectName, const vector<string>& parameters);
	virtual vector<string> getEffectList();

	/* Renderer */
	virtual void setRendererParameters(const RendererParameters &) {}

	/* Navigation */
	virtual void initNavigation(const NavigationParameters & ){}

	/* multiple calls to add multiple scenes */
	virtual void addScene(const SceneParameters &){}
	/*
                enum CameraType{CAMERA_FPS,CAMERA_FIXED,CAMERA_FREE,CAMERA_TRACK,CAMERA_TRACK_FIXED};
                virtual void setCameraType(CameraType);
                virtual CameraType getCameraType();
                virtual void setCameraTarget(long id);*/
	virtual void setCameraPosition(double x,double y,double z);
	virtual void setCameraOrientation(double x,double y,double z);
	virtual void getCameraPosition(double& x,double& y,double& z);
	virtual void getCameraOrientation(double& x,double& y,double& z);

	/* Mouse Management */
	virtual void showMouse(bool);
	virtual void selectObjectOnClick(bool );
	virtual void dragAndDropWithMouse(bool);

	/* Menu */
	virtual long createMenu(string fileName);
	virtual void showMenu(long id);
	virtual void hideMenu(long id);

	Ogre::Root*  getRoot(){return _root;}
protected:
	Ogre::Root *  _root;
	Ogre::SceneManager * _sceneMgr;
	Ogre::Camera * _camera;
	Ogre::Viewport * _vp;
	MascaretFrameListener * _fl;
public:
	map<long,Ogre::SceneNode* > _nodes;
	map<long,long > _parent;

	OgreMascaret * _appli;
};
#endif

