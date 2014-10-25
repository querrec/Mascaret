#include "VEHA/Kernel/Package.h"
#include "VEHA/Kernel/Class.h"
#include "VEHA/Kernel/Package.h"

namespace VEHA
{

Package::Package(const string& name)
:Element(name)
{
}

Package::~Package()
{
}

void Package::addClasses(shared_ptr<Class> value)
{
	_classes.push_back(value);
	value->setPackage(shared_dynamic_cast<Package>(shared_from_this()));
}

shared_ptr<Class>  Package::getClass(const string& name) const
{
	for (size_t i=_classes.size();i--;)
		if (_classes[i]->getName()==name)
			return _classes[i];
	return shared_ptr<Class>();
}

}
