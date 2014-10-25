/*
 * AReViShapeSpecification.h
 *
 *  Created on: 24 févr. 2011
 *      Author: soler
 */

#ifndef AREVISHAPESPECIFICATION_H_
#define AREVISHAPESPECIFICATION_H_
#include "VRPlateform/AReViEntityObject.h"
#include "areviMascaret_plateform.h"
#include "VEHA/Entity/ShapeSpecification.h"
#include "VEHA/Entity/Vector3.h"


#include "AReVi/Lib3D/material3D.h"
#include "AReVi/Shapes/shape3D.h"
#include "AReVi/Shapes/surface3D.h"
#include "AReVi/Lib3D/object3D.h"
#include "AReVi/Lib3D/transform3D.h"
#include "AReVi/Lib3D/util3D_priv.h"

using AReVi::ArRef;
using AReVi::Activity;
using AReVi::ArObject;
using AReVi::Material3D;
using AReVi::Object3D;
using AReVi::Transform3D;
using AReVi::StlString;

using namespace VEHA;
using namespace AReVi;

namespace AReViMascaret
{

using boost::shared_ptr;
using std::string;
using VEHA::ShapeSpecification;
using VEHA::AnimationSpecification;
using VEHA::Color;


class AREVI_MASCARET_API AReViShapeSpecification : public VEHA::ShapeSpecification
{
public:
	AReViShapeSpecification(const string& name, const string & url, bool movable=true,bool recursive =false,const string& shader="");
	AReViShapeSpecification(const string& name, const vector<double>& distances,const vector<string> & urls, bool movable=true,bool recursive = false,const string& shader="");
	virtual ~AReViShapeSpecification();

	virtual void setVisibility(bool v);
	virtual bool getVisibility() const;

	virtual void setScale(const Vector3& scale);
	virtual Vector3 getScale() const;

	virtual void setEntity(shared_ptr<Entity> entity);
	virtual shared_ptr<Entity> getEntity() const;

	inline AReVi::ArRef<AReViEntityObject> getObject3D();
	inline void setObject3D(AReVi::ArRef<AReViEntityObject> object);

	/* Shape Modifier Operation*/
	virtual void setColor(shared_ptr<Color> color);
	virtual shared_ptr<Color> getColor();
	virtual void setTransparent(shared_ptr<Color> color);
	virtual void growUp(int percent);
	virtual bool restaureColor(shared_ptr<Color> color);
	virtual bool restaureShape();

protected:
	friend class AReViScene;
	AReVi::ArRef<AReViEntityObject> _obj;
	/* --- Shape Modifier attributes ---*/
	ArRef<Object3D> _object3D;
	double br,bg,bb;
	StlMap<StlString,ArRef<Transform3D> > _transformMap;
	StlMap<StlString,ArRef<Material3D> > _surfaceMaterialMap;

};
AReVi::ArRef<AReViEntityObject> AReViShapeSpecification::getObject3D()
{
	return _obj;
}
void AReViShapeSpecification::setObject3D(AReVi::ArRef<AReViEntityObject> object)
{
	_obj=object;
}
}
#endif /* AREVISHAPESPECIFICATION_H_ */
