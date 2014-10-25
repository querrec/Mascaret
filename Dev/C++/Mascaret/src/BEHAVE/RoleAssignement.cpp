#include "BEHAVE/RoleAssignement.h"
#include "BEHAVE/OrganisationalEntity.h"

namespace BEHAVE
{

RoleAssignement::RoleAssignement(shared_ptr<OrganisationalEntity> organisation) : _organisation(organisation)
{}

RoleAssignement::~RoleAssignement()
{}

}
