#ifndef _v2_VEHA_Entity_Entity_H
#define _v2_VEHA_Entity_Entity_H

#include "Tools/veha_plateform.h"
#include "VEHA/Kernel/InstanceSpecification.h"


namespace VEHA
{

class Vector3;
class RotationVector;
class EntityClass;
class ShapeSpecification;
class AnimationSpecification;
class SoundSpecification;
class TopologicalSpecification;
class PointSpecification;
class PathSpecification;

enum Axe{X,Y,Z,minusX,minusY,minusZ};
enum Plane{XY,YX,YZ,ZY,XZ,ZX};

class VEHA_API Entity : public InstanceSpecification, public boost::noncopyable

{
public :
	Entity(const string& name,shared_ptr<EntityClass> cl);
	virtual ~Entity();

	void setVisibility(bool v);
	bool getVisibility();

	inline shared_ptr<ShapeSpecification> getActiveShape(void) {return _activeShape;}
	void setActiveShape(const string& name);

	inline shared_ptr<Entity> getParent(void) { return _parent;}
	void setParent(shared_ptr<Entity> e);

	inline vector< shared_ptr<Entity> > getChildren(void) { return _children;}
	inline shared_ptr<Entity> getChild(int index) { return _children[index];}
	shared_ptr<Entity> getChild(string name);
	inline void addChild(shared_ptr<Entity> value) { value->setParent(shared_dynamic_cast<Entity>(this->shared_from_this()));}
	bool hasChild(shared_ptr<Entity> entity);

	void setLocalPosition(const Vector3& pos);
	Vector3 getLocalPosition();

	void setGlobalPosition(const Vector3& pos);
	Vector3 getGlobalPosition();

	void translate(const Vector3& pos);

	void setLocalRotation(const RotationVector& orientation);
	RotationVector getLocalRotation();

	void setGlobalRotation(const RotationVector& orientation);
	RotationVector getGlobalRotation();

	void rotate(const RotationVector& orientation);
	void rotate(const Vector3& orientation);

	void setLocalOrientation(const Vector3& pos);
	Vector3 getLocalOrientation();

	void setGlobalOrientation(const Vector3& pos);
	Vector3 getGlobalOrientation();

	vector<shared_ptr<ShapeSpecification> > getShapes();
	shared_ptr<ShapeSpecification> getShape(const string& name);

	vector<shared_ptr<AnimationSpecification> > getAnimations();
	shared_ptr<AnimationSpecification> getAnimation(const string& name);

	vector<shared_ptr<SoundSpecification> > getSounds();
	shared_ptr<SoundSpecification> getSound(const string& name);

	vector<shared_ptr<TopologicalSpecification> > getTopologicals();
	shared_ptr<TopologicalSpecification> getTopological(const string& name);

	vector<shared_ptr<PointSpecification> > getPoints();
	shared_ptr<PointSpecification> getPoint(const string& name);

	vector<shared_ptr<PathSpecification> > getPaths();
	shared_ptr<PathSpecification> getPath(const string& name);

	bool playAnimation(const string& animationName, int sens=1, double animationSpeed=1.0, bool cycle=true);
	bool playSound(const string& name, double speed, bool cycle);

	bool stopAnimation(const string& animationName);
	bool stopAllAnimations();
	bool stopSound(const string& animationName);

	inline shared_ptr<PointSpecification> getReferentialPoint(void) { return _referentialPoint;}
	inline map<string,shared_ptr<PointSpecification> > getViewPoints() {return _viewPoints;}
	shared_ptr<PointSpecification> getViewPoint(const string& name);
	inline void addViewPoint(const string& name,shared_ptr<PointSpecification> viewPoint) {_viewPoints[name]=viewPoint;}
	inline void removeViewPoint(const string& name) {_viewPoints.erase(name);}

	void addCallbackOnChangePosition( void (*f)(shared_ptr<Entity>) );
	void removeCallbackOnChangePosition( void (*f)(shared_ptr<Entity>) );

protected:
	inline void setReferentialPoint(shared_ptr<PointSpecification> referentialPoint) { _referentialPoint = referentialPoint;}
	inline void _addChildren(shared_ptr<Entity> value) {
		int i=_hasChildren(value);
		if (i<0) _children.push_back(value);
	}
	inline void _removeChildren(shared_ptr<Entity> value){
		int i=_hasChildren(value);
		if (i>=0) { swap(_children[i],*(--_children.end())); _children.pop_back();}
	}
	inline int _hasChildren(shared_ptr<Entity> value)
	{
		for(size_t i=_children.size();i--;)
			if (_children[i]==value) return i;
		return -1;
	}
/*
	inline void fireChangePosition()
	{
		_cbChangePosition();
	}
*/
	inline void fireClickOn()
	{
		_cbClickOn();
	}

	shared_ptr<ShapeSpecification> _activeShape;
	shared_ptr<PointSpecification>   _referentialPoint;
	map<string,shared_ptr<PointSpecification> > _viewPoints;
	shared_ptr<Entity>  _parent;
	vector< shared_ptr<Entity> >    _children;
	boost::signal0<void>	 _cbChangePosition,_cbClickOn;  // TODO: faire les addcallback
	vector < void (*)(shared_ptr<Entity>) > _CallbackOnChangePosition;
	void fireChangePosition(void);
};

}
#endif
