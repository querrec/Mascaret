#ifndef HAVE_VIRTUALHUMAN_H
#define HAVE_VIRTUALHUMAN_H

#include "Tools/veha_plateform.h"
#include "HAVE/EmbodiedAgent.h"


namespace HAVE
{
class VEHA_API VirtualHuman : public EmbodiedAgent
{
public :
	VirtualHuman(shared_ptr<BEHAVE::AgentPlateform> ap, const string & name, shared_ptr<BEHAVE::AgentClass> agentclass);
	virtual ~VirtualHuman();

	virtual bool playAnimation(const string& animationName, int sens = 1, double animationSpeed = 1.0, bool cycle = false);
	virtual bool stopAnimation(const string& animationName);
	virtual bool stopAllAnimations();
	virtual bool playSound(const string& soundName, double pitch = 1.0, bool cycle = false);
	virtual bool stopSound(const string& soundName);
};
}

#endif

