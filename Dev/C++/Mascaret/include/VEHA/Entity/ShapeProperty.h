#ifndef _v2_VEHA_Entity_ShapeProperty_H
#define _v2_VEHA_Entity_ShapeProperty_H

#include "Tools/veha_plateform.h"
#include "VEHA/Kernel/Property.h"


namespace VEHA
{

class VEHA_API ShapeProperty : public Property
{
public :
	ShapeProperty( const string& name,Class* cl);
	virtual ~ShapeProperty();

};

}
#endif
