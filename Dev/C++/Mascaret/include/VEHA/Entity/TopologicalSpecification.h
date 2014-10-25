#ifndef _v2_VEHA_Entity_TopologicalSpecification_H
#define _v2_VEHA_Entity_TopologicalSpecification_H

#include "Tools/veha_plateform.h"
#include "VEHA/Entity/VirtualRealitySpecification.h"

namespace VEHA
{
class Entity;

class VEHA_API TopologicalSpecification : public VirtualRealitySpecification
{
public :
	TopologicalSpecification(shared_ptr<Classifier> type);
	virtual ~TopologicalSpecification();

};

}
#endif
