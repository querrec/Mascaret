#ifndef _v2_VEHA_Entity_GeometrySpecification_H
#define _v2_VEHA_Entity_GeometrySpecification_H

#include "Tools/veha_plateform.h"
#include "VEHA/Entity/TopologicalSpecification.h"
#include "MascaretApplication.h"

namespace VEHA
{

class VEHA_API GeometrySpecification : public TopologicalSpecification
{
public :
	GeometrySpecification();
	virtual ~GeometrySpecification();
	// --- Attributes ---
	// --- Relations ---
	// --- Operations ---
	// --- contains ---
public :
	//bool contains(GeometrySpecification e);
};

}
#endif
