#ifndef BEHAVE_KB_H
#define BEHAVE_KB_H


#include "Tools/veha_plateform.h"
#include "VEHA/Kernel/Element.h"
#include "VEHA/Kernel/Environment.h"

using namespace VEHA;

namespace BEHAVE
{
using VEHA::Element;
class VEHA_API KnowledgeBase : public Element
{
public :
	KnowledgeBase(const string & name);
	virtual ~KnowledgeBase();

        inline void setEnvironment (shared_ptr<Environment> env) {_environment = env;}
        inline shared_ptr<Environment> getEnvironment (void) {return _environment;}

protected : 
	shared_ptr<Environment> _environment;
};
}

#endif

