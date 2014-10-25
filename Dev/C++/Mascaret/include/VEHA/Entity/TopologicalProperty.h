#ifndef _v2_VEHA_Entity_TopologicalProperty_H
#define _v2_VEHA_Entity_TopologicalProperty_H

#include "Tools/veha_plateform.h"
#include "VEHA/Kernel/Property.h"

namespace VEHA
{

class VEHA_API TopologicalProperty : public Property
{
public :
	TopologicalProperty( const string& name,Class* cl,shared_ptr<Classifier> type);
	virtual ~TopologicalProperty();
	// --- Attributes ---
	// --- Relations ---
	// --- Operations ---
};

}
#endif
