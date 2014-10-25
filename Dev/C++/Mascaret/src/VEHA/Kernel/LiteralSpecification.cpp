#include "VEHA/Kernel/LiteralSpecification.h"

namespace VEHA
{

LiteralSpecification::LiteralSpecification(shared_ptr<Classifier> type)
:ValueSpecification(type)
{
	CLASSTYPE="Litteral";
}

LiteralSpecification::~LiteralSpecification()
{
}

}
