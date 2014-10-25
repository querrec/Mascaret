#include "VEHA/Kernel/OpaqueExpression.h"
#include <stdexcept>

namespace VEHA
{

OpaqueExpression::OpaqueExpression()
//TODO
:ValueSpecification(shared_ptr<Classifier>())
{
	throw std::runtime_error("OpaqueExpression NOT Implemented");
}

OpaqueExpression::~OpaqueExpression()
{
}
const ValueSpecification& OpaqueExpression::operator= (const ValueSpecification& vs)
{
	if (vs.getType() != getType())
		throw std::bad_cast();
	const OpaqueExpression & p= dynamic_cast<const OpaqueExpression&>(vs);
	_language=p.getLanguage();
	_body=p.getBody();
	return *this;
}
shared_ptr<ValueSpecification> OpaqueExpression::clone() const
{
	shared_ptr<OpaqueExpression> oe(new OpaqueExpression());
	oe->setLanguage(_language);
	oe->setBody(_body);
	return oe;

}

}
