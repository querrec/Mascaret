#include "VEHA/Entity/Entity.h"
#include "VEHA/Entity/ShapeSpecification.h"
#include "VEHA/Entity/AnimationSpecification.h"
#include "VEHA/Entity/SoundSpecification.h"
#include "VEHA/Entity/PointSpecification.h"
#include "VEHA/Entity/PathSpecification.h"
#include "VEHA/Entity/RotationVector.h"
#include "VEHA/Entity/Vector3.h"
#include "VEHA/Entity/EntityClass.h"
#include "VEHA/Entity/VirtualRealityComponentFactory.h"
#include "VEHA/Kernel/CallbackManager.h"
#include "MascaretApplication.h"

namespace VEHA
{

Entity::Entity(const string& name, shared_ptr<EntityClass> cl) :
    InstanceSpecification(name, cl),
    _activeShape(),
    _referentialPoint(MascaretApplication::getInstance()->getVRComponentFactory()->createPoint()),
    _parent()
{
}

Entity::~Entity()
{
}

bool Entity::hasChild(shared_ptr<Entity> entity)
{
	bool hasChild = false;
	for (unsigned int i = 0; i < _children.size(); i++)
	{
		if (entity == _children[i]) 
		{
			hasChild = true;
			break;
		}
	}
	if (hasChild) return hasChild;
	else
	{
		int i = 0;
		while (i < _children.size() && !hasChild)
			hasChild = _children[i++]->hasChild(entity); 
	}
	return hasChild;
}

shared_ptr<Entity> Entity::getChild(string name)
{
  for (size_t i = _children.size(); i--; )
    if(_children[i]->getName() == name)
      return _children[i];

  return shared_ptr<Entity>();
}

void Entity::setActiveShape(const string& name)
{
  if (getClassifier()->hasAttribute(name)) // TODO: copie des proprietes dans la nouvelle shape
  {
    shared_ptr<Slot> s = getProperty(name);
    _activeShape = shared_dynamic_cast<ShapeSpecification>(s->getValue());
    if (_activeShape)
      _activeShape->setEntity(shared_dynamic_cast<Entity>(shared_from_this()));
    else
      cerr << getName() << "->setActiveShape(" << name << ") : the property is undefined" << endl;
  }
}

void Entity::setLocalPosition(const Vector3& pos)
{
  CallbackManager_changesMethod::addChangeMethodSignal(&_cbChangePosition); //TODO pb si le slot disparait
  _referentialPoint->setLocalPosition(pos);
  fireChangePosition();
}

void Entity::translate(const Vector3& pos)
{
  CallbackManager_changesMethod::addChangeMethodSignal(&_cbChangePosition); //TODO pb si le slot disparait
  _referentialPoint->translate(pos);
  fireChangePosition();
}

Vector3 Entity::getLocalPosition()
{
  return _referentialPoint->getLocalPosition();
}

void Entity::setGlobalPosition(const Vector3& pos)
{
  CallbackManager_changesMethod::addChangeMethodSignal(&_cbChangePosition); //TODO pb si le slot disparait
  _referentialPoint->setGlobalPosition(pos);
  fireChangePosition();
}
Vector3 Entity::getGlobalPosition()
{
  return _referentialPoint->getGlobalPosition();
}
void Entity::setLocalRotation(const RotationVector& rot)
{
  _referentialPoint->setLocalRotation(rot);
  fireChangePosition();
}

RotationVector Entity::getLocalRotation()
{
  CallbackManager_changesMethod::addChangeMethodSignal(&_cbChangePosition); //TODO pb si le slot disparait
  return _referentialPoint->getLocalRotation();
}
void Entity::setGlobalRotation(const RotationVector& rot)
{
  _referentialPoint->setGlobalRotation(rot);
  fireChangePosition();
}
RotationVector Entity::getGlobalRotation()
{
  CallbackManager_changesMethod::addChangeMethodSignal(&_cbChangePosition); //TODO pb si le slot disparait
  return _referentialPoint->getGlobalRotation();
}

void Entity::rotate(const RotationVector& ori)
{
  CallbackManager_changesMethod::addChangeMethodSignal(&_cbChangePosition); //TODO pb si le slot disparait
  _referentialPoint->rotate(ori);
  fireChangePosition();
}

void Entity::rotate(const Vector3& ori)
{
  CallbackManager_changesMethod::addChangeMethodSignal(&_cbChangePosition); //TODO pb si le slot disparait
  _referentialPoint->rotate(ori);
  fireChangePosition();
}

void Entity::setLocalOrientation(const Vector3& ori)
{
  Vector3 ori2 = ori;
  _referentialPoint->setLocalOrientation(ori2);
  fireChangePosition();
}

Vector3 Entity::getLocalOrientation()
{
  CallbackManager_changesMethod::addChangeMethodSignal(&_cbChangePosition); //TODO pb si le slot disparait
  return _referentialPoint->getLocalOrientation();
}
void Entity::setGlobalOrientation(const Vector3& ori)
{
  Vector3 ori2 = ori;
  _referentialPoint->setGlobalOrientation(ori2);
  fireChangePosition();
}
Vector3 Entity::getGlobalOrientation()
{
  CallbackManager_changesMethod::addChangeMethodSignal(&_cbChangePosition); //TODO pb si le slot disparait
  return _referentialPoint->getGlobalOrientation();
}
void Entity::setVisibility(bool visible)
{
  getActiveShape()->setVisibility(visible);
}

bool Entity::getVisibility()
{
  return getActiveShape()->getVisibility();
}
vector<shared_ptr<ShapeSpecification> > Entity::getShapes()
{
  vector<shared_ptr<ShapeSpecification> > vect;
  const map<string, shared_ptr<Slot> >& slots = getSlots();
  for (map<string, shared_ptr<Slot> >::const_iterator i = slots.begin(); i != slots.end(); i++)
  {
    const map<string, shared_ptr<ValueSpecification> >& values = i->second->getValues();
    if (values.size() == 1)
    {
      for (map<string, shared_ptr<ValueSpecification> >::const_iterator i = values.begin();
          i != values.end(); i++)
      {
        shared_ptr<ShapeSpecification> sh = shared_dynamic_cast<ShapeSpecification>(i->second);
        if (sh) vect.push_back(sh);
      }
    }
  }
  return vect;
}

shared_ptr<ShapeSpecification> Entity::getShape(const string& name)
{
  if (getClassifier()->hasAttribute(name))
  {
    shared_ptr<Slot> prop = getProperty(name);
    if (prop) return shared_dynamic_cast<ShapeSpecification>(prop->getValue());
  }
  return shared_ptr<ShapeSpecification>();
}

vector<shared_ptr<AnimationSpecification> > Entity::getAnimations()
{
  vector<shared_ptr<AnimationSpecification> > vect;
  const map<string, shared_ptr<Slot> >& slots = getSlots();
  for (map<string, shared_ptr<Slot> >::const_iterator i = slots.begin(); i != slots.end(); i++)
  {
    const map<string, shared_ptr<ValueSpecification> >& values = i->second->getValues();
    if (values.size() == 1)
    {
      for (map<string, shared_ptr<ValueSpecification> >::const_iterator i = values.begin();
          i != values.end(); i++)
      {
        shared_ptr<AnimationSpecification> sh = shared_dynamic_cast<AnimationSpecification>(
            i->second);
        if (sh) vect.push_back(sh);
      }
    }
  }
  return vect;
}
shared_ptr<AnimationSpecification> Entity::getAnimation(const string& name)
{
  if (getClassifier()->hasAttribute(name))
  {
    shared_ptr<Slot> prop = getProperty(name);
    if (prop) return shared_dynamic_cast<AnimationSpecification>(prop->getValue());
  }
  return shared_ptr<AnimationSpecification>();
}

vector<shared_ptr<SoundSpecification> > Entity::getSounds()
{
  vector<shared_ptr<SoundSpecification> > vect;
  const map<string, shared_ptr<Slot> >& slots = getSlots();
  for (map<string, shared_ptr<Slot> >::const_iterator i = slots.begin(); i != slots.end(); i++)
  {
    const map<string, shared_ptr<ValueSpecification> >& values = i->second->getValues();
    if (values.size() == 1)
    {
      for (map<string, shared_ptr<ValueSpecification> >::const_iterator i = values.begin();
          i != values.end(); i++)
      {
        shared_ptr<SoundSpecification> sh = shared_dynamic_cast<SoundSpecification>(i->second);
        if (sh) vect.push_back(sh);
      }
    }
  }
  return vect;
}
shared_ptr<SoundSpecification> Entity::getSound(const string& name)
{
  if (getClassifier()->hasAttribute(name))
  {
    shared_ptr<Slot> prop = getProperty(name);
    if (prop) return shared_dynamic_cast<SoundSpecification>(prop->getValue());
  }
  return shared_ptr<SoundSpecification>();
}

vector<shared_ptr<TopologicalSpecification> > Entity::getTopologicals()
{
  vector<shared_ptr<TopologicalSpecification> > vect;
  const map<string, shared_ptr<Slot> >& slots = getSlots();
  for (map<string, shared_ptr<Slot> >::const_iterator i = slots.begin(); i != slots.end(); i++)
  {
    const map<string, shared_ptr<ValueSpecification> >& values = i->second->getValues();
    if (values.size() == 1)
    {
      for (map<string, shared_ptr<ValueSpecification> >::const_iterator i = values.begin();
          i != values.end(); i++)
      {
        shared_ptr<TopologicalSpecification> sh = shared_dynamic_cast<TopologicalSpecification>(
            i->second);
        if (sh) vect.push_back(sh);
      }
    }
  }
  return vect;
}
shared_ptr<TopologicalSpecification> Entity::getTopological(const string& name)
{
  if (getClassifier()->hasAttribute(name))
  {
    shared_ptr<Slot> prop = getProperty(name);
    if (prop) return shared_dynamic_cast<TopologicalSpecification>(prop->getValue());
  }
  return shared_ptr<TopologicalSpecification>();
}

vector<shared_ptr<PointSpecification> > Entity::getPoints()
{
  vector<shared_ptr<PointSpecification> > vect;
  const map<string, shared_ptr<Slot> >& slots = getSlots();
  for (map<string, shared_ptr<Slot> >::const_iterator i = slots.begin(); i != slots.end(); i++)
  {
    const map<string, shared_ptr<ValueSpecification> >& values = i->second->getValues();
    if (values.size() == 1)
    {
      for (map<string, shared_ptr<ValueSpecification> >::const_iterator i = values.begin();
          i != values.end(); i++)
      {
        shared_ptr<PointSpecification> sh = shared_dynamic_cast<PointSpecification>(i->second);
        if (sh) vect.push_back(sh);
      }
    }
  }
  return vect;
}
shared_ptr<PointSpecification> Entity::getPoint(const string& name)
{
  if (getClassifier()->hasAttribute(name))
  {
    shared_ptr<Slot> prop = getProperty(name);
    if (prop) return shared_dynamic_cast<PointSpecification>(prop->getValue());
  }
  return shared_ptr<PointSpecification>();
}

vector<shared_ptr<PathSpecification> > Entity::getPaths()
{
  vector<shared_ptr<PathSpecification> > vect;
  const map<string, shared_ptr<Slot> >& slots = getSlots();
  for (map<string, shared_ptr<Slot> >::const_iterator i = slots.begin(); i != slots.end(); i++)
  {
    const map<string, shared_ptr<ValueSpecification> >& values = i->second->getValues();
    if (values.size() == 1)
    {
      for (map<string, shared_ptr<ValueSpecification> >::const_iterator i = values.begin();
          i != values.end(); i++)
      {
        shared_ptr<PathSpecification> sh = shared_dynamic_cast<PathSpecification>(i->second);
        if (sh) vect.push_back(sh);
      }
    }
  }
  return vect;
}
shared_ptr<PathSpecification> Entity::getPath(const string& name)
{
  if (getClassifier()->hasAttribute(name))
  {
    shared_ptr<Slot> prop = getProperty(name);
    if (prop) return shared_dynamic_cast<PathSpecification>(prop->getValue());
  }
  return shared_ptr<PathSpecification>();
}

bool Entity::playAnimation(const string& animationName, int sens, double animationSpeed, bool cycle)
{
  shared_ptr<AnimationSpecification> anim = getAnimation(animationName);
  if (!anim) return false;
  bool result = true;
  anim->setShape(getActiveShape());
  result = anim->play(animationSpeed, sens, cycle);
  for (size_t i = 0; result && i < _children.size(); i++)
    result = _children[i]->playAnimation(animationName, sens, animationSpeed, cycle);
  return result;
}
bool Entity::stopAnimation(const string& animationName)
{
  shared_ptr<AnimationSpecification> anim = getAnimation(animationName);
  if (anim) return anim->stop();
  return false;
}
bool Entity::stopAllAnimations()
{
  vector<shared_ptr<AnimationSpecification> > anims = getAnimations();
  for (size_t i = 0; i < anims.size(); i++)
  {
    anims[i]->stop();
  }
  return true;
}
bool Entity::playSound(const string& name, double speed, bool cycle)
{
  shared_ptr<SoundSpecification> sound = getSound(name);
  if (!sound) return false;
  bool result = true;
  if (getActiveShape()) sound->attachTo(_referentialPoint);
  result = sound->play(speed, cycle);
  return result;
}

bool Entity::stopSound(const string& soundName)
{
  shared_ptr<SoundSpecification> sound = getSound(soundName);
  if (sound) return sound->stop();
  return false;
}
shared_ptr<PointSpecification> Entity::getViewPoint(const string& name)
{
  map<string, shared_ptr<PointSpecification> >::iterator it = _viewPoints.find(name);
  cerr << "[Entity Info] " << getName() << " has " << _viewPoints.size() << " viewpoints" << endl;
  if (it != _viewPoints.end()) return it->second;
  for (size_t i = 0; i < _children.size(); i++)
  {
    shared_ptr<PointSpecification> vp = _children[i]->getViewPoint(name);
    if (vp) return vp;
  }
  return shared_ptr<PointSpecification>();
}
void Entity::setParent(shared_ptr<Entity> e)
{
  if (_parent)
  {
    _parent->_removeChildren(shared_dynamic_cast<Entity>(this->shared_from_this()));

  }
  _referentialPoint->setParent(shared_ptr<PointSpecification>());
  _parent = e;
  if (_parent)
  {
    _referentialPoint->setParent(_parent->_referentialPoint);
    _parent->_addChildren(shared_dynamic_cast<Entity>(this->shared_from_this()));

  }
}

void Entity::addCallbackOnChangePosition( void (*f)(shared_ptr<Entity>) )
{
	_CallbackOnChangePosition.push_back(f);
}

void Entity::removeCallbackOnChangePosition( void (*f)(shared_ptr<Entity>) )
{
	unsigned int i;
	
	for (i = 0; i < _CallbackOnChangePosition.size(); i ++)
	{
		if (_CallbackOnChangePosition[i] == f) break;	
	}

	if (i < _CallbackOnChangePosition.size())
		_CallbackOnChangePosition.erase(_CallbackOnChangePosition.begin() + i);

}

void Entity::fireChangePosition(void)
{
	for (unsigned i = 0; i < _CallbackOnChangePosition.size(); i ++)
	{
		(*_CallbackOnChangePosition[i])(shared_dynamic_cast<Entity>(this->shared_from_this()));
	}
}



} /// End: namespace VEHA
