#ifndef _v2_VEHA_Entity_SoundProperty_H
#define _v2_VEHA_Entity_SoundProperty_H

#include "Tools/veha_plateform.h"
#include "VEHA/Kernel/Property.h"

namespace VEHA
{

class VEHA_API SoundProperty : public Property
{
public :
	SoundProperty(const string& name,Class* cl);
	virtual ~SoundProperty();
	// --- Attributes ---
	// --- Relations ---
	// --- Operations ---
};

}
#endif
