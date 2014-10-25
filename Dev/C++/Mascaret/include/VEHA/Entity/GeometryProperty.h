#ifndef _v2_VEHA_Entity_GeometryProperty_H
#define _v2_VEHA_Entity_GeometryProperty_H

#include "Tools/veha_plateform.h"
#include "VEHA/Entity/TopologicalProperty.h"


namespace VEHA
{

class VEHA_API GeometryProperty : public TopologicalProperty
{
public :
	GeometryProperty( const string& name,Class* cl,shared_ptr<Classifier> type);
	virtual ~GeometryProperty();
	// --- Attributes ---
	// --- Relations ---
	// --- Operations ---
};

}
#endif
