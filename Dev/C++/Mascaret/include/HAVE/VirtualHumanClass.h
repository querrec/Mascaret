#ifndef HAVE_VIRTUALHUMANCLASS_H
#define HAVE_VIRTUALHUMANCLASS_H


#include "Tools/veha_plateform.h"
#include "BEHAVE/AgentClass.h"

namespace HAVE
{
class BodyClass;
class VEHA_API VirtualHumanClass : public BEHAVE::AgentClass
{
public :
	VirtualHumanClass(const string & name);
	virtual ~VirtualHumanClass();

	shared_ptr<BodyClass> getBodyClass();
protected:
	shared_ptr<BodyClass> _bodyClass;
};
inline shared_ptr<BodyClass> VirtualHumanClass::getBodyClass()
{
	return _bodyClass;
}
}

#endif

