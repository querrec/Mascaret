#ifndef _v2_VEHA_Entity_EntityClass_H
#define _v2_VEHA_Entity_EntityClass_H

#include "Tools/veha_plateform.h"
#include "VEHA/Kernel/Class.h"
#include <boost/signals.hpp>

namespace VEHA
{

class Entity;
class ShapeProperty;
class TopologicalProperty;
class GeometryProperty;

class VEHA_API EntityClass : public Class
{
public :
	EntityClass(const string& name);
	virtual ~EntityClass();
	// --- Attributes ---
	// --- Relations ---
	//------ shape -----
	//callbacks for class:
	//		typedef function<void , const shared_ptr<Kernel::InstanceSpecification> & > cbEntityCreate;
protected :
	vector< shared_ptr<ShapeProperty> > _shape;
public :
	inline vector< shared_ptr<ShapeProperty> > getShape(void) { return _shape;}
	inline shared_ptr<ShapeProperty> getShape_at(int index) { return _shape[index];}
	inline void addShape(shared_ptr<ShapeProperty> value) { _shape.push_back(value);}
	/*
//------ undefined -----
protected :
    vector< TopologicalProperty * >	_undefined;
public :
    inline vector< TopologicalProperty * > getUndefined(void) { return _undefined;}
    inline TopologicalProperty * getUndefined_at(int index) { return _undefined[index];}
    inline void addUndefined(TopologicalProperty * value) { _undefined.push_back(value);}
	 */
	//------ referentialPoint -----
protected :
	shared_ptr<TopologicalProperty> _referentialPoint;
public :
	inline shared_ptr<TopologicalProperty> getReferentialPoint(void) { return _referentialPoint;}
	inline void setReferentialPoint(shared_ptr<TopologicalProperty> referentialPoint) { _referentialPoint = referentialPoint;}
	//------ geometry -----
protected :
	shared_ptr<GeometryProperty>    _geometry;
public :
	inline shared_ptr<GeometryProperty> getGeometry(void) { return _geometry;}
	inline void setGeometry(shared_ptr<GeometryProperty> geometry) { _geometry = geometry;}
	// --- Operations ---
};

}
#endif
