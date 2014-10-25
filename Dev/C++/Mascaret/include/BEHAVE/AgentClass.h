#ifndef BEHAVE_AGENTCLASS_H
#define BEHAVE_AGENTCLASS_H

#include "Tools/veha_plateform.h"
#include "VEHA/Kernel/Class.h"

namespace BEHAVE
{
class VEHA_API AgentClass : public VEHA::Class
{
public :
	AgentClass(const string & name);
	virtual ~AgentClass();
};
}

#endif

