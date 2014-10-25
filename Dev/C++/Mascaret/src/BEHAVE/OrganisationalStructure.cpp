#include "BEHAVE/OrganisationalStructure.h"
#include "BEHAVE/Role.h"

namespace BEHAVE
{

OrganisationalStructure::OrganisationalStructure(const string & name) : Element(name)
{}

OrganisationalStructure::~OrganisationalStructure()
{}


shared_ptr<Role> OrganisationalStructure::getRole(string roleName) const
{
	for(unsigned int i=0; i<_roles.size(); i++)
		if(_roles[i]->getName() == roleName)
			return _roles[i];

	return shared_ptr<Role>();
}

void OrganisationalStructure::removeRole(shared_ptr<Role> r)
{
	vector<shared_ptr<Role> >::iterator it;
	for(it = _roles.begin(); it != _roles.end(); it++)
	{
		if(*it == r)
		{
			_roles.erase(it);
			return;
		}
	}
}

}
