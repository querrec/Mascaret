#ifndef HAVE_EMBODIEDAGENT_H
#define HAVE_EMBODIEDAGENT_H


#include "Tools/veha_plateform.h"
#include "BEHAVE/Agent.h"

// #include "HAVE/Body.h"


namespace HAVE
{
using BEHAVE::AgentPlateform;
using BEHAVE::AgentClass;
class Body;

class VEHA_API EmbodiedAgent : public BEHAVE::Agent
{
public :
	EmbodiedAgent(shared_ptr<AgentPlateform> ap, const string & name, shared_ptr<AgentClass> agentclass);
	virtual ~EmbodiedAgent();

	inline shared_ptr<Body> getBody(void) {return _body;}

protected :
	shared_ptr<Body> _body;
};
}

#endif

