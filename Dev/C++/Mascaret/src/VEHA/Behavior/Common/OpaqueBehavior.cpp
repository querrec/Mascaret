#include "VEHA/Kernel/InstanceSpecification.h"
#include "VEHA/Kernel/Classifier.h"
#include "VEHA/Behavior/Common/OpaqueBehavior.h"
#include "VEHA/Behavior/Common/pluginManager.h"
#include "VEHA/Behavior/Common/BehaviorExecution.h"
#include <boost/function.hpp>

namespace VEHA
{

OpaqueBehavior::OpaqueBehavior(const string& name)
:Behavior(name)
{
}

OpaqueBehavior::~OpaqueBehavior()
{
}

//	typedef boost::function<shared_ptr<BehaviorExecution> (shared_ptr<InstanceSpecification>,	const Parameters&)>  BehaviorInitFunc;

shared_ptr<BehaviorExecution> OpaqueBehavior::createBehaviorExecution(shared_ptr<InstanceSpecification> host,const Parameters& p, bool sync)
{
	if(host)
	{
		shared_ptr<Classifier> cl = host->getClassifier();
		if(cl)
		{
			BehaviorInitDeleterFunc symbols = _lookForSymbols(cl);
			if (!symbols.init) cerr << " Erreur : Opaque behavior : "<< cl->getName()+"_"+_body+"_init" << " non trouve ..." << endl;
			else
			{
				if(symbols.deleter)
				{
					return shared_ptr<BehaviorExecution>((symbols.init)(shared_dynamic_cast<Behavior>(shared_from_this()), host,p), symbols.deleter);
				}
				else
				{
					return shared_ptr<BehaviorExecution>((symbols.init)(shared_dynamic_cast<Behavior>(shared_from_this()), host,p));
				}
			}
		}
	}
	return shared_ptr<BehaviorExecution>();
}

BehaviorInitDeleterFunc OpaqueBehavior::_lookForSymbols(shared_ptr<Classifier> cl)
{
	BehaviorInitDeleterFunc symbols;
	symbols.init = (BehaviorInitFunc)(intptr_t)PluginManager::getInstance()->getSymbol(cl->getName()+"_"+_body+"_init");

	if(symbols.init)
	{
		symbols.deleter = (BehaviorDeleterFunc)(intptr_t)PluginManager::getInstance()->getSymbol(cl->getName()+"_"+_body+"_deleter");
	}
	else
	{
		vector<shared_ptr<Classifier> > parents = cl->getParents();
		unsigned int i = 0;
		while (i < parents.size() && !symbols.init)
		{
			symbols = _lookForSymbols(parents[i++]);
		}
	}

	return symbols;
}

}
