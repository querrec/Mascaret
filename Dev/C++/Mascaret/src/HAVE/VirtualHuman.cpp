#include "HAVE/VirtualHuman.h"
#include "VEHA/Entity/ShapeSpecification.h"
#include "HAVE/Body.h"


namespace HAVE {

VirtualHuman::VirtualHuman(shared_ptr<AgentPlateform> ap, const string & name, shared_ptr<AgentClass> agentclass) : EmbodiedAgent(ap, name, agentclass)
{
}

VirtualHuman::~VirtualHuman()
{}

bool VirtualHuman::playAnimation(const string& animationName, int sens, double animationSpeed, bool cycle)
{
	return getBody()->playAnimation(animationName,sens,animationSpeed, cycle);
}
bool VirtualHuman::stopAnimation(const string& animationName)
{
	return getBody()->stopAnimation(animationName);
}
bool VirtualHuman::stopAllAnimations()
{
	return getBody()->stopAllAnimations();
}
bool VirtualHuman::playSound(const string& soundName, double pitch, bool cycle)
{
	return getBody()->playSound(soundName,pitch, cycle);
}
bool VirtualHuman::stopSound(const string& soundName)
{
	return getBody()->stopSound(soundName);
}
}
