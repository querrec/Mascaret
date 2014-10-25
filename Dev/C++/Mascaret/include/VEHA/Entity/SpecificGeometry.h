#ifndef _v2_VEHA_Entity_SpecificGeometry_H
#define _v2_VEHA_Entity_SpecificGeometry_H

#include "Tools/veha_plateform.h"
#include "VEHA/Entity/GeometrySpecification.h"


namespace VEHA
{

class VEHA_API SpecificGeometry : public GeometrySpecification
{
public :
	SpecificGeometry();
	virtual ~SpecificGeometry();
	// --- Attributes ---
	//------ body -----
protected :
	string _body;
public :
	inline string getBody(void) { return _body;}
	inline void setBody(const string& body) { _body = body;}
	// --- Relations ---
	// --- Operations ---
};

}
#endif
