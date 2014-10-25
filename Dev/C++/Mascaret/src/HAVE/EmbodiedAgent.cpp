#include "HAVE/EmbodiedAgent.h"
#include "HAVE/Body.h"

namespace HAVE {

EmbodiedAgent::EmbodiedAgent(shared_ptr<AgentPlateform> ap, const string & name, shared_ptr<AgentClass> agentclass) : Agent(ap, name, agentclass)
{
   _body = shared_ptr<Body> (new Body(this));
}

EmbodiedAgent::~EmbodiedAgent()
{}

}
