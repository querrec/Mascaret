//-----------------------------------------------------------------------------
// nsCtrlObjAReVi.cpp
//-----------------------------------------------------------------------------

#include "nsCtrlObjAReVi.h"
#include "nsEventObjAReVi.h"

//-----------------------------------------------------------------------------
// Class NSListenerObjAReVi
//-----------------------------------------------------------------------------

AR_CLASS_DEF(NSListenerObjAReVi,NSListenerBase3D)

//-----------------------------------------------------------------------------

NSListenerObjAReVi::NSListenerObjAReVi(ArCW & arCW) // Protected
  : NSListenerBase3D(arCW)
{
}

NSListenerObjAReVi::~NSListenerObjAReVi(void)
{}

//-----------------------------------------------------------------------------

void
NSListenerObjAReVi::init(ArRef<LocalDepService> service, ArRef<ArObject> object)
{

  if (!object->getClass()->isA(ObjAReVi::CLASS())){
    return;
  }  
  _position = -1;
  
  NSListenerBase3D::init(service,object);

  _obj = ar_down_cast<ObjAReVi>(object);

  _obj->addPlayAnimationCB(thisRef(),&NSListenerObjAReVi::_onPlayAnimationCB);
  _obj->addAnimationPositionCB(thisRef(),&NSListenerObjAReVi::_onAnimationPositionCB);
}

//-----------------------------------------------------------------------------

void
NSListenerObjAReVi::getPendingEvents(StlList<ArRef<NSEvent> > &events)
{
  NSListenerBase3D::getPendingEvents(events);


  if (_position !=-1)
  {
    ArRef<NSEventObjAReViAnimationPosition> event = NSEventObjAReViAnimationPosition::NEW();
    event->animationName = _animationplayed;
    event->position = _position;

    events.push_back(event);
    _animationplayed = "";
    _position = -1;
  }
  else if (_animationplayed !="")
  {
    ArRef<NSEventObjAReViPlayAnimation> event = NSEventObjAReViPlayAnimation::NEW();
    event->animationName = _animationplayed;

    events.push_back(event);
    _animationplayed = "";
  }

}

//-----------------------------------------------------------------------------
void
NSListenerObjAReVi::_onPlayAnimationCB(const ObjAReVi::PlayAnimationEvent & event){
  _animationplayed = event.animname;
}

//-----------------------------------------------------------------------------
void
NSListenerObjAReVi::_onAnimationPositionCB(const ObjAReVi::AnimationPositionEvent & event){
  _animationplayed = event.animname;
  _position = event.position;
}

//-----------------------------------------------------------------------------
// Class NSInteractorObjAReVi
//-----------------------------------------------------------------------------

AR_CLASS_DEF(NSInteractorObjAReVi,NSInteractorBase3D)

//-----------------------------------------------------------------------------

NSInteractorObjAReVi::NSInteractorObjAReVi(ArCW & arCW) 
  : NSInteractorBase3D(arCW),
    _act(Activity::NEW(1.0/24.0))
{
  _act->setBehavior(thisRef(),&NSInteractorObjAReVi::activate);
}

NSInteractorObjAReVi::~NSInteractorObjAReVi(void)
{}

//-----------------------------------------------------------------------------

void
NSInteractorObjAReVi::init(ArRef<RemoteDepService> service, ArRef<ArObject> object)
{
  NSInteractorBase3D::init(service,object);
  
  _obj = ar_down_cast<ObjAReVi>(object);

  _registerEvent(NSEventObjAReViPlayAnimation::CLASS()->getName(),
                (NetworkService::EventMethod)&NSInteractorObjAReVi::_onPlayAnimation);
  _registerEvent(NSEventObjAReViAnimationPosition::CLASS()->getName(),
                (NetworkService::EventMethod)&NSInteractorObjAReVi::_onAnimationPosition);
}

//------------------------------------------------------------------------------

bool
NSInteractorObjAReVi:: activate(ArRef<Activity> /*act*/, double /*dt*/)
{
  return true;
}

//------------------------------------------------------------------------------

NSEvent::EventProcess
NSInteractorObjAReVi::_onPlayAnimation(ArConstRef<NSEvent> eventObj)
{
  ArConstRef<NSEventObjAReViPlayAnimation> event = ar_down_cast<NSEventObjAReViPlayAnimation>(eventObj);
  return NSEvent::EP_OK;
}

//------------------------------------------------------------------------------

NSEvent::EventProcess
NSInteractorObjAReVi::_onAnimationPosition(ArConstRef<NSEvent> eventObj)
{
  ArConstRef<NSEventObjAReViAnimationPosition> event = ar_down_cast<NSEventObjAReViAnimationPosition>(eventObj);
  _obj->setAnimationPosition(event->animationName, event->position);
  return NSEvent::EP_OK;
}



