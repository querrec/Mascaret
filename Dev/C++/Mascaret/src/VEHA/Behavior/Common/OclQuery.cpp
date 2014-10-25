#include "VEHA/Behavior/Common/OclQuery.h"
#include "VEHA/Behavior/Common/BehaviorExecution.h"
#include "VEHA/Kernel/expressionParser.h"
#include <boost/foreach.hpp>

namespace VEHA
{

OclQuery::OclQuery(const string& name)
:OpaqueBehavior(name),_ep(),_evaluated(false)
{
}

OclQuery::~OclQuery()
{
}

//	typedef BehaviorExecution * (*BehaviorInitFunc)(InstanceSpecification *,const Parameters&);
shared_ptr<BehaviorExecution> OclQuery::createBehaviorExecution(shared_ptr<InstanceSpecification> host,const Parameters& p, bool sync)
{
	if (!_evaluated)
	{
		_ep=OclParser::parseExpression(_body);
		_evaluated=true;
	}
	pair<string,shared_ptr<ValueSpecification> > param;
	OclParser::Context ctx;
	BOOST_FOREACH(param,p)
	{
		ctx[param.first]=toAnyValue(param.second);
	}
	ctx["self"]=host;

	shared_ptr<BehaviorExecution> be=
			shared_ptr<BehaviorExecution>
	(new OclQueryBehaviorExecution(shared_dynamic_cast<OclQuery>(shared_from_this()),host,p));

	be->result=checkAndGetValues(OclParser::evalExpression(_ep,ctx) );
	be->stop();
	return be;
}
vector<shared_ptr<ValueSpecification> > OclQuery::checkAndGetValues(const AnyValue& v)
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
	else  result.push_back(toValueSpecification(v));
	return result;
}

}
