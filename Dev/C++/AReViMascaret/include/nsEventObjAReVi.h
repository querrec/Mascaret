//-----------------------------------------------------------------------------
// nsEventObjAReVi.h
//-----------------------------------------------------------------------------

#ifndef NSEVT_OBJAREVI_H
#define NSEVT_OBJAREVI_H

#include "areviMascaret_plateform.h"
#include "AReVi/arObject.h"
#include "AReViNS/Events/nsEvent.h"
#include "AReVi/Utils/abstractStream.h"

#include "ObjAReVi.h"
using namespace AReVi;


//-----------------------------------------------------------------------------
// Class NSEventObjAReViPlayAnimation
//-----------------------------------------------------------------------------

class AREVI_MASCARET_API NSEventObjAReViPlayAnimation : public NSEvent
{
public:
        AR_CLASS(NSEventObjAReViPlayAnimation)
        AR_CONSTRUCTOR(NSEventObjAReViPlayAnimation)

        virtual void writeToStream(ArRef<AbstractOStream> output) const; 
        virtual bool readFromStream(ArRef<AbstractIStream> input); 

        StlString animationName;

};

//-----------------------------------------------------------------------------
// Class NSEventObjAReViAnimationPosition
//-----------------------------------------------------------------------------

class NSEventObjAReViAnimationPosition : public NSEvent
{
public:
        AR_CLASS(NSEventObjAReViAnimationPosition)
        AR_CONSTRUCTOR(NSEventObjAReViAnimationPosition)

        virtual void writeToStream(ArRef<AbstractOStream> output) const; 
        virtual bool readFromStream(ArRef<AbstractIStream> input); 

        StlString animationName;
        double position;

};

#endif
