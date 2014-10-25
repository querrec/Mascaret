#ifndef _v2_VEHA_Entity_ShapeGeometry_H
#define _v2_VEHA_Entity_ShapeGeometry_H

#include "Tools/veha_plateform.h"
#include "VEHA/Entity/GeometrySpecification.h"

namespace VEHA
{
class ShapeSpecification;

class VEHA_API ShapeGeometry : public GeometrySpecification
{
public :
	ShapeGeometry();
	virtual ~ShapeGeometry();

	inline shared_ptr<ShapeSpecification> getShape(void) { return _shape;}
	inline void setShape(shared_ptr<ShapeSpecification> shape) { _shape = shape;}
protected :
	shared_ptr<ShapeSpecification>  _shape;
};

}
#endif
