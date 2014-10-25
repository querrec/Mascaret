#ifndef BEHAVE_ROLECLASS_H
#define BEHAVE_ROLECLASS_H


#include "Tools/veha_plateform.h"
#include "VEHA/Kernel/Class.h"

namespace BEHAVE
{
class VEHA_API RoleClass : public VEHA::Class
{
public :
	RoleClass(const string & name);
	virtual ~RoleClass();
};
}

#endif

