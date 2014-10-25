#ifndef _v2_VEHA_Kernel_Expression_H
#define _v2_VEHA_Kernel_Expression_H
#include "Tools/veha_plateform.h"
#include "VEHA/Kernel/ValueSpecification.h"
#include "VEHA/Kernel/expressionParser.h"
#include "boost/signals/signal0.hpp"

namespace VEHA
{
class ValueSpecification;
class SlotDerived;


class VEHA_API Expression : public ValueSpecification
{
public :
	Expression(const string& expression,shared_ptr<Classifier> returnType);
	virtual ~Expression();
	// --- Attributes ---
	virtual AnyValue evaluateExpression(shared_ptr<InstanceSpecification> owner);
	virtual AnyValue evaluateExpression(const OclParser::Context& c);
	virtual vector<shared_ptr<ValueSpecification> > checkAndGetValues(const AnyValue& v);
protected :
	virtual void _parseExpression();
	string _expression;
	OclParser::ExpressionTree _expressionTree;

	//                vector<shared_ptr<ValueSpecification> > _operand;
public :
	inline const string& getExpression(){return _expression;}

	virtual shared_ptr<ValueSpecification> clone() const;
	//                inline vector<shared_ptr<ValueSpecification> > getSpecification(void) {return _operand;}
	//                inline shared_ptr<ValueSpecification> getSpecification_at(unsigned int index) {return _operand[index];}
	//                inline void addSpecification(shared_ptr<ValueSpecification> value) {_operand.push_back(value);}
	// --- Relations ---
	// --- Operations ---

	bool getNeedUpdate(){return _needUpdate;}
	void notifyNeedUpdate();
	void addCallbackOnMayChange(const boost::signal0<void>::slot_type & f){_cbOnNeedUpdate.connect(f);}
	template <typename SigSlot>
	void removeCallbackOnMayChange(const SigSlot& f){_cbOnNeedUpdate.disconnect(f);}
protected:

	void startRegisterChangesMethods();
	void stopRegisterChangesMethods();
protected:
	friend class SlotDerived;
	bool _needUpdate;
	AnyValue _result;
	vector<boost::signal0<void>* > _callbacks;
	boost::signal0<void> _cbOnNeedUpdate;
};

}
#endif
