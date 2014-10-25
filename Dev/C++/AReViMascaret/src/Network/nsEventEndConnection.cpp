#include "Network/nsEventEndConnection.h"
using namespace AReVi;
namespace AReViMascaret
{
AR_CLASS_DEF(NSEventEndConnection,NSEvent)
NSEventEndConnection::NSEventEndConnection(ArCW & arCW) // Protected
  : NSEvent(arCW)
{
  // Nothing to be done
}

NSEventEndConnection::~NSEventEndConnection(void)
{
  // Nothing to be done
}
}
