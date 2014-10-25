#include "VEHA/Kernel/Constraint.h"

namespace VEHA
{

Constraint::Constraint()
{
}

Constraint::~Constraint()
{
}
bool Constraint::eval(OclParser::Context context)
{
	if(shared_dynamic_cast<Expression>(getSpecification()))
	{
		return shared_dynamic_cast<Expression>(getSpecification())->evaluateExpression(context);
	}
	else
	{
		bool b=false;
		getSpecification()->getBoolValue(b);
		return b;
	}
	return false;
}

}
