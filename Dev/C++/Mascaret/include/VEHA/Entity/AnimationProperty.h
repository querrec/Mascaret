#ifndef _v2_VEHA_Entity_AnimationProperty_H
#define _v2_VEHA_Entity_AnimationProperty_H

#include "Tools/veha_plateform.h"
#include "VEHA/Kernel/Property.h"

namespace VEHA
{

class VEHA_API AnimationProperty : public Property
{
public :
	AnimationProperty( const string& name,Class* cl);
	virtual ~AnimationProperty();
	// --- Attributes ---
	// --- Relations ---
	// --- Operations ---
};

}
#endif
