#ifndef _v2_VEHA_Behavior_BasicActions_ValuePin_H
#define _v2_VEHA_Behavior_BasicActions_ValuePin_H

#include "Tools/veha_plateform.h"
#include "VEHA/Behavior/BasicActions/InputPin.h"
#include "VEHA/Kernel/ValueSpecification.h"

namespace VEHA
{
class VEHA_API ValuePin : public InputPin
{

private:
	shared_ptr<ValueSpecification> _value;

public :
	ValuePin();
	virtual ~ValuePin();

	inline void setValue(shared_ptr<ValueSpecification> value){_value = value;}
	inline shared_ptr<ValueSpecification> getValue (){return _value;}



};

}
#endif
