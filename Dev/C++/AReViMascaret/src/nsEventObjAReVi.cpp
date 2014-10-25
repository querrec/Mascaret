//-----------------------------------------------------------------------------
// nsEventObjAReVi.cpp
//-----------------------------------------------------------------------------

#include "nsEventObjAReVi.h"

//-----------------------------------------------------------------------------
// Class NSEventObjAReViPlayAnimation
//-----------------------------------------------------------------------------

AR_CLASS_DEF(NSEventObjAReViPlayAnimation,NSEvent)

//-----------------------------------------------------------------------------

NSEventObjAReViPlayAnimation::NSEventObjAReViPlayAnimation(ArCW & arCW) // Protected
  : NSEvent(arCW)
{
}

NSEventObjAReViPlayAnimation::~NSEventObjAReViPlayAnimation(void)
{}

//-----------------------------------------------------------------------------

void NSEventObjAReViPlayAnimation::writeToStream(ArRef<AbstractOStream> output) const
{
   NSEvent::writeToStream(output);
   output->writeString(animationName);
}

//-----------------------------------------------------------------------------

bool NSEventObjAReViPlayAnimation::readFromStream(ArRef<AbstractIStream> input)
{
   NSEvent::readFromStream(input);
   input->readString(animationName);
   return true;
}

//-----------------------------------------------------------------------------
// Class NSEventObjAReViAnimationPosition
//-----------------------------------------------------------------------------

AR_CLASS_DEF(NSEventObjAReViAnimationPosition,NSEvent)

//-----------------------------------------------------------------------------

NSEventObjAReViAnimationPosition::NSEventObjAReViAnimationPosition(ArCW & arCW) // Protected
  : NSEvent(arCW)
{
}

NSEventObjAReViAnimationPosition::~NSEventObjAReViAnimationPosition(void)
{}

//-----------------------------------------------------------------------------

void NSEventObjAReViAnimationPosition::writeToStream(ArRef<AbstractOStream> output) const
{
   NSEvent::writeToStream(output);
   output->writeString(animationName);
   output->writeDouble(position);
}

//-----------------------------------------------------------------------------

bool NSEventObjAReViAnimationPosition::readFromStream(ArRef<AbstractIStream> input)
{
   NSEvent::readFromStream(input);
   input->readString(animationName);
   input->readDouble(position);
   return true;
}

