#include "VEHA/Kernel/SlotDerived.h"
#include "VEHA/Kernel/Property.h"
#include "VEHA/Kernel/ValueSpecification.h"
#include "VEHA/Kernel/InstanceValue.h"
#include "VEHA/Kernel/InstanceSpecification.h"
#include "VEHA/Kernel/Expression.h"
#include <boost/bind.hpp>

namespace VEHA
{

SlotDerived::SlotDerived(InstanceSpecification * instance, shared_ptr<Property> property)
:Slot(instance,property)
{
	shared_ptr<Property> prop=getDefiningProperty();
	if(prop)
	{
		cerr<<"slot derived:"<<shared_dynamic_cast<Expression>(prop->getDefaultValue())->getExpression() <<" from "<<instance->getName()<<endl;
		_expression=shared_dynamic_cast<Expression>(prop->getDefaultValue()->clone());
		if (!_expression) _expression=shared_ptr<Expression>(new Expression(string("'No Expression'"),prop->getType()));
		_values.clear();
		_expression->_cbOnNeedUpdate.connect(boost::bind(&SlotDerived::notifyMaybeChanged,this));
	}
}

SlotDerived::~SlotDerived()
{
}
void SlotDerived::_update()
{
	if (_expression->getNeedUpdate())
	{
		//Remove old values
		//		map<string,shared_ptr<ValueSpecification> >::iterator it;
		//		for (it=_values.begin();it!=_values.end();it++)
		//			_removeValue(it->first);
		_values.clear();
		//add new values
		shared_ptr<InstanceSpecification> is=shared_dynamic_cast<InstanceSpecification>(_owningInstance->shared_from_this());
		vector<shared_ptr<ValueSpecification> > values
		=_expression->checkAndGetValues(
				_expression->evaluateExpression(is	));
		for(size_t i=values.size();i--;)
			_addValue(values[i]);
	}

}
void SlotDerived::notifyMaybeChanged()
{
	_cbOnChange();
}

}
