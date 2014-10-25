#include "VEHA/Entity/SoundSpecification.h"
#include "VEHA/Kernel/PrimitiveType.h"
#include "MascaretApplication.h"
#include "VEHA/Kernel/Model.h"
#include "Tools/utils.h"


namespace VEHA
{

SoundSpecification::SoundSpecification(const std::string & name, const std::string & url, double gain, double minDistance, double maxDistance, bool cycle)
:VirtualRealitySpecification(MascaretApplication::getInstance()->getModel()->getBasicType("sound")), _gain(gain), _minDistance(minDistance),_maxDistance(maxDistance)

{
	_name = name;
}

SoundSpecification::~SoundSpecification()
{
}

shared_ptr<ValueSpecification> SoundSpecification::clone() const
{
	throw std::exception();
}

}
