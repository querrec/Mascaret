/*
 * OgreCamera.cpp
 *
 *  Created on: 25 févr. 2011
 *      Author: soler
 */

#include "VRPlateform/OgreCamera.h"
#include "VRPlateform/OgrePointSpecification.h"
#include "VRPlateform/OgreWindow3D.h"
#include "VRPlateform/OgreScene.h"
#include "VRPlateform/OgreMascaretApplication.h"
#include "VEHA/Entity/RotationVector.h"

#include "VEHA/Entity/VirtualRealityComponentFactory.h"


using VEHA::Color;
using boost::shared_ptr;
using namespace IEHA;
using namespace Ogre;
namespace OgreMascaret
{

OgreCamera::OgreCamera(shared_ptr<Window3D> window) :
    _camera(0),
    _cameraRight(0),
    mRaySceneQuery(0)
{
	_useHeadTracking = false;

	_window=shared_dynamic_cast<OgreWindow3D>(window);
	static int i=0;
	char name[16];
	snprintf(name, 16, "MainCam%i", i++);
	_camera = OgreMascaretApplication::getInstance()->getSceneManager()->createCamera(name);
	//_camera->setFixedYawAxis(true,Ogre::Vector3::UNIT_Z);
	_camera->setAutoAspectRatio(true);
	_window->getWindow()->addViewport(_camera);

	_location = make_shared<OgrePointSpecification>();
	_location->getNode()->setFixedYawAxis(true, Ogre::Vector3::UNIT_Z); // FIXME: I think it's wrong
	_wingTurret = make_shared<OgrePointSpecification>();
	_wingTurret->getNode()->setFixedYawAxis(true, Ogre::Vector3::UNIT_Z); // FIXME: I think it's wrong
	_wingTurret->setParent(_location);

	OgreMascaretApplication::getInstance()->getRoot()->addFrameListener(this);

	_userPosition = make_shared<OgrePointSpecification>();
	_userPosition->setParent(_wingTurret);

	_userLeftEye = make_shared<OgrePointSpecification>();
	_userLeftEye->setParent(_userPosition);
	_userRightEye = make_shared<OgrePointSpecification>();
	_userRightEye->setParent(_userPosition);

	_userLeftEye->getNode()->attachObject(_camera);

	// TODO: Configurable
	_userPod.physicalScreenHeight = 1.8;
	_userPod.physicalScreenWidth = 2.4;

	setBackgroundColor(Color(0.6,0.6,0.6));
	mRaySceneQuery = OgreMascaretApplication::getInstance()->getSceneManager()->createRayQuery( Ray( ) );

	_microphone = make_shared<Sound::Listener3D>();
	_microphone->setGain(0.0); // TODO: a renveler
	_manageMicrophone();
}

OgreCamera::~OgreCamera()
{
	OgreMascaretApplication::getInstance()->getSceneManager()->destroyQuery(mRaySceneQuery);
	if(_cameraRight)
		_deleteStereoCamera();
}

void OgreCamera::_createStereoCamera()
{
	_cameraRight = OgreMascaretApplication::getInstance()->getSceneManager()->createCamera(
			_camera->getName() + "_right");

	_userRightEye->getNode()->attachObject(_cameraRight);

	Ogre::NameValuePairList nvList;
	unsigned int width, height;
	unsigned int x, y;
	_window->getSize(width, height);
	_window->getPosition(x, y);
	{
		cout << "Windows Size: (" << width << "," << height << ")" << endl;
		std::stringstream s;
		s << width;
		nvList["left"] = s.str();
	}

	_windowRight = shared_dynamic_cast<OgreWindow3D>(
			OgreMascaretApplication::getInstance()->getVRComponentFactory()->createWindow3D());
	_windowRight->setSize(width, height);
	_windowRight->setPosition(x + width, y);
	Ogre::Viewport * v = _windowRight->getWindow()->addViewport(_cameraRight);

	_cameraRight->synchroniseBaseSettingsWith(_camera);
	setBackgroundColor(getBackgroundColor());
}

void OgreCamera::_deleteStereoCamera()
{
	OgreMascaretApplication::getInstance()->getSceneManager()->destroyCamera(_cameraRight);
	_cameraRight = NULL;
}

void OgreCamera::_updateFrustum()
{
	double horizontalOffset;
	double verticalOffset;
	double distanceToScreen;

	cout << "== Frustum Update ==" << endl;

	if(_useHeadTracking)
	{
		cout << "Head tracking activated" << endl;
		distanceToScreen = -_userPosition->getLocalPosition().x;
		if(distanceToScreen < 0.1)
			distanceToScreen = 0.1;
		verticalOffset = _userPosition->getLocalPosition().z;
		horizontalOffset = _userPosition->getLocalPosition().y;

		Radian FOVy = Radian(2*atan2(0.5 * _userPod.physicalScreenHeight, distanceToScreen));
		_camera->setFOVy(FOVy);
		if(_cameraRight)
		{
			_cameraRight->setFOVy(FOVy);
		}
		cout << "FOVy: " << Degree(FOVy) << endl;

		cout << "ViewerPosition, Local: " << _userPosition->getLocalPosition() << " Global: " << _userPosition->getGlobalPosition() << endl;
		cout << "Location, Local: " << _location->getLocalPosition() << " Global: " << _location->getGlobalPosition() << endl;
		cout << "Global Camera position: " << _location->getGlobalPosition() << endl;
		cout << "Distance to screen: " << distanceToScreen << endl;

		_stereoViewDistance = distanceToScreen;
	}
	else
	{
		cout << "Head tracking NOT activated" << endl;
		verticalOffset = 0;
		horizontalOffset = 0;
		distanceToScreen = 0.5 * _userPod.physicalScreenWidth / tan(0.5 * getFieldOfView());
		cout << "FOVy: " << Degree(Radian(getFieldOfView())) << endl;
	}

//	verticalOffset = 0.0;
//	horizontalOffset = 0.0;
//	distanceToScreen = 2.0;

	if(_cameraRight)
	{
		cout << "Stereo activated" << endl;
	    _camera->setNearClipDistance(0.1);
	    _cameraRight->setNearClipDistance(0.1);
	    //_camera->setNearClipDistance(_stereoViewDistance / 2);
	    //_stereoCamera->setNearClipDistance(_stereoViewDistance / 2);

	    double ratio = _camera->getAspectRatio();
	    double nearD = _camera->getNearClipDistance();
	    Radian FOVy = Radian(2*atan2(0.5 * _userPod.physicalScreenHeight, distanceToScreen));
	    double wd2 = nearD * tan(0.5 * FOVy.valueRadians());
	    double ndfl = nearD / _stereoViewDistance;

	    double left, right, top, bottom;

	    left  = -ratio * wd2 + (horizontalOffset + 0.5 * _stereoEyeSeparation) * ndfl;
	    right =  ratio * wd2 + (horizontalOffset + 0.5 * _stereoEyeSeparation) * ndfl;
	    top    =  wd2 - verticalOffset * ndfl;
	    bottom = -wd2 - verticalOffset * ndfl;

	    _camera->setFrustumExtents(left, right, top, bottom);

	    left  = -ratio * wd2 + (horizontalOffset - 0.5 * _stereoEyeSeparation) * ndfl;
	    right =  ratio * wd2 + (horizontalOffset - 0.5 * _stereoEyeSeparation) * ndfl;
	    top    =  wd2 - verticalOffset * ndfl;
	    bottom = -wd2 - verticalOffset * ndfl;

	    _cameraRight->setFrustumExtents(left, right, top, bottom);

	    cout << "Eye separation: " << _stereoEyeSeparation << " View distance: " << _stereoViewDistance << endl;
	}
	else
	{
		cout << "Stereo NOT activated" << endl;

	    double ratio = _camera->getAspectRatio();
	    Radian FOVx = Radian(2*atan2(0.5 * _userPod.physicalScreenWidth, distanceToScreen));
	    double nearD = _camera->getNearClipDistance();
	    double wd2 = nearD * tan(0.5 * FOVx.valueRadians());
	    double ndfl = nearD / _stereoViewDistance;

		double left, right, top, bottom;

	    left  = -ratio * wd2 + horizontalOffset * ndfl;
	    right =  ratio * wd2 + horizontalOffset * ndfl;
	    top    =  wd2 - verticalOffset * ndfl;
	    bottom = -wd2 - verticalOffset * ndfl;

	    _camera->setFrustumExtents(left, right, top, bottom);
	}
}

double OgreCamera::getAspectRatio() const
{
	return _camera->getAspectRatio();
}
void OgreCamera::setAspectRatio(double aspectRatio)
{
	_camera->setAspectRatio(Real(aspectRatio));
	if(_cameraRight)
		_cameraRight->setAspectRatio(Real(aspectRatio));
}

double OgreCamera::getFarDistance() const
{
	return _camera->getFarClipDistance();
}
void OgreCamera::setFarDistance(double farDistance)
{
	_camera->setFarClipDistance(farDistance);
	if(_cameraRight)
		_cameraRight->setFarClipDistance(farDistance);
}

double OgreCamera::getFieldOfView() const
{
	return _camera->getFOVy().valueRadians();
}
void OgreCamera::setFieldOfView(double fieldOfView)
{
	_camera->setFOVy(Radian(fieldOfView));
	if(_cameraRight)
		_cameraRight->setFOVy(Radian(fieldOfView));
}

double OgreCamera::getNearDistance() const
{
	return _camera->getNearClipDistance();
}
void OgreCamera::setNearDistance(double nearDistance)
{
	_camera->setNearClipDistance(nearDistance);
	if(_cameraRight)
		_cameraRight->setNearClipDistance(nearDistance);
}

Color OgreCamera::getBackgroundColor() const
{
	ColourValue const & c = _window->getWindow()->getViewport(0)->getBackgroundColour();
	return Color(c.r, c.g, c.b);
}
void OgreCamera::setBackgroundColor(const Color& color)
{
	_window->getWindow()->getViewport(0)->setBackgroundColour(ColourValue(color.r,color.g,color.b));
	if(_windowRight)
		_windowRight->getWindow()->getViewport(0)->setBackgroundColour(ColourValue(color.r,color.g,color.b));
}
void OgreCamera::setStereo(bool stereo)
{
	if(getStereo() == true && stereo == false)
	{
		// Remove stereo rendering
		_deleteStereoCamera();
		_updateFrustum();
	}
	else if(getStereo() == false && stereo == true)
	{
		// Create stereo rendering
		_createStereoCamera();
		_updateFrustum();
	}
}
bool OgreCamera::getStereo() const
{
	return _cameraRight ? true : false;
}

double OgreCamera::getStereoEyeSeparation() const
{
	return _stereoEyeSeparation;
}
void OgreCamera::setStereoEyeSeparation(double stereoEyeSeparation)
{
	_stereoEyeSeparation = stereoEyeSeparation;

	if(getStereo())
	{
		_userLeftEye->setLocalPosition(Vector3(0, _stereoEyeSeparation / 2.0, 0));
		_userRightEye->setLocalPosition(Vector3(0, -_stereoEyeSeparation / 2.0, 0));
	}
	else
	{
		_userLeftEye->setLocalPosition(Vector3(0, 0, 0));
		_userRightEye->setLocalPosition(Vector3(0, 0, 0));
	}

	_updateFrustum();
}

double OgreCamera::getStereoViewDistance() const
{
	return _stereoViewDistance;
}
void OgreCamera::setStereoViewDistance(double stereoViewDistance)
{
	_stereoViewDistance = stereoViewDistance;
	_updateFrustum();
}


shared_ptr<PointSpecification> OgreCamera::getGlobalLocation() const
{
	shared_ptr<OgrePointSpecification> point(new OgrePointSpecification());
	point->setGlobalPosition(_location->getGlobalPosition());
	Vector3 ori=_wingTurret->getGlobalOrientation();
    ori.z=_location->getGlobalOrientation().z;
    point->setGlobalOrientation(ori);
	return point;
}
void OgreCamera::setGlobalLocation(shared_ptr<PointSpecification>& loc,Navigation::NavigationMode mode)
{
	_location->setGlobalPosition(loc->getGlobalPosition());
	Vector3 ori=loc->getGlobalOrientation();
	if(mode == Navigation::HUMAN)
	{
		_location->setGlobalOrientation(Vector3(0,0,ori.z));
		_wingTurret->setGlobalOrientation(Vector3(ori.x,ori.y,0));
	}
	else
	{
		_location->setGlobalOrientation(ori);
	}

	_manageMicrophone();
}

void OgreCamera::translate(const Vector3& delta,Navigation::NavigationMode mode)
{
	if(mode == Navigation::HUMAN)
	{
		//TODO: check orientation of wingTurret and change if needed
	}
	_location->translate(delta);

	_manageMicrophone();
}
void OgreCamera::rotate(const RotationVector& delta,Navigation::NavigationMode mode)
{
	if(mode == Navigation::HUMAN)
	{
		_location->rotate(delta);
		Vector3 ori=_location->getLocalOrientation();
		_location->setLocalOrientation(Vector3(0,0,ori.z));
		_wingTurret->setLocalOrientation(Vector3(ori.x,ori.y,0));
	}
	else
		_location->rotate(delta);

	_manageMicrophone();
}
void OgreCamera::rotate(const Vector3& delta,Navigation::NavigationMode mode)
{
	if(mode == Navigation::HUMAN)
	{
		_location->rotate(Vector3(0,0,delta.z));
		_wingTurret->rotate(Vector3(delta.x,delta.y,0));
	}
	else
		_location->rotate(delta);

	_manageMicrophone();
}
void OgreCamera::gotoViewPoint(const string& name)
{
	shared_ptr<Zone> zone=accessCurrentScene()->getZone();
	if(zone)
	{
		shared_ptr<PointSpecification> viewPoint=zone->getViewPoint(name);
		if(viewPoint)
		{
			cout << "OgreCamera::gotoViewPoint("<<name<<") ";
			_location->setParent(viewPoint);
			_location->setLocalPosition(Vector3(0,0,0));
			_location->setLocalRotation(RotationVector(0,0,0,0));
			cout << _location->getGlobalPosition() << endl;
			//_location->setGlobalPosition(viewPoint->getGlobalPosition());
			//_location->setGlobalRotation(viewPoint->getGlobalRotation());
			_currentViewPoint = name;
		}
	}

	_manageMicrophone();
}
const shared_ptr<Scene> OgreCamera::getCurrentScene() const
{
	return _scene;
}
shared_ptr<Scene> OgreCamera::accessCurrentScene()
{
	return _scene;
}
void OgreCamera::setCurrentScene(shared_ptr<Scene> scene)
{
	_scene=scene;
}

void OgreCamera::_manageMicrophone()
{
	_microphone->setPosition(_location->getLocalPosition());
	Ogre::Vector3 odir = _camera->getRealDirection();
	Ogre::Vector3 oup = _camera->getRealUp();
	VEHA::Vector3 dir = VEHA::Vector3(odir.x, odir.y, odir.z);
	VEHA::Vector3 up = VEHA::Vector3(oup.x, oup.y, oup.z);
	_microphone->setOrientation(dir, up); // TODO: Voir les attachements dans OGRE
}

shared_ptr<VEHA::Entity> OgreCamera::getEntity(int x, int y, double& distOut) const
{
	std::vector<Ogre::String > listEnt;
	return getEntity(x, y, distOut, listEnt, true, true);
}

shared_ptr<VEHA::Entity> OgreCamera::getEntity(int x, int y, double& distOut,
		bool excludeHidden) const
{
	std::vector<Ogre::String > listEnt;
	return getEntity(x, y, distOut, listEnt, true, excludeHidden);
}

shared_ptr<VEHA::Entity> OgreCamera::getEntity(int x, int y, double& distOut,
		std::vector<Ogre::String > listEnt,  bool blackList, bool excludeHidden) const
{
	shared_ptr<VEHA::Entity> entity;
	VEHA::Vector3 pos;
	_getEntityAndPoint(x, y, listEnt, blackList, excludeHidden, entity, pos, distOut);
	return entity;
}

VEHA::Vector3 OgreCamera::getPoint(int x, int y) const
{
	shared_ptr<VEHA::Entity> entity;
	VEHA::Vector3 pos;
	double distOut;
	std::vector<Ogre::String > listEnt;
	_getEntityAndPoint(x, y, listEnt, true, true, entity, pos, distOut);
	return pos;
}

void OgreCamera::_getEntityAndPoint(int x, int y,
		std::vector<Ogre::String > listEnt,  bool blackList, bool excludeHidden,
		shared_ptr<VEHA::Entity> & entity, VEHA::Vector3 & pos, double & distOut
		) const
{
	// TODO: Gestion listEnt et excludeHidden
	unsigned int width,height;
	_window->getSize(width,height);


	// Setup the ray scene query
	Ray mouseRay = _camera->getCameraToViewportRay(x/(float)width,y/(float)height);
	Ogre::Vector3 source = mouseRay.getOrigin();
	Ogre::Vector3 target = mouseRay.getDirection();

	mRaySceneQuery->setRay(mouseRay);
	mRaySceneQuery->setSortByDistance(true);
	mRaySceneQuery->setQueryTypeMask(SceneManager::ENTITY_TYPE_MASK);
	// Execute query
	RaySceneQueryResult &query_result = mRaySceneQuery->execute();
	RaySceneQueryResult::iterator itr;
//	shared_ptr<VEHA::Entity> entity;


    // at this point we have raycast to a series of different objects bounding boxes.
    // we need to test these different objects to see which is the first polygon hit.
    // there are some minor optimizations (distance based) that mean we wont have to
    // check all of the objects most of the time, but the worst case scenario is that
    // we need to test every triangle of every object.
    Ogre::Real closest_distance = -1.0f;
    Ogre::Vector3 closest_result;

    for (size_t qr_idx = 0; qr_idx < query_result.size(); qr_idx++)
    {
        // stop checking if we have found a raycast hit that is closer
        // than all remaining entities
        if ((closest_distance >= 0.0f) &&
            (closest_distance < query_result[qr_idx].distance))
        {
             break;
        }
 
        // only check this result if its a hit against an entity
        if ((query_result[qr_idx].movable != NULL) &&
            (query_result[qr_idx].movable->getMovableType().compare("Entity") == 0))
        {
            // get the entity to check
            Ogre::Entity *pentity = static_cast<Ogre::Entity*>(query_result[qr_idx].movable);           
 
            // mesh data to retrieve         
            size_t vertex_count;
            size_t index_count;
            Ogre::Vector3 *vertices;
            unsigned long *indices;
 
//            // get the mesh information
//			getMeshInformation(pentity, vertex_count, vertices, index_count, indices,
//				pentity->getParentNode()->_getDerivedPosition(),
//				pentity->getParentNode()->_getDerivedOrientation(),
//                pentity->getParentNode()->_getDerivedScale());
 
            // detect if in object list
            bool entOnList = false;
            //cout << " " << pentity->getName();
            shared_ptr<Entity> tmpEnt = OgreShapeSpecification::getEntityFromObject3D(pentity);
            if(tmpEnt)
            {
              for(size_t i = listEnt.size(); i--;)
              {
                if( tmpEnt->getName().compare(listEnt[i]) == 0 )
                {
                  entOnList = true;
                  break;
                }
              }
            }

            if ((pentity->isVisible() || !excludeHidden) && ((entOnList && !blackList) || (!entOnList && blackList) ) )
            {
            	if (pentity->getMovableType().compare("Entity") == 0)
            	{
            		// get the mesh information
            		getMeshInformation(pentity, vertex_count, vertices, index_count, indices,
            			pentity->getParentNode()->_getDerivedPosition(),
            			pentity->getParentNode()->_getDerivedOrientation(),
            			pentity->getParentNode()->_getDerivedScale());
            	}
            	else
            	{
            		// TODO other case
            		std::cerr << " MovableObject of Unknown Type (" << pentity->getMovableType()
            				<< " on " << pentity->getName() << ": abort !" << std::endl;
//            		return shared_ptr<VEHA::Entity>();
            		return;
            	}

            	// test for hitting individual triangles on the mesh
            	bool new_closest_found = false;
            	for (int i = 0; i < static_cast<int>(index_count); i += 3)
            	{
            		// check for a hit against this triangle
            		std::pair<bool, Ogre::Real> hit = Ogre::Math::intersects(mouseRay, vertices[indices[i]],
            				vertices[indices[i+1]], vertices[indices[i+2]], true, false);

            		// if it was a hit check if its the closest
            		if (hit.first)
            		{
            			if ((closest_distance < 0.0f) ||
            					(hit.second < closest_distance))
            			{
            				// this is the closest so far, save it off
            				closest_distance = hit.second;
            				new_closest_found = true;
            			}
            		}
            	}

            	// free the verticies and indicies memory
            	delete[] vertices;
            	delete[] indices;

            	// if we found a new closest raycast for this object, update the
            	// closest_result before moving on to the next object.
            	if (new_closest_found)
            	{
            		closest_result = mouseRay.getPoint(closest_distance);
//            		shared_ptr<OgreTrihedron> trihedron = make_shared<OgreTrihedron>("schlafeu",
//            				VEHA::Vector3(closest_result.x, closest_result.y, closest_result.z));
            		pos = VEHA::Vector3(closest_result.x, closest_result.y, closest_result.z);
            		entity = tmpEnt;
            	}
            }
        }
    }

//    cerr << " !!!!! " << entity << endl;

	if(entity)
	{
		distOut=closest_distance;
//		cerr << entity->getName() << endl;
	}
	else
	{
		distOut=-1;
	}
//	return entity;
}

bool OgreCamera::frameRenderingQueued(const FrameEvent& evt)
{
    if(_window->isClosed())
        return false;

    //Need to capture/update each device
    _window->_keyboard->capture();
    _window->_mouse->capture();

    if(_window->_keyboard->isKeyDown(OIS::KC_ESCAPE))
        return false;
	return true;
}
bool OgreCamera::frameStarted(const FrameEvent & evt)
{
	return !_window->isClosed();
}
bool OgreCamera::frameEnded(const FrameEvent& evt)
{
	MascaretApplication::getInstance()->step();
	return true;
}
void OgreCamera::getMeshInformation(Ogre::Entity *entity,
									size_t &vertex_count,
									Ogre::Vector3* &vertices,
									size_t &index_count,
									unsigned long* &indices,
									const Ogre::Vector3 &position,
									const Ogre::Quaternion &orient,
									const Ogre::Vector3 &scale) const
{
    bool added_shared = false;
    size_t current_offset = 0;
    size_t shared_offset = 0;
    size_t next_offset = 0;
    size_t index_offset = 0;
    vertex_count = index_count = 0;
 
   Ogre::MeshPtr mesh = entity->getMesh();
 
 
   bool useSoftwareBlendingVertices = entity->hasSkeleton();
 
   if (useSoftwareBlendingVertices)
   {
      entity->_updateAnimation();
   }
 
    // Calculate how many vertices and indices we're going to need
    for (unsigned short i = 0; i < mesh->getNumSubMeshes(); ++i)
    {
        Ogre::SubMesh* submesh = mesh->getSubMesh( i );
 
        // We only need to add the shared vertices once
        if(submesh->useSharedVertices)
        {
            if( !added_shared )
            {
                vertex_count += mesh->sharedVertexData->vertexCount;
                added_shared = true;
            }
        }
        else
        {
            vertex_count += submesh->vertexData->vertexCount;
        }
 
        // Add the indices
        index_count += submesh->indexData->indexCount;
    }
 
 
    // Allocate space for the vertices and indices
    vertices = new Ogre::Vector3[vertex_count];
    indices = new unsigned long[index_count];
 
    added_shared = false;
 
    // Run through the submeshes again, adding the data into the arrays
    for ( unsigned short i = 0; i < mesh->getNumSubMeshes(); ++i)
    {
        Ogre::SubMesh* submesh = mesh->getSubMesh(i);
 
      //----------------------------------------------------------------
      // GET VERTEXDATA
      //----------------------------------------------------------------
 
        //Ogre::VertexData* vertex_data = submesh->useSharedVertices ? mesh->sharedVertexData : submesh->vertexData;
      Ogre::VertexData* vertex_data;
 
      //When there is animation:
      if(useSoftwareBlendingVertices)
#ifdef BUILD_AGAINST_AZATHOTH
         vertex_data = submesh->useSharedVertices ? entity->_getSharedBlendedVertexData() : entity->getSubEntity(i)->_getBlendedVertexData();
#else
         vertex_data = submesh->useSharedVertices ? entity->_getSkelAnimVertexData() : entity->getSubEntity(i)->_getSkelAnimVertexData();
#endif
      else
         vertex_data = submesh->useSharedVertices ? mesh->sharedVertexData : submesh->vertexData;
 
 
        if((!submesh->useSharedVertices)||(submesh->useSharedVertices && !added_shared))
        {
            if(submesh->useSharedVertices)
            {
                added_shared = true;
                shared_offset = current_offset;
            }
 
            const Ogre::VertexElement* posElem =
                vertex_data->vertexDeclaration->findElementBySemantic(Ogre::VES_POSITION);
 
            Ogre::HardwareVertexBufferSharedPtr vbuf =
                vertex_data->vertexBufferBinding->getBuffer(posElem->getSource());
 
            unsigned char* vertex =
                static_cast<unsigned char*>(vbuf->lock(Ogre::HardwareBuffer::HBL_READ_ONLY));
 
            // There is _no_ baseVertexPointerToElement() which takes an Ogre::Real or a double
            //  as second argument. So make it float, to avoid trouble when Ogre::Real will
            //  be comiled/typedefed as double:
            //      Ogre::Real* pReal;
            float* pReal;
 
            for( size_t j = 0; j < vertex_data->vertexCount; ++j, vertex += vbuf->getVertexSize())
            {
                posElem->baseVertexPointerToElement(vertex, &pReal);
 
                Ogre::Vector3 pt(pReal[0], pReal[1], pReal[2]);
 
                vertices[current_offset + j] = (orient * (pt * scale)) + position;
            }
 
            vbuf->unlock();
            next_offset += vertex_data->vertexCount;
        }
 
 
        Ogre::IndexData* index_data = submesh->indexData;
        size_t numTris = index_data->indexCount / 3;
        Ogre::HardwareIndexBufferSharedPtr ibuf = index_data->indexBuffer;
 
        bool use32bitindexes = (ibuf->getType() == Ogre::HardwareIndexBuffer::IT_32BIT);
 
        unsigned long*  pLong = static_cast<unsigned long*>(ibuf->lock(Ogre::HardwareBuffer::HBL_READ_ONLY));
        unsigned short* pShort = reinterpret_cast<unsigned short*>(pLong);
 
 
        size_t offset = (submesh->useSharedVertices)? shared_offset : current_offset;
        size_t index_start = index_data->indexStart;
        size_t last_index = numTris*3 + index_start;
 
        if (use32bitindexes)
            for (size_t k = index_start; k < last_index; ++k)
            {
                indices[index_offset++] = pLong[k] + static_cast<unsigned long>( offset );
            }
 
        else
            for (size_t k = index_start; k < last_index; ++k)
            {
                indices[ index_offset++ ] = static_cast<unsigned long>( pShort[k] ) +
                    static_cast<unsigned long>( offset );
            }
 
        ibuf->unlock();
        current_offset = next_offset;
    }
}
}
