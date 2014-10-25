//-----------------------------------------------------------------------------
// nsCtrlObjAReVi.h
//-----------------------------------------------------------------------------

#ifndef NSCTRL_OBJAREVI_H
#define NSCTRL_OBJAREVI_H

#include "areviMascaret_plateform.h"
#include "AReVi/activity.h"
#include "AReViNS/Controlers/nsCtrlBase3D.h"
#include "ObjAReVi.h"
using namespace AReVi;


//-----------------------------------------------------------------------------
// Class NSListenerObjAReVi
//-----------------------------------------------------------------------------

class AREVI_MASCARET_API NSListenerObjAReVi : public NSListenerBase3D
{
public:
        AR_CLASS(NSListenerObjAReVi)
        AR_CONSTRUCTOR(NSListenerObjAReVi)

        void init(ArRef<LocalDepService> service, ArRef<ArObject> object);
        void getPendingEvents(StlList<ArRef<NSEvent> > &events);

private :
        void _onPlayAnimationCB(const ObjAReVi::PlayAnimationEvent & event);
        void _onAnimationPositionCB(const ObjAReVi::AnimationPositionEvent & event);
        StlString _animationplayed;
        double _position;
        ArRef<ObjAReVi> _obj;

};

//-----------------------------------------------------------------------------
// Class NSInteractorObjAReVi
//-----------------------------------------------------------------------------

class NSInteractorObjAReVi : public NSInteractorBase3D
{
public:
        AR_CLASS(NSInteractorObjAReVi)
        AR_CONSTRUCTOR(NSInteractorObjAReVi)
       
        void init(ArRef<RemoteDepService> service, ArRef<ArObject> object);
        bool activate(ArRef<Activity> /*act*/, double dt);

protected :
	ArRef<Activity> _act;
        ArRef<ObjAReVi> _obj;

        NSEvent::EventProcess _onPlayAnimation(ArConstRef<NSEvent> eventObj);
        NSEvent::EventProcess _onAnimationPosition(ArConstRef<NSEvent> eventObj);

};

#endif
