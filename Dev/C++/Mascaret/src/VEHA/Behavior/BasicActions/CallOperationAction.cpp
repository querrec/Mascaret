#include "VEHA/Behavior/BasicActions/CallOperationAction.h"
#include "VEHA/Behavior/BasicActions/CallOperationBehaviorExecution.h"
#include "VEHA/Kernel/Operation.h"
#include "VEHA/Kernel/Parameter.h"
#include <boost/algorithm/string.hpp>

namespace VEHA
{

CallOperationAction::CallOperationAction()
{
	_kind = "CallOperation";
}

CallOperationAction::~CallOperationAction()
{
}

void CallOperationAction::addArgument(string name, string value)
{
	map<string,string>::iterator it = _arguments.find(name);
	if (it == _arguments.end())
		_arguments[name] = value;
}

string CallOperationAction::getArgument(string name)
{
	map<string,string>::iterator it = _arguments.find(name);
	if (it != _arguments.end())
		return _arguments[name];
	else
		return "";
}


shared_ptr<BehaviorExecution> CallOperationAction::createBehaviorExecution(shared_ptr<InstanceSpecification> host, const Parameters& p, bool sync)
{
	shared_ptr<CallOperationBehaviorExecution> behavior(new CallOperationBehaviorExecution(shared_dynamic_cast<CallOperationAction>(shared_from_this()),host,p));
	return behavior;
}
Parameters CallOperationAction::getParameters()
{
	// TODO: Gerér les strings (ex: "toto,\n\"coucou\"\t") pour pouvoir y inclure des "," et autres caractères pour le moment impossible
	size_t i=0;
	string params=_stringarguments;
	Parameters parameters;
	int posEndParam = params.find(',');
cerr << " ICI ICI " << endl;
	while (posEndParam >=0)
	{
		string param=params.substr(0,posEndParam);
		boost::trim(param);
		if(_operation->getParameter().size() > i)
		{
			parameters[_operation->getParameter()[i]->getName()]=_operation->getParameter()[i]->getType()->createValueFromString(param);
		}
		params = params.substr(posEndParam+1,params.size()-1);
		posEndParam = params.find(',');
		i++;
	}
	if ( (posEndParam < 0) && (params.size()>0) )
	{
		if(_operation->getParameter().size() > i)
		{
			boost::trim(params);
			parameters[_operation->getParameter()[i]->getName()]=_operation->getParameter()[i]->getType()->createValueFromString(params);
		}
	}
	return parameters;
}

}
