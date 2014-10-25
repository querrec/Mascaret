#ifndef HAVE_BODY_H
#define HAVE_BODY_H

#include "Tools/veha_plateform.h"
#include "VEHA/Entity/Entity.h"
#include "HAVE/EmbodiedAgent.h"


namespace HAVE
{
using VEHA::Entity;
class VEHA_API Body : public Entity
{
public :
	Body(EmbodiedAgent * ea);
	virtual ~Body();
	inline shared_ptr<EmbodiedAgent> getAgent() {return shared_dynamic_cast<EmbodiedAgent>(_agent->shared_from_this());}
private :
	EmbodiedAgent * _agent;

};
}

#endif

