/*
 * OgreShapeSpecification.h
 *
 *      Author: soler
 */

#ifndef OGRESHAPESPECIFICATION_H_
#define OGRESHAPESPECIFICATION_H_
#include "ogreMascaret_plateform.h"
#include "VEHA/Entity/ShapeSpecification.h"
#include "VEHA/Entity/Vector3.h"
#include <Ogre.h>

namespace OgreMascaret
{
using VEHA::Color;
using VEHA::Vector3;

class OGRE_MASCARET_API OgreShapeSpecification : public VEHA::ShapeSpecification
{
public:
	OgreShapeSpecification(const string& name, const string & url, bool movable=true,bool recursive =false,const string& shader="");
	OgreShapeSpecification(const string& name, const vector<double>& distances,const vector<string> & urls, bool movable=true,bool recursive = false,const string& shader="");
	virtual ~OgreShapeSpecification();

	virtual void setVisibility(bool v);
	virtual bool getVisibility() const;

	virtual void setScale(const Vector3& scale);
	virtual Vector3 getScale() const;

	virtual void setEntity(shared_ptr<VEHA::Entity> entity);
	virtual shared_ptr<VEHA::Entity> getEntity() const;

	inline Ogre::Entity* getObject3D();

	virtual void setColor(shared_ptr <Color> color);
	virtual shared_ptr<Color> getColor();
	virtual void setTransparent(shared_ptr <Color> color);
//	virtual void blinkBackground(double time);
	virtual void growUp(int percent);
	virtual bool restaureColor(shared_ptr <Color> color);
	virtual bool restaureShape();

	static shared_ptr<VEHA::Entity> getEntityFromObject3D(Ogre::Entity* obj);
protected:
	OgreShapeSpecification(const string& name);
	Ogre::Entity* _entityOgre;
	static std::map<Ogre::Entity*,shared_ptr<VEHA::Entity> > _entitiesMap;
        static int _id;
};
Ogre::Entity* OgreShapeSpecification::getObject3D()
{
	return _entityOgre;
}
}
#endif /* OGRESHAPESPECIFICATION_H_ */
