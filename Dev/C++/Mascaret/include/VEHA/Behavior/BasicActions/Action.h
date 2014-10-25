#ifndef _v2_VEHA_Behavior_BasicActions_Action_H
#define _v2_VEHA_Behavior_BasicActions_Action_H

#include "Tools/veha_plateform.h"
#include "VEHA/Behavior/Common/Behavior.h"
#include "VEHA/Behavior/Common/BehaviorExecution.h"
#include "VEHA/Behavior/Common/Parameters.h"
#include "VEHA/Behavior/BasicActions/InputPin.h"
#include "VEHA/Behavior/BasicActions/OutputPin.h"
#include "VEHA/Behavior/BasicActions/ValuePin.h"

namespace VEHA
{
class VEHA_API Action : public Behavior
{
protected :
	string _kind;
	vector<shared_ptr<InputPin> > _inputPins;
	vector<shared_ptr<ValuePin> > _valuePins;
	vector<shared_ptr<OutputPin> > _outputPins;

public :
	Action();
	virtual ~Action();
	inline string getKind(void) {return _kind;}
	// --- Attributes ---
	// --- Relations ---
	// --- Operations ---
	virtual shared_ptr<BehaviorExecution> createBehaviorExecution(shared_ptr<InstanceSpecification> host,const Parameters& p, bool sync = false) = 0;

	virtual inline void addInputPin(shared_ptr<InputPin> inputPin){_inputPins.push_back(inputPin);}
	virtual inline void addOutputPin(shared_ptr<OutputPin> outputPin){_outputPins.push_back(outputPin);}
	virtual inline void addValuePin(shared_ptr<ValuePin> valuePin){_valuePins.push_back(valuePin);}
	virtual inline vector<shared_ptr<InputPin> > getInputPin(){return _inputPins;}
	virtual inline vector<shared_ptr<OutputPin> > getOutputPin(){return _outputPins;}
	virtual inline vector<shared_ptr<ValuePin> > getValuePins(){return _valuePins;}

};

}
#endif
