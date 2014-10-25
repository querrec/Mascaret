#include "BEHAVE/AgentBehavior.h"
#include "VEHA/Behavior/Common/pluginManager.h"
#include "boost/function.hpp"

#include "BEHAVE/SimpleCommunicationBehavior.h"
#include "BEHAVE/ProceduralBehavior.h"
#include "BEHAVE/ActionListenerBehavior.h"


namespace BEHAVE
{
typedef BehaviorExecution* (*BehaviorInitFunc)(shared_ptr<Behavior>, shared_ptr<InstanceSpecification>,const Parameters&);

AgentBehavior::AgentBehavior(const string & name) : Behavior(name)
{}

AgentBehavior::~AgentBehavior()
{}

shared_ptr<BehaviorExecution> AgentBehavior::createBehaviorExecution(shared_ptr<InstanceSpecification> host,const Parameters& p, bool sync)
{
	if (getName() == "SimpleCommunicationBehavior")
	{
		return shared_ptr<BehaviorExecution>(new SimpleCommunicationBehavior(shared_dynamic_cast<Behavior>(shared_from_this()), host,p));
	}
	else if (getName() == "ProceduralBehavior")
	{
		return shared_ptr<BehaviorExecution>(new ProceduralBehavior(shared_dynamic_cast<Behavior>(shared_from_this()), host,p));
	}
	else if (getName() == "ActionListenerBehavior")
	{
		return shared_ptr<BehaviorExecution>(new ActionListenerBehavior(shared_dynamic_cast<Behavior>(shared_from_this()), host,p));
	}
	else
	{
		BehaviorInitDeleterFunc symbols = _lookForSymbols();
		if (!symbols.init) cerr << " Erreur : Agent behavior : "<< getName()+"_init" << " non trouve ..." << endl;
		else
		{
			if(symbols.deleter)
			{
				return shared_ptr<BehaviorExecution>((symbols.init)(shared_dynamic_cast<Behavior>(shared_from_this()), host, p), symbols.deleter);
			}
			else
			{
				return shared_ptr<BehaviorExecution>((symbols.init)(shared_dynamic_cast<Behavior>(shared_from_this()), host, p));
			}
		}
		return shared_ptr<BehaviorExecution>();
	}
}

BehaviorInitDeleterFunc AgentBehavior::_lookForSymbols()
{
	BehaviorInitDeleterFunc symbols;
	symbols.init = (BehaviorInitFunc)(intptr_t)PluginManager::getInstance()->getSymbol(getName()+"_init");

	if(symbols.init)
	{
		symbols.deleter = (BehaviorDeleterFunc)(intptr_t)PluginManager::getInstance()->getSymbol(getName()+"_deleter");
	}

	return symbols;
}
}
