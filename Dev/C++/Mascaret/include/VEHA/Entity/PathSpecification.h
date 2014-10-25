#ifndef _v2_VEHA_Entity_Path_H
#define _v2_VEHA_Entity_Path_H

#include "Tools/veha_plateform.h"
#include "VEHA/Entity/TopologicalSpecification.h"
#include "MascaretApplication.h"

namespace VEHA
{
class PointSpecification;

class VEHA_API PathSpecification : public TopologicalSpecification
{
public :
	PathSpecification();
	PathSpecification(const string& str);
	virtual ~PathSpecification();
	// --- Attributes ---
public :
	inline vector< shared_ptr<PointSpecification> > getPoints(void) { return _points;}
	inline shared_ptr<PointSpecification> getPoint(int index) { return _points[index];}
	inline void addPoint(shared_ptr<PointSpecification> value) { _points.push_back(value);}

	// --- Operations ---
	//virtual const ValueSpecification& operator= (const ValueSpecification&);
	virtual shared_ptr<ValueSpecification> clone() const;

protected :
	vector< shared_ptr<PointSpecification> > _points;
};

}
#endif
