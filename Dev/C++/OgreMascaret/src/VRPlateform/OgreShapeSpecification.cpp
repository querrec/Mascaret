/*
 * OgreShapeSpecification.cpp
 *
 *  Created on: 25 févr. 2011
 *      Author: soler
 */

#include "Tools/filesystem.h"

#include "VRPlateform/OgreShapeSpecification.h"
#include "VRPlateform/OgrePointSpecification.h"
#include "VRPlateform/OgreMascaretApplication.h"

#include <sstream>
using namespace std;

#include <boost/algorithm/string.hpp>
using Ogre::Real;

DECLARE_NAMESPACE_FILESYSTEM;

namespace OgreMascaret
{
int OgreShapeSpecification::_id = 0;
std::map<Ogre::Entity*,shared_ptr<VEHA::Entity> > OgreShapeSpecification::_entitiesMap;


OgreShapeSpecification::OgreShapeSpecification(const string& name) //protected
: ShapeSpecification(name)
{
}

OgreShapeSpecification::OgreShapeSpecification(const string& name, const string & url, bool movable,bool recursive,const string& shader)
: ShapeSpecification(name,url,movable,recursive,shader)
{
	fs::path path1(url);
	string extension=fs::extension(path1);
	boost::to_lower(extension);

    if(extension == ".dae")
    {
    	//TODO
    	throw std::runtime_error("loader colada non implementer");
    }
    else if(extension == ".wrl")
    {
    	//TODO
    	throw std::runtime_error("loader VRML non implementer");
    }
    else
    {
       ostringstream oss;
       oss << OgreShapeSpecification::_id++;
       string ogreName = name+oss.str();
       
       #if BOOST_FILESYSTEM_VERSION == 2
       _entityOgre = OgreMascaretApplication::getInstance()->getSceneManager()->createEntity(ogreName,path1.filename());
       #else
       _entityOgre = OgreMascaretApplication::getInstance()->getSceneManager()->createEntity(ogreName,path1.filename().string());
       #endif
    }

}
OgreShapeSpecification::OgreShapeSpecification(const string& name, const vector<double>& distances,const vector<string> & urls, bool movable,bool recursive,const string& shader)
: ShapeSpecification(name,distances,urls,movable,recursive,shader)
{
}

OgreShapeSpecification::~OgreShapeSpecification()
{
}

void OgreShapeSpecification::setVisibility(bool vis)
{
	_entityOgre->setVisible(vis);
}
bool OgreShapeSpecification::getVisibility() const
{
	return _entityOgre->getVisible();
}

void OgreShapeSpecification::setScale(const Vector3& vec)
{
	shared_dynamic_cast<OgrePointSpecification>(getEntity()->getReferentialPoint())->getNode()->setScale(Real(vec.x),Real(vec.y),Real(vec.z));
}
Vector3 OgreShapeSpecification::getScale() const
{
	Ogre::Vector3 vec=shared_dynamic_cast<OgrePointSpecification>(getEntity()->getReferentialPoint())->getNode()->getScale();
	return Vector3(vec.x,vec.y,vec.z);
}
void OgreShapeSpecification::setEntity(shared_ptr<Entity> entity)
{
	_entitiesMap[_entityOgre]=entity;
	// cerr << "Entity : " << entity->getName() << endl;
	shared_dynamic_cast<OgrePointSpecification>(entity->getReferentialPoint())->getNode()->attachObject(_entityOgre);
}
shared_ptr<Entity> OgreShapeSpecification::getEntity() const
{
	return _entitiesMap[_entityOgre];
}
shared_ptr<VEHA::Entity> OgreShapeSpecification::getEntityFromObject3D(Ogre::Entity* obj)
{
	return _entitiesMap[obj];
}

void OgreShapeSpecification::setColor(shared_ptr <Color> color)
{
	cout<<"SetColor function in ogreMascaret"<<endl;
}

shared_ptr<Color> OgreShapeSpecification::getColor()
{
	cout<<"GetColor function in ogreMascaret"<<endl;
	return shared_ptr<Color>();
}

void OgreShapeSpecification::setTransparent(shared_ptr <Color> color)
{
	cout<<"SetTransparent function in ogreMascaret"<<endl;
}

void OgreShapeSpecification::growUp(int percent)
{
	cout<<"GrowUp function in ogreMascaret"<<endl;
}

bool OgreShapeSpecification::restaureColor(shared_ptr <Color> color)
{
	cout<<"restaureColor function in ogreMascaret"<<endl;
	return false;
}

bool OgreShapeSpecification::restaureShape()
{
	cout<<"restaureShape function in ogreMascaret"<<endl;
	return false;
}
}
