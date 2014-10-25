#include "HAVE/VirtualHumanClass.h"
#include "HAVE/BodyClass.h"
namespace HAVE {

VirtualHumanClass::VirtualHumanClass(const string & name) : AgentClass(name)
{
	_bodyClass=shared_ptr<BodyClass>(new BodyClass(string("Body"+name),this));
}

VirtualHumanClass::~VirtualHumanClass()
{}

}
