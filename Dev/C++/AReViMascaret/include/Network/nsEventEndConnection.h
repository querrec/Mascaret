#ifndef NSEVENTENDCONNECTION_H
#define NSEVENTENDCONNECTION_H

#include "areviMascaret_plateform.h"
#include "AReVi/arSystem.h"
#include "AReVi/activity.h"
#include "AReViNS/remoteViewer3D.h"

namespace AReViMascaret
{
class AREVI_MASCARET_API NSEventEndConnection : public AReVi::NSEvent
{
public:
  AR_CLASS(NSEventEndConnection)
  AR_CONSTRUCTOR(NSEventEndConnection)
};
}
#endif
