#ifndef _v2_VEHA_Entity_PointProperty_H
#define _v2_VEHA_Entity_PointProperty_H

#include "Tools/veha_plateform.h"
#include "VEHA/Entity/TopologicalProperty.h"

namespace VEHA
{

class VEHA_API PointProperty : public TopologicalProperty
{
public :
	PointProperty(const string& name,Class* cl);
	virtual ~PointProperty();
	// --- Attributes ---
	// --- Relations ---
	// --- Operations ---
};

}
#endif
