#ifndef _v2_VEHA_Entity_PathProperty_H
#define _v2_VEHA_Entity_PathProperty_H

#include "Tools/veha_plateform.h"
#include "VEHA/Entity/TopologicalProperty.h"


namespace VEHA
{

class VEHA_API PathProperty : public TopologicalProperty
{
public :
	PathProperty( const string& name,Class* cl);
	virtual ~PathProperty();
	// --- Attributes ---
	// --- Relations ---
	// --- Operations ---
};

}
#endif
