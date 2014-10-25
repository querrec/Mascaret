#include "VEHA/Kernel/Expression.h"
#include <stdexcept>
#include "VEHA/Kernel/CallbackManager.h"
#include <boost/bind.hpp>

using std::back_insert_iterator;

namespace VEHA
{

Expression::Expression(const string& expression,shared_ptr<Classifier> returnType)
//TODO
:ValueSpecification(returnType),
 _expression(expression),_needUpdate(true)
{
	cerr<<"Create expression with : "<<expression<< " for type :"<< returnType->getName()<<endl;
	CLASSTYPE="Expression";
	_parseExpression();
	//            throw std::runtime_error("Expression NOT Implemented");
}

Expression::~Expression()
{
	cerr<<"Expression :" <<_expression<<" is destroyed"<<endl;
}
AnyValue Expression::evaluateExpression(shared_ptr<InstanceSpecification> owner)
{
	//		CallbackManager_changesMethod::addChangeMethodSignal(&_cbOnNeedUpdate); //TODO pb si l'exp disparait
	//if (!_needUpdate) return _result;

	try
	{
		startRegisterChangesMethods();

		_result=OclParser::evalExpression(_expressionTree,owner);

		//stopRegisterChangesMethods();
		_needUpdate=false;
	}
	catch (std::exception& e)
	{
		cerr << "[Expression Error] Evaluating of expression: " << _expression << " throw an exception." << endl;
		throw;
	}

	return _result;
}
AnyValue Expression::evaluateExpression(const OclParser::Context& c)
{
	//		CallbackManager_changesMethod::addChangeMethodSignal(&_cbOnNeedUpdate); //TODO pb si l'exp disparait
	//if (!_needUpdate) return _result;

	try
	{
		startRegisterChangesMethods();

		_result=OclParser::evalExpression(_expressionTree,c);

		stopRegisterChangesMethods();
		_needUpdate=false;
	}
	catch(std::exception& e)
	{
		cerr << "[Expression Error] Evaluating of expression: " << _expression << " throw an exception." << endl;
		throw;
	}

	return _result;
}
vector<shared_ptr<ValueSpecification> > Expression::checkAndGetValues(const AnyValue& v)
{
	vector<shared_ptr<ValueSpecification> > result;
	if (v.type()==typeid(CollectionAnyValue))
	{
		CollectionAnyValue vec=(CollectionAnyValue)v;
		//boucle
		for (size_t i=0;i<vec.size();i++)
		{
			vector<shared_ptr<ValueSpecification> > val=checkAndGetValues(vec[i]);
			result.insert(result.end(),val.begin(),val.end());
		}
	}
	else  result.push_back(getType()->createValueFromString(v));
	return result;
}

void Expression::_parseExpression()
{
	_expressionTree=OclParser::parseExpression(_expression);
}

shared_ptr<ValueSpecification> Expression::clone() const
{
	cerr<<"Cloning:";
	Expression * vs=new Expression(_expression,getType());

	return shared_ptr<ValueSpecification>(vs);
}

void Expression::startRegisterChangesMethods()
{
	CallbackManager_changesMethod::registerChangesMethodsFor(this);
}
void Expression::stopRegisterChangesMethods()
{
	vector<boost::signal0<void>* > newCallbacks=CallbackManager_changesMethod::stopRegisterChangesMethodsFor(this);
	sort(newCallbacks.begin(),newCallbacks.end());
	vector<boost::signal0<void>* > toDelete,toAdd;
	back_insert_iterator<vector<boost::signal0<void>* > > it_ins(toDelete);

	set_difference(_callbacks.begin(),_callbacks.end(),newCallbacks.begin(),newCallbacks.end(),it_ins);
	it_ins=back_insert_iterator<vector<boost::signal0<void>* > >(toAdd);
	set_difference(newCallbacks.begin(),newCallbacks.end(),_callbacks.begin(),_callbacks.end(),it_ins);
	for(size_t i=toAdd.size();i--;)toAdd[i]->connect(boost::bind(&Expression::notifyNeedUpdate,this));
	for(size_t i=toDelete.size();i--;)
           if (toDelete[i]) toDelete[i]->disconnect(boost::bind(&Expression::notifyNeedUpdate,this));
	_callbacks=newCallbacks;

}
void Expression::notifyNeedUpdate()
{
	if (!_needUpdate)
	{
		cerr<<"NeedUpdate: "<<"("<<_expression<<")"<<endl;
		_needUpdate=true;
		_cbOnNeedUpdate();
	}
}

}
