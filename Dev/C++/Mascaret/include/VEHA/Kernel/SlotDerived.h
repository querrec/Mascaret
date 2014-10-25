#ifndef VEHA_Kernel_Slot_DERIVED_H
#define VEHA_Kernel_Slot_DERIVED_H

#include "Tools/veha_plateform.h"
#include "VEHA/Kernel/Element.h"
#include "VEHA/Kernel/InstanceValue.h"
#include "VEHA/Kernel/Property.h"
#include "VEHA/Kernel/Slot.h"
#include <stdexcept>
#include "Tools/utils.h"
#include <boost/signals.hpp>
#include "callbacks.h"


namespace VEHA
{
class Property;
class Expression;
class ValueSpecification;
class InstanceSpecification;
//class InstanceValue;


class VEHA_API SlotDerived : public Slot
{
public :
	SlotDerived(InstanceSpecification * instance,shared_ptr<Property> property);
	virtual ~SlotDerived();
public :
	virtual inline bool _canRemoveValue(){return false;}
public :

	inline virtual void addValue(shared_ptr<ValueSpecification> ) {throw std::runtime_error("Can't add values in a derived slot");}

	void notifyMaybeChanged();
protected:
	virtual void _update() ;
	shared_ptr<Expression> _expression;
};

}
#endif
