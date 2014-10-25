#ifndef CALLBACK_MANAGER_H
#define CALLBACK_MANAGER_H


#include <boost/signals/signal0.hpp>
#include "Tools/veha_plateform.h"

namespace VEHA
{

class VEHA_API CallbackManager_changesMethod
{
public:
	/*
		static CallbackManager_changesMethod * get(){
			if (!_instance)_instance=new CallbackManager_changesMethod();
			return _instance;
			}
	 */
	static void registerChangesMethodsFor(void * p)
	{
		//cerr<<"Creating cb for:"<<p<<endl;
		_signals.insert(make_pair(p,vector<boost::signal0<void>* >()));
	}
	static vector<boost::signal0<void>* > stopRegisterChangesMethodsFor(void * p)
			{
		//cerr<<"Stop cb for:"<<p<<endl;
		vector<boost::signal0<void>* > result;
		map<void*,vector<boost::signal0<void> *> >::iterator it;
		if ((it=_signals.find(p))!=_signals.end())
		{
			result=it->second;
			_signals.erase(it);
		}
		return result;
			}
	static void addChangeMethodSignal(boost::signal0<void> * s )
	{
		map<void*,vector<boost::signal0<void> *> >::iterator it;
		for(it=_signals.begin();it!=_signals.end();it++)
		{
		//	cerr<<"adding cb "<< s <<"for "<<it->first<<endl;
			it->second.push_back(s);
		}
	}


private:
	static map<void*,vector<boost::signal0<void> *> > _signals;

	//		static CallbackManager_changesMethod * _instance;
	CallbackManager_changesMethod(){}
};

}
#endif
