#include "HAVE/Body.h"
#include "HAVE/EmbodiedAgent.h"
#include "VEHA/Entity/EntityClass.h"
#include "HAVE/VirtualHumanClass.h"
#include "HAVE/BodyClass.h"
namespace HAVE {
using namespace VEHA;
Body::Body(EmbodiedAgent * ea)
: Entity("body of "+ea->getName(),
		shared_dynamic_cast<EntityClass>(shared_dynamic_cast<VirtualHumanClass>(ea->getClassifier())->getBodyClass())), _agent(ea)
{
}

Body::~Body()
{}

}
